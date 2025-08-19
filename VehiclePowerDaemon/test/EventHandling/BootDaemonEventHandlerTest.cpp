/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonEventHandler.h"
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
static constexpr int mVpsSocketId{11};

class BootDaemonEventHandlerTest : public Test {
protected:
    MockSubscriptionManager mSubscriptionManagerMock;
    MockVpdNotificationSender mVpdNotificationSenderMock;

    BootDaemonEventHandler mBootDaemonEventHandler{mVpdNotificationSenderMock,
                                                   mSubscriptionManagerMock};
};

TEST_F(BootDaemonEventHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto bootDaemonEventHandler{std::make_unique<BootDaemonEventHandler>(mVpdNotificationSenderMock,
                                                                         mSubscriptionManagerMock)};
}

TEST_F(BootDaemonEventHandlerTest, Send_handOffResponsibility)
{
    constexpr EVpdEvent handOffResponsibilityEvent = EVpdEvent::handOffResponsibility;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock,
                sendNotification(mVpsSocketId, common::EVpdNotifications::HandOffResponsibility))
        .Times(1);

    mBootDaemonEventHandler.Process(mBootdSocketId, handOffResponsibilityEvent);
}

TEST_F(BootDaemonEventHandlerTest, Send_handOffResponsibilityStartupAnimationDisplayed)
{
    constexpr EVpdEvent handOffResponsibilityEvent =
        EVpdEvent::handOffResponsibilityStartupAnimationDisplayed;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(
        mVpdNotificationSenderMock,
        sendNotification(mVpsSocketId,
                         common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed))
        .Times(1);

    mBootDaemonEventHandler.Process(mBootdSocketId, handOffResponsibilityEvent);
}

TEST_F(BootDaemonEventHandlerTest, Send_handOffResponsibilityStartupAnimationAndDiDisplayed)
{
    constexpr EVpdEvent handOffResponsibilityEvent =
        EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock,
                sendNotification(
                    mVpsSocketId,
                    common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed))
        .Times(1);

    mBootDaemonEventHandler.Process(mBootdSocketId, handOffResponsibilityEvent);
}

TEST_F(BootDaemonEventHandlerTest, ProcessVpsEvent_False_Test)
{
    constexpr EVpdEvent unsupported = EVpdEvent::Unsupported;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock, sendNotification(mVpsSocketId, _)).Times(0);

    mBootDaemonEventHandler.Process(mBootdSocketId, unsupported);
}

TEST_F(BootDaemonEventHandlerTest, getBootDaemonID_False_Test)
{
    constexpr EVpdEvent unsupported = EVpdEvent::Unsupported;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(0));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock, sendNotification(mVpsSocketId, _)).Times(0);

    mBootDaemonEventHandler.Process(mBootdSocketId, unsupported);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
