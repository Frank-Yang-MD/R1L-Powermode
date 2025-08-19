/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "MockPowerStateHolder.h"

#include "VcpuPowerStateChangeHandler.h"

#include <gtest/gtest.h>

#include <set>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::NiceMock;
using ::testing::Test;

class VcpuPowerStateChangeHandlerTest : public Test {
public:
    VcpuPowerStateChangeHandlerTest()
        : mVcpuPowerStateChangeHandler{mMockPowerStateChangeListener_stateMachine,
                                       mMockPowerStateChangeListener_powerStateHolder,
                                       mMockPowerStateChangeListener_powerStateChangeListener}
    {
    }

    ~VcpuPowerStateChangeHandlerTest();

protected:
    NiceMock<MockPowerStateChangeListener> mMockPowerStateChangeListener_stateMachine;
    NiceMock<MockPowerStateChangeListener> mMockPowerStateChangeListener_powerStateHolder;
    NiceMock<MockPowerStateChangeListener> mMockPowerStateChangeListener_powerStateChangeListener;

    VcpuPowerStateChangeHandler mVcpuPowerStateChangeHandler;
};

static const std::set<common::EVpdEvent> validEvents = {
    common::EVpdEvent::ShutdownCancel, common::EVpdEvent::Suspend, common::EVpdEvent::Shutdown,
    common::EVpdEvent::Reboot, common::EVpdEvent::RebootRecovery};

bool contains(const std::set<common::EVpdEvent>& set, common::EVpdEvent event)
{
    return set.find(event) != set.end();
}

PowerState vpdEventToPowerState(common::EVpdEvent event)
{
    PowerState powerState = PowerState::NORMAL;

    switch (event) {
    case common::EVpdEvent::ShutdownCancel:
        powerState = PowerState::SUSPEND_CANCEL;
        break;
    case common::EVpdEvent::Suspend:
        powerState = PowerState::SUSPEND;
        break;
    case common::EVpdEvent::Shutdown:
        powerState = PowerState::SHUTDOWN;
        break;
    case common::EVpdEvent::Reboot:
        powerState = PowerState::REBOOT;
        break;
    case common::EVpdEvent::RebootRecovery:
        powerState = PowerState::REBOOT_RECOVERY;
        break;
    default:
        break;
    }

    return powerState;
}

VcpuPowerStateChangeHandlerTest::~VcpuPowerStateChangeHandlerTest() {}

TEST_F(VcpuPowerStateChangeHandlerTest, generalTest)
{
    auto testFunc = [this](common::EVpdEvent event, int times) {
        EXPECT_CALL(mMockPowerStateChangeListener_stateMachine,
                    onPowerStateChange(vpdEventToPowerState(event)))
            .Times(times);
        EXPECT_CALL(mMockPowerStateChangeListener_powerStateHolder,
                    onPowerStateChange(vpdEventToPowerState(event)))
            .Times(times);
        EXPECT_CALL(mMockPowerStateChangeListener_powerStateChangeListener,
                    onPowerStateChange(vpdEventToPowerState(event)))
            .Times(times);

        mVcpuPowerStateChangeHandler.Process(0, event);
    };

    constexpr int once = 1;
    constexpr int never = 0;

    for (int i = 0; i < common::EVpdEvent::EVPDEVENT_COUNT; ++i) {
        auto event = static_cast<common::EVpdEvent>(i);
        if (contains(validEvents, event)) {
            testFunc(event, once);
        }
        else {
            testFunc(event, never);
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
