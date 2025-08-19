/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMON_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMON_H_

#include "ClientInteraction.h"
#include "CpuComClient.h"
#include "EventHandling.h"
#include "NativeRunnerHolder.h"
#include "NotifyHelper.h"
#include "PowerStateHolder.h"
#include "ShutdownManager.h"
#include "StateHolder.h"
#include "StateMachineHolder.h"
#include "SubscriptionManager.h"
#include "SubscriptionNotificationDecorator.h"
#include "SystemPowerStateListener.h"
#include "TimeoutNotification.h"
#include "TimerManager.h"
#include "VehicleConfigProvider.h"
#include "VpdRestartHolder.h"
#include "WakeUpEventDetector.h"
#include "WakeUpManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VehiclePowerDaemon final {
public:
    VehiclePowerDaemon(IAndroidPowerManager& androidPowerManager,
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
                       std::chrono::microseconds modulesUnloadTimeout);
    ~VehiclePowerDaemon();

    /**
     *  Start daemon
     */
    void StartDaemon();

    void StopDaemon();

private:
    std::unique_ptr<StateMachineHolder> mStateMachine;
    std::unique_ptr<SubscriptionManager> mSubscriptionManager;
    std::unique_ptr<EventHandling> mEventHandling;

    IAndroidPowerManager& mAndroidPowerManager;
    std::unique_ptr<ShutdownManager> mShutdownManager;
    IWakeLockManager& mWakeLockManager;
    IWakeUpManager& mWakeUpManager;
    IKernelModulesControl& mKernelModulesControl;

    ILogDaemonActions& mLogDaemonActions;

    std::unique_ptr<NativeRunnerHolder> mNativeRunnerHolder;
    IWakeUpEventDetector& mWakeUpEventDetector;
    const IConfigReader& mConfigProvider;
    std::unique_ptr<VehicleConfigProvider> mVConfigProvider;
    IEventSource& mEventSource;
    IEventReceiver& mEventReceiver;

    IVpdNotificationSender& mEventTransport;
    std::unique_ptr<ClientInteraction> mClientInteraction;
    std::unique_ptr<NotifyHelper> mNotifyHelper;

    std::unique_ptr<CpuComClient> mCpuClient;
    std::unique_ptr<TimeoutNotification> mTimeoutNotification;

    const IClock& mClock;
    std::unique_ptr<ITimeoutControl> mTimeoutControl;

    std::unique_ptr<ITimer> mCheckStartNativeTimer;
    std::unique_ptr<ITimer> mAppStopVpsTimer;
    std::unique_ptr<ITimer> mAppStopVpsTimeout;
    std::unique_ptr<ITimer> mFwStopVpsTimer;
    std::unique_ptr<ITimer> mFwStopVpsTimeout;
    std::unique_ptr<ITimer> mWaitForStopNativeSuspendCancelTimer;
    std::unique_ptr<ITimer> mStopNativeTimer;
    std::unique_ptr<ITimer> mLogWaitSavingTimer;
    std::unique_ptr<ITimer> mLogdogCheckSavingStateTimer;
    std::unique_ptr<ITimer> mLogdogSaveStateTimer;
    std::unique_ptr<ITimer> mLogdogStopTimer;
    std::unique_ptr<ITimer> mWakeUpKeyDetectorOpenTimer;
    std::unique_ptr<ITimer> mWakeUpKeyDetectorReadTimer;

    std::unique_ptr<ITimer> mModulesLoadTimeout;

    std::unique_ptr<ITimer> mModulesUnloadCheckTimer;
    std::unique_ptr<ITimer> mModulesUnloadTimeout;

    std::unique_ptr<ITimer> mAppResumeEfwTimer;
    std::unique_ptr<ITimer> mAppResumeAppTimer;

    std::unique_ptr<ITimer> mAppRestartEfwTimer;
    std::unique_ptr<ITimer> mAppRestartAppTimer;

    std::unique_ptr<ITimer> mUnmountTimer;

    std::unique_ptr<IStateHolder> mStateHolder;
    std::unique_ptr<ISubscriptionNotification> mSubscrNotificationDecorator;

    std::unique_ptr<TimerManager> mTimerManager;
    std::unique_ptr<PowerStateHolder> mPowerStateHolder;
    std::unique_ptr<SystemPowerStateListener> mSystemPowerStateListener;
    std::unique_ptr<VpdRestartHolder> mVpdRestartHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMON_H_
