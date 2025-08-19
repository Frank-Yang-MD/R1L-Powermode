/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockNotifyHelper.h"
#include "MockTimeOutControl.h"
#include "MockVehicleConfigProvider.h"
#include "MockWakeUpEventDetector.h"

#include "AbnormalTimerHandler.h"
#include "IKernelModulesControl.h"
#include "ILogDaemonActions.h"
#include "ITimerCreator.h"
#include "InitialState.h"
#include "LogDaemonBusy.h"
#include "NormalOperation.h"
#include "PollingWakeUp.h"
#include "Resume.h"
#include "ResumeProcessingStartComplete.h"
#include "SendShutdownProcessingStart.h"
#include "StateMachineHolder.h"
#include "Timer.h"
#include "VpdClock.h"
#include "WaitBeforeSendingAppStopNativeServices.h"
#include "WaitBeforeSendingFwStopVps.h"
#include "WaitForAppRestartComplete.h"
#include "WaitForAppResumeComplete.h"
#include "WaitForAppStopVps.h"
#include "WaitForFwRestartComplete.h"
#include "WaitForFwResumeComplete.h"
#include "WaitForFwStopVps.h"
#include "WaitForLogDaemonStop.h"
#include "WaitForLogSaving.h"
#include "WaitForModulesLoadAfterSuspendCancel.h"
#include "WaitForModulesUnload.h"
#include "WaitForMount.h"
#include "WaitForNativeServicesStart.h"
#include "WaitForStartCpuCom.h"
#include "WaitForUnmount.h"
#include "WaitForVpsStart.h"
#include "WaitForVpsStartComplete.h"
#include "WaitNativeAppStopProcessingComplete.h"
#include "WakeUp.h"
#include "WatchDogTimerHandler.h"

#include "MockAndroidPowerManager.h"
#include "MockIDisconnectNotification.h"
#include "MockKernelModulesControl.h"
#include "MockNativeRunnerHolder.h"
#include "MockPowerStateHolder.h"
#include "MockStateHolder.h"
#include "MockTimerCreator.h"
#include "MockTimerManager.h"
#include "MockVcpuInteraction.h"
#include "MockVpdRestartHolder.h"
#include "MockWakeUpManager.h"

using namespace std;
using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StateMachineHolderTest : public Test {
protected:
    unique_ptr<INativeRunnerHolder> mNativeRunnerHolder;
    unique_ptr<IVcpuInteraction> mVcpuInteraction;
    unique_ptr<INotifyHelper> mNotifyHelper;
    unique_ptr<ILogDaemonActions> mLogDaemonActions;
    unique_ptr<IAndroidPowerManager> mAndroidPowerManager;
    unique_ptr<ShutdownManager> mShutdownManager;
    unique_ptr<IWakeLockManager> mWakeLockManager;
    unique_ptr<IWakeUpManager> mWakeUpManager;
    unique_ptr<IAndroidInit> mAndroidInit;
    unique_ptr<IVehicleConfigProvider> mVehicleConfigProvider;
    unique_ptr<IClock> mClock;
    unique_ptr<ITimeoutControl> mTimeoutControl;
    unique_ptr<ISubscriptionManager> mSubscriptionManager;
    unique_ptr<MockWakeUpEventDetector> mWakeUpEventDetector;
    unique_ptr<ITimerCreator> mTimerCreator;
    unique_ptr<IKernelModulesControl> mKernelModuleControl;
    unique_ptr<IPowerStateHolder> mPowerStateHolder;

    unique_ptr<StateMachineHolder> mStateMachineHolder;

    unique_ptr<Timer> mTimer;
    unique_ptr<IStateHolder> mStateHolder;
    unique_ptr<IVpdRestartHolder> mVpdRestartHolder;
    unique_ptr<ITimerManager> mTimerManager;
    unique_ptr<IDisconnectNotification> mDisconnectNotification;

    void SetUp() override
    {
        mNativeRunnerHolder.reset(new MockNativeRunnerHolder());
        mVcpuInteraction.reset(new MockVcpuInteraction());
        mNotifyHelper.reset(new MockNotifyHelper());
        mLogDaemonActions.reset(new MockLogDaemonActions());
        mAndroidInit.reset(new MockAndroidInit());
        mAndroidPowerManager.reset(new MockAndroidPowerManager{});
        mShutdownManager.reset(
            new ShutdownManager{*mAndroidPowerManager.get(), *mPowerStateHolder.get()});
        mWakeLockManager.reset(new MockWakeLockManager{});
        mWakeUpManager.reset(new MockWakeUpManager{});
        mVehicleConfigProvider.reset(new MockVehicleConfigProvider());
        mSubscriptionManager.reset(new MockSubscriptionManager());
        mTimer.reset(
            new Timer(*mTimeoutControl.get(), *mClock.get(), (std::chrono::microseconds)0));
        mWakeUpEventDetector.reset(new MockWakeUpEventDetector{});
        mTimerCreator.reset(new MockTimerCreator());
        mStateHolder.reset(new MockStateHolder());
        mKernelModuleControl.reset(new MockKernelModulesControl{});
        mPowerStateHolder.reset(new MockPowerStateHolder{});
        mVpdRestartHolder.reset(new MockVpdRestartHolder{});
        mTimerManager.reset(new MockTimerManager());
        mDisconnectNotification.reset(new MockIDisconnectNotification());

        struct TimeoutConfig config {
            100, 100, 100, 100, 100, 100
        };
        EXPECT_CALL(*(dynamic_cast<MockVehicleConfigProvider*>(mVehicleConfigProvider.get())),
                    getConfig())
            .WillRepeatedly(ReturnRef(config));

        std::vector<int> subscribers = {5, 2, 9, 8, 4};
        EXPECT_CALL(*(dynamic_cast<MockSubscriptionManager*>(mSubscriptionManager.get())),
                    getNativeSubscribers())
            .WillRepeatedly(Return(subscribers));

        mStateMachineHolder.reset(new StateMachineHolder(
            *mNativeRunnerHolder.get(), *mVcpuInteraction.get(), *mNotifyHelper.get(),
            *mLogDaemonActions.get(), *mShutdownManager.get(), *mWakeLockManager.get(),
            *mWakeUpManager.get(), *mSubscriptionManager.get(), *mWakeUpEventDetector.get(),
            *mKernelModuleControl.get(), *mPowerStateHolder.get(), *mTimer.get(), *mTimer.get(),
            *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(),
            *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(),
            *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(), *mTimer.get(),
            *mTimer.get(), *mStateHolder.get(), *mVpdRestartHolder.get(), *mTimerManager.get()));
    }
};

