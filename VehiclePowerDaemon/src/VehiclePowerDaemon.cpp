/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VehiclePowerDaemon.h"

namespace {
using namespace std::chrono_literals;
const std::chrono::microseconds START_NATIVE_CHECK_INTERVAL{25ms};
const std::chrono::microseconds LOGDOG_SAVE_STATE_POOL_INTERVAL{5s};

const std::chrono::milliseconds TIMEOUT_BEFORE_SENDING_FW_STOP{300ms};
const std::chrono::milliseconds TIMEOUT_BEFORE_SENDING_APP_STOP_NATIVE{300ms};
const std::chrono::milliseconds LOG_ON_TIMEOUT{300ms};

const std::chrono::microseconds MODULES_UNLOAD_CHECK_INTERVAL{50ms};
}  // namespace

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VehiclePowerDaemon::VehiclePowerDaemon(
    IAndroidPowerManager& androidPowerManager,
    IWakeLockManager& wakeLockManager,
    IKernelModulesControl& kernelModulesControl,
    ILogDaemonActions& logDaemonActions,
    const IConfigReader& configReader,
    IEventSource& eventSource,
    IEventReceiver& eventReceiver,
    IVpdNotificationSender& vpdNotificationSender,
    std::function<std::unique_ptr<cpucom::v2::ICpuCom>()> cpuComFactory,
    const IClock& clock,
    IAndroidInit& androidInit,
    IWakeUpEventDetector& wakeUpEventDetector,
    IWakeUpManager& wakeUpManager,
    std::chrono::microseconds abnormalPeriod,
    std::chrono::microseconds watchdogPeriod,
    std::chrono::microseconds modulesUnloadTimeout)
    : mAndroidPowerManager{androidPowerManager}
    , mWakeLockManager{wakeLockManager}
    , mWakeUpManager{wakeUpManager}
    , mKernelModulesControl{kernelModulesControl}
    , mLogDaemonActions{logDaemonActions}
    , mWakeUpEventDetector{wakeUpEventDetector}
    , mConfigProvider{configReader}
    , mEventSource{eventSource}
    , mEventReceiver{eventReceiver}
    , mEventTransport{vpdNotificationSender}
    , mClock{clock}
{
    mVConfigProvider.reset(new VehicleConfigProvider{mConfigProvider});
    mNativeRunnerHolder.reset(
        new NativeRunnerHolder{*mVConfigProvider.get(), mConfigProvider, androidInit});

    mClientInteraction.reset(new ClientInteraction{mEventTransport});
    mCpuClient.reset(new CpuComClient{mEventReceiver, cpuComFactory});

    mTimeoutControl.reset(new TimeoutControl(mEventReceiver));
    mTimeoutNotification.reset(new TimeoutNotification{*mTimeoutControl.get(), *mCpuClient.get(),
                                                       abnormalPeriod, watchdogPeriod});

    mTimerManager.reset(new TimerManager(*mTimeoutNotification.get()));
    mPowerStateHolder.reset(new PowerStateHolder{});
    mSystemPowerStateListener.reset(
        new SystemPowerStateListener{*mCpuClient.get(), *mTimerManager.get()});

    mShutdownManager.reset(new ShutdownManager{mAndroidPowerManager, *mPowerStateHolder.get()});

    mSubscriptionManager.reset(new SubscriptionManager{});

    mCheckStartNativeTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, START_NATIVE_CHECK_INTERVAL});

    const auto stopVpsTimeout = mVConfigProvider.get()->getConfig().VPowerAppStopJavaTimeout;

    /*
     * There are two independent timeouts for waiting for AppStop and FwStop for VPS.
     * Tapp = T/2
     * Tfw = T - Tapp
     */
    mAppStopVpsTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{stopVpsTimeout / 2}});

    mAppStopVpsTimeout.reset(
        new Timer{*mTimeoutControl.get(), mClock, TIMEOUT_BEFORE_SENDING_APP_STOP_NATIVE});

    mFwStopVpsTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{stopVpsTimeout}});

    mFwStopVpsTimeout.reset(
        new Timer{*mTimeoutControl.get(), mClock, TIMEOUT_BEFORE_SENDING_FW_STOP});

    const auto appStopNativeTimeout =
        mVConfigProvider.get()->getConfig().VPowerAppStopNativeTimeout;
    mStopNativeTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appStopNativeTimeout}});

    mWaitForStopNativeSuspendCancelTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appStopNativeTimeout}});

    mLogWaitSavingTimer.reset(new Timer{*mTimeoutControl.get(), mClock, LOG_ON_TIMEOUT});

    mLogdogCheckSavingStateTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, LOGDOG_SAVE_STATE_POOL_INTERVAL});

    const auto logdogLogSaveTimeout = mVConfigProvider.get()->getConfig().VPowerLogSaveTimeout;
    mLogdogSaveStateTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{logdogLogSaveTimeout}});

    const auto logdogStopTimeout = mVConfigProvider.get()->getConfig().VPowerAppStopLogdogTimeout;
    mLogdogStopTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{logdogStopTimeout}});

    mModulesLoadTimeout.reset(new Timer{*mTimeoutControl.get(), mClock, modulesUnloadTimeout});

    mModulesUnloadCheckTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, MODULES_UNLOAD_CHECK_INTERVAL});
    mModulesUnloadTimeout.reset(new Timer{*mTimeoutControl.get(), mClock, modulesUnloadTimeout});

    const auto appResumeEfwTimeout = mVConfigProvider.get()->getConfig().VPowerAppResumeEfwTimeout;
    mAppResumeEfwTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appResumeEfwTimeout}});
    const auto appResumeAppTimeout = mVConfigProvider.get()->getConfig().VPowerAppResumeAppTimeout;
    mAppResumeAppTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appResumeAppTimeout}});

    mAppRestartEfwTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appResumeEfwTimeout}});
    mAppRestartAppTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{appResumeAppTimeout}});
    mUnmountTimer.reset(
        new Timer{*mTimeoutControl.get(), mClock, std::chrono::microseconds{stopVpsTimeout}});

    mVpdRestartHolder.reset(new VpdRestartHolder{*mNativeRunnerHolder.get()});

    mNotifyHelper.reset(new NotifyHelper(mClientInteraction->getClientActions(),
                                         *mSubscriptionManager.get(), mEventTransport,
                                         *mCpuClient.get(), *mVpdRestartHolder.get()));

    mStateHolder.reset(new StateHolder());

    mStateMachine.reset(new StateMachineHolder{
        *mNativeRunnerHolder.get(),
        *mCpuClient.get(),
        *mNotifyHelper.get(),
        mLogDaemonActions,
        *mShutdownManager.get(),
        mWakeLockManager,
        mWakeUpManager,
        *mSubscriptionManager.get(),
        mWakeUpEventDetector,
        mKernelModulesControl,
        *mPowerStateHolder.get(),
        *mCheckStartNativeTimer.get(),
        *mAppStopVpsTimer.get(),
        *mAppStopVpsTimeout.get(),
        *mFwStopVpsTimer.get(),
        *mFwStopVpsTimeout.get(),
        *mStopNativeTimer.get(),
        *mLogWaitSavingTimer.get(),
        *mLogdogCheckSavingStateTimer.get(),
        *mLogdogSaveStateTimer.get(),
        *mLogdogStopTimer.get(),
        *mModulesLoadTimeout.get(),
        *mModulesUnloadCheckTimer.get(),
        *mModulesUnloadTimeout.get(),
        *mAppResumeEfwTimer.get(),
        *mAppResumeAppTimer.get(),
        *mAppRestartEfwTimer.get(),
        *mAppRestartAppTimer.get(),
        *mUnmountTimer.get(),
        *mStateHolder.get(),
        *mVpdRestartHolder.get(),
        *mTimerManager.get(),
    });

    mSubscrNotificationDecorator.reset(
        new SubscriptionNotificationDecorator(mClientInteraction->getClientActions(),
                                              *mStateMachine->getVpdState(), *mStateHolder.get()));
    mSubscriptionManager->setSubscriptionNotification(mSubscrNotificationDecorator.get());
    mSubscriptionManager->setDisconnectNotification(mStateMachine->getVpdState());

    mEventHandling.reset(new EventHandling{mEventSource, *mStateMachine->getVpdState(),
                                           *mSubscriptionManager.get(), *mPowerStateHolder.get(),
                                           mEventTransport, mEventReceiver, *mTimerManager.get(),
                                           *mSystemPowerStateListener.get()});
}

VehiclePowerDaemon::~VehiclePowerDaemon()
{
    mEventHandling.reset();
    mSubscriptionManager->setSubscriptionNotification(nullptr);
    mStateMachine.reset();
    mSubscriptionManager.reset();
    mShutdownManager.reset();
    mTimeoutControl.reset();
    mTimeoutNotification.reset();
    mCpuClient.reset();
    mNotifyHelper.reset();
    mNativeRunnerHolder.reset();
    mVConfigProvider.reset();
}

void VehiclePowerDaemon::StartDaemon()
{
    // Add the initiated event
    mEventReceiver.onEventReceive(0, common::EVpdEvent::Timeout);

    // Start and run event handling loop
    mEventHandling->run();
}

void VehiclePowerDaemon::StopDaemon() { mEventHandling->stop(); }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
