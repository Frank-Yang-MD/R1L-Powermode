/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonUnsubscriptionHandler.h"
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

using ::testing::_;
using ::testing::Return;
using ::testing::Test;

static constexpr int mBootdSocketId{10};

class BootDaemonUnsubscriptionHandlerTest : public Test {
protected:
    MockSubscriptionManager mSubscriptionManagerMock;

    BootDaemonUnsubscriptionHandler mBootDaemonUnsubscriptionHandler{mSubscriptionManagerMock};
};

TEST_F(BootDaemonUnsubscriptionHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto bootDaemonUnsubscriptionHandler{
        std::make_unique<BootDaemonUnsubscriptionHandler>(mSubscriptionManagerMock)};
}

TEST_F(BootDaemonUnsubscriptionHandlerTest, subscribeBootDaemon)
{
    constexpr EVpdEvent subscribeBootDaemon = EVpdEvent::UnsubscribeBootDaemon;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(_)).Times(1);
    EXPECT_CALL(mSubscriptionManagerMock, remove(_)).Times(1);

    mBootDaemonUnsubscriptionHandler.Process(mBootdSocketId, subscribeBootDaemon);
}

TEST_F(BootDaemonUnsubscriptionHandlerTest, must_not_remove_BootDaemonClient_another_event)
{
    constexpr EVpdEvent stopFailedVps = EVpdEvent::StopFailedVps;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(mBootdSocketId)).Times(0);

    mBootDaemonUnsubscriptionHandler.Process(mBootdSocketId, stopFailedVps);
}

TEST_F(BootDaemonUnsubscriptionHandlerTest, must_not_remove_BootDaemonClient_another_client)
{
    constexpr EVpdEvent subscribeBootDaemon = EVpdEvent::UnsubscribeBootDaemon;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(0));
    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(mBootdSocketId)).Times(0);

    mBootDaemonUnsubscriptionHandler.Process(mBootdSocketId, subscribeBootDaemon);
}

TEST_F(BootDaemonUnsubscriptionHandlerTest,
       must_not_remove_BootDaemonClient_another_client_and_event)
{
    constexpr EVpdEvent stopFailedVps = EVpdEvent::StopFailedVps;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(0));
    EXPECT_CALL(mSubscriptionManagerMock, setBootDaemonID(mBootdSocketId)).Times(0);

    mBootDaemonUnsubscriptionHandler.Process(mBootdSocketId, stopFailedVps);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