TEST_F(StateMachineHolderTest, InitStates)
{
    // S0
    ASSERT_NE(InitialState::getInstance(), nullptr);

    // S1
    ASSERT_NE(WaitForStartCpuCom::getInstance(), nullptr);

    // S2
    ASSERT_NE(WaitForNativeServicesStart::getInstance(), nullptr);

    // S3
    ASSERT_NE(WaitForVpsStart::getInstance(), nullptr);

    // S4
    ASSERT_NE(WaitForVpsStartComplete::getInstance(), nullptr);

    // S5
    ASSERT_NE(NormalOperation::getInstance(), nullptr);

    // S6
    ASSERT_NE(WaitForAppStopVps::getInstance(), nullptr);

    ASSERT_NE(WaitBeforeSendingFwStopVps::getInstance(), nullptr);

    ASSERT_NE(WaitForFwStopVps::getInstance(), nullptr);

    ASSERT_NE(WaitBeforeSendingAppStopNativeServices::getInstance(), nullptr);

    // S7
    ASSERT_NE(WaitNativeAppStopProcessingComplete::getInstance(), nullptr);

    // S7.1
    ASSERT_NE(WaitForLogSaving::getInstance(), nullptr);

    // S8
    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);

    // S10
    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);

    ASSERT_NE(WaitForModulesUnload::getInstance(), nullptr);

    ASSERT_NE(WaitForModulesLoadAfterSuspendCancel::getInstance(), nullptr);

    ASSERT_NE(SendShutdownProcessingStart::getInstance(), nullptr);

    // S11.1
    ASSERT_NE(PollingWakeUp::getInstance(), nullptr);

    ASSERT_NE(ResumeProcessingStartComplete::getInstance(), nullptr);

    ASSERT_NE(WakeUp::getInstance(), nullptr);

    // S12
    ASSERT_NE(Resume::getInstance(), nullptr);

    ASSERT_NE(WaitForFwResumeComplete::getInstance(), nullptr);

    ASSERT_NE(WaitForFwRestartComplete::getInstance(), nullptr);

    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);

    ASSERT_NE(WaitForAppResumeComplete::getInstance(), nullptr);
    ASSERT_NE(WaitForMount::getInstance(), nullptr);
    ASSERT_NE(WaitForUnmount::getInstance(), nullptr);

    auto stateMachine = mStateMachineHolder->getVpdState();
    ASSERT_NE(stateMachine, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
