/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdStateMachine.h"
#include "MockSM.h"
#include "MockStates.h"
#include <gtest/gtest.h>

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpdStateMachineTest : public Test {
protected:
    MockSM mMockSM;
    MockVpdStateBase mMockVpdStateBase{&mMockSM};
    MockSomeState mMockSomeState{&mMockSM};

    VpdStateMachine mVpdStateMachine;

    void SetUp() override
    {
        EXPECT_CALL(mMockVpdStateBase, onEnter()).Times(1);
        mVpdStateMachine.transitionTo(&mMockVpdStateBase);
    }
};

TEST_F(VpdStateMachineTest, TransitionTo)
{
    EXPECT_CALL(mMockVpdStateBase, onExit()).Times(1);
    EXPECT_CALL(mMockSomeState, onEnter()).Times(1);

    mVpdStateMachine.transitionTo(&mMockSomeState);
}

TEST_F(VpdStateMachineTest, onSubscribeNative)
{
    constexpr int id{};
    EXPECT_CALL(mMockVpdStateBase, onSubscribeNative(id)).Times(1);

    mVpdStateMachine.onSubscribeNative(id);
}

TEST_F(VpdStateMachineTest, onSubscribeVps)
{
    constexpr int id{};
    EXPECT_CALL(mMockVpdStateBase, onSubscribeVps(id)).Times(1);

    mVpdStateMachine.onSubscribeVps(id);
}

TEST_F(VpdStateMachineTest, onStartCompleteCpuCom)
{
    EXPECT_CALL(mMockVpdStateBase, onStartCompleteCpuCom()).Times(1);

    mVpdStateMachine.onStartCompleteCpuCom();
}

TEST_F(VpdStateMachineTest, onStopFailedVps)
{
    EXPECT_CALL(mMockVpdStateBase, onStopFailedVps()).Times(1);

    mVpdStateMachine.onStopFailedVps();
}

TEST_F(VpdStateMachineTest, onPowerStateChange)
{
    const std::vector<PowerState> powerStates = {
        PowerState::NORMAL,   PowerState::SUSPEND_CANCEL, PowerState::SUSPEND,
        PowerState::SHUTDOWN, PowerState::REBOOT,         PowerState::REBOOT_RECOVERY,
    };

    for (const auto& powerState : powerStates) {
        EXPECT_CALL(mMockVpdStateBase, onPowerStateChange(powerState)).Times(1);
        mVpdStateMachine.onPowerStateChange(powerState);
    }
}

TEST_F(VpdStateMachineTest, onStartCompleteVps)
{
    EXPECT_CALL(mMockVpdStateBase, onStartCompleteVps()).Times(1);

    mVpdStateMachine.onStartCompleteVps();
}

TEST_F(VpdStateMachineTest, onStartFailedVps)
{
    EXPECT_CALL(mMockVpdStateBase, onStartFailedVps()).Times(1);

    mVpdStateMachine.onStartFailedVps();
}

TEST_F(VpdStateMachineTest, onStopCompleteNative)
{
    constexpr int id{};
    EXPECT_CALL(mMockVpdStateBase, onStopCompleteNative(id)).Times(1);

    mVpdStateMachine.onStopCompleteNative(id);
}

TEST_F(VpdStateMachineTest, onStopCompleteLog)
{
    EXPECT_CALL(mMockVpdStateBase, onStopCompleteLog()).Times(1);

    mVpdStateMachine.onStopCompleteLog();
}

TEST_F(VpdStateMachineTest, onTimeout)
{
    EXPECT_CALL(mMockVpdStateBase, onTimeout()).Times(1);

    mVpdStateMachine.onTimeout();
}

TEST_F(VpdStateMachineTest, onAppStopCompleteVps)
{
    EXPECT_CALL(mMockVpdStateBase, onAppStopCompleteVps()).Times(1);

    mVpdStateMachine.onAppStopCompleteVps();
}

TEST_F(VpdStateMachineTest, onFwStopCompleteVps)
{
    EXPECT_CALL(mMockVpdStateBase, onFwStopCompleteVps()).Times(1);

    mVpdStateMachine.onFwStopCompleteVps();
}

TEST_F(VpdStateMachineTest, onFwResumeComplete)
{
    EXPECT_CALL(mMockVpdStateBase, onFwResumeComplete()).Times(1);

    mVpdStateMachine.onFwResumeComplete();
}

TEST_F(VpdStateMachineTest, onFwRestartComplete)
{
    EXPECT_CALL(mMockVpdStateBase, onFwRestartComplete()).Times(1);

    mVpdStateMachine.onFwRestartComplete();
}

TEST_F(VpdStateMachineTest, onAppResumeComplete)
{
    EXPECT_CALL(mMockVpdStateBase, onAppResumeComplete()).Times(1);

    mVpdStateMachine.onAppResumeComplete();
}

TEST_F(VpdStateMachineTest, onAppRestartComplete)
{
    EXPECT_CALL(mMockVpdStateBase, onAppRestartComplete()).Times(1);

    mVpdStateMachine.onAppRestartComplete();
}

TEST_F(VpdStateMachineTest, onVpsUnmountComplete)
{
    EXPECT_CALL(mMockVpdStateBase, onVpsUnmountComplete()).Times(1);

    mVpdStateMachine.onVpsUnmountComplete();
}

TEST_F(VpdStateMachineTest, onShutdownProcessingStartSent)
{
    EXPECT_CALL(mMockVpdStateBase, onShutdownProcessingStartSent()).Times(1);

    mVpdStateMachine.onShutdownProcessingStartSent();
}

TEST_F(VpdStateMachineTest, onResumeProcessingStartCompleteSentSuccess)
{
    EXPECT_CALL(mMockVpdStateBase, onResumeProcessingStartCompleteSentSuccess()).Times(1);

    mVpdStateMachine.onResumeProcessingStartCompleteSentSuccess();
}

TEST_F(VpdStateMachineTest, onResumeProcessingStartCompleteSentFailure)
{
    EXPECT_CALL(mMockVpdStateBase, onResumeProcessingStartCompleteSentFailure()).Times(1);

    mVpdStateMachine.onResumeProcessingStartCompleteSentFailure();
}

TEST_F(VpdStateMachineTest, onWakeUp)
{
    EXPECT_CALL(mMockVpdStateBase, onWakeUp()).Times(1);

    mVpdStateMachine.onWakeUp();
}

TEST_F(VpdStateMachineTest, onDisconnectVps)
{
    EXPECT_CALL(mMockVpdStateBase, onDisconnectVps()).Times(1);

    mVpdStateMachine.onDisconnectVps();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
