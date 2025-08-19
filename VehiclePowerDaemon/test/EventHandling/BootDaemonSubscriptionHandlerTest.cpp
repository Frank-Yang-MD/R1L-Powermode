/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonSubscriptionHandler.h"
#include "EVpdEvent.h"
#include "EVpdNotifications.h"
#include "EventHandlingMocks.h"
#include "MockVpdNotificationSender.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

using ::testing::Test;

static constexpr int mBootdSocketId{10};

class BootDaemonSubscriptionHandlerTest : public Test {
protected:
    MockSubscriptionManager mSubscriptionManagerMock;

    BootDaemonSubscriptionHandler mBootDaemonSubscriptionHandler{mSubscriptionManagerMock};
};

TEST_F(BootDaemonSubscriptionHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto bootDaemonSubscriptionHandler{
        std::make_unique<BootDaemonSubscriptionHandler>(mSubscriptionManagerMock)};
}

TEST_F(BootDaemonSubscriptionHandlerTest, subscribeBootDaemon)
{
    constexpr EVpdEvent subscribeBootDaemon = EVpdEvent::SubscribeBootDaemon;

    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(mBootdSocketId)).Times(1);

    mBootDaemonSubscriptionHandler.Process(mBootdSocketId, subscribeBootDaemon);
}

TEST_F(BootDaemonSubscriptionHandlerTest, must_not_subscribeBootDaemon)
{
    constexpr EVpdEvent stopFailedVps = EVpdEvent::StopFailedVps;

    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(mBootdSocketId)).Times(0);

    mBootDaemonSubscriptionHandler.Process(mBootdSocketId, stopFailedVps);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
