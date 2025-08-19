/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "HmiEventsHandler.h"
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
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::Test;

class HmiEventsHandlerTest : public Test {
protected:
    ~HmiEventsHandlerTest();

    const int mBootdSocketId{10};
    const int mVpsSocketId{11};
    NiceMock<MockSubscriptionManager> mSubscriptionManagerMock;
    NiceMock<MockVpdNotificationSender> mVpdNotificationSenderMock;

    HmiEventsHandler mHmiEventsHandler{mVpdNotificationSenderMock, mSubscriptionManagerMock};
};

HmiEventsHandlerTest::~HmiEventsHandlerTest() {}

TEST_F(HmiEventsHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto hmiEventsHandler{
        std::make_unique<HmiEventsHandler>(mVpdNotificationSenderMock, mSubscriptionManagerMock)};
}

TEST_F(HmiEventsHandlerTest, VpsReady_must_be_called_if_event_is_VpsReady)
{
    constexpr EVpdEvent vpsReadyEvent = EVpdEvent::VpsReady;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock,
                sendNotification(mBootdSocketId, common::EVpdNotifications::HmiEventVpsReady))
        .Times(1);

    mHmiEventsHandler.Process(mVpsSocketId, vpsReadyEvent);
}

TEST_F(HmiEventsHandlerTest, EndEarlyHmi_must_be_called_if_event_is_EndEarlyHmi)
{
    constexpr EVpdEvent vpsReadyEvent = EVpdEvent::EndEarlyHmi;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock,
                sendNotification(mBootdSocketId, common::EVpdNotifications::EndEarlyHmi))
        .Times(1);

    mHmiEventsHandler.Process(mVpsSocketId, vpsReadyEvent);
}

TEST_F(HmiEventsHandlerTest,
       ForcedCompletionEarlyServices_must_be_called_if_event_is_ForcedCompletionEarlyServices)
{
    constexpr EVpdEvent vpsReadyEvent = EVpdEvent::ForcedCompletionEarlyServices;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(
        mVpdNotificationSenderMock,
        sendNotification(mBootdSocketId, common::EVpdNotifications::ForcedCompletionEarlyServices))
        .Times(1);

    mHmiEventsHandler.Process(mVpsSocketId, vpsReadyEvent);
}

TEST_F(HmiEventsHandlerTest, VpdNotificationSender_must_not_be_called_if_event_is_wrong)
{
    constexpr EVpdEvent wrongEvent = EVpdEvent::AppStopCompleteVps;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock, sendNotification(_, _)).Times(0);

    mHmiEventsHandler.Process(mVpsSocketId, wrongEvent);
}

TEST_F(HmiEventsHandlerTest, VpdNotificationSender_must_not_be_called_if_vps_event_is_wrong)
{
    constexpr EVpdEvent wrongEvent = EVpdEvent::Unsupported;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(mVpsSocketId));

    EXPECT_CALL(mVpdNotificationSenderMock, sendNotification(_, _)).Times(0);

    mHmiEventsHandler.Process(mVpsSocketId, wrongEvent);
}

TEST_F(HmiEventsHandlerTest, VpdNotificationSender_must_not_be_called_if_vps_client_is_wrong)
{
    constexpr EVpdEvent wrongEvent = EVpdEvent::ForcedCompletionEarlyServices;

    ON_CALL(mSubscriptionManagerMock, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
    ON_CALL(mSubscriptionManagerMock, getVpsID()).WillByDefault(Return(0));

    EXPECT_CALL(mVpdNotificationSenderMock, sendNotification(_, _)).Times(0);

    mHmiEventsHandler.Process(mVpsSocketId, wrongEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
