/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "NotificationReceiver.h"

#include "MockIInternalCallback.h"
#include "MockIVpdNotificationHandler.h"

#include "VpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::StrictMock;

class NotificationReceiverTest : public testing::Test {
protected:
    StrictMock<MockIInternalCallback> mMockInternalCallback;
    StrictMock<MockIVpdNotificationHandler> mMockVpdNotificationHandler;
    NotificationReceiver mNotificationReceiver;
};

TEST_F(NotificationReceiverTest, internalCallback_onConnect_no_callback)
{
    EXPECT_CALL(mMockInternalCallback, onConnect()).Times(0);
    mNotificationReceiver.onConnect();
}

TEST_F(NotificationReceiverTest, internalCallback_onDisconnect_no_callback)
{
    EXPECT_CALL(mMockInternalCallback, onDisconnect()).Times(0);
    mNotificationReceiver.onDisconnect();
}

TEST_F(NotificationReceiverTest, internalCallback_onConnect)
{
    mNotificationReceiver.setInternalCallback(&mMockInternalCallback);

    EXPECT_CALL(mMockInternalCallback, onConnect()).Times(1);

    mNotificationReceiver.onConnect();
}

TEST_F(NotificationReceiverTest, internalCallback_onDisconnect)
{
    mNotificationReceiver.setInternalCallback(&mMockInternalCallback);

    EXPECT_CALL(mMockInternalCallback, onDisconnect()).Times(1);

    mNotificationReceiver.onDisconnect();
}

TEST_F(NotificationReceiverTest, process_notification)
{
    const auto vpdNotification = common::EVpdNotifications::AppStartColdboot;
    mNotificationReceiver.addHandler(vpdNotification, &mMockVpdNotificationHandler);

    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification)).Times(1);

    mNotificationReceiver.onNotificationReceived(vpdNotification);
}

TEST_F(NotificationReceiverTest, process_notification_no_hanlder)
{
    using testing::_;

    const auto vpdNotification = common::EVpdNotifications::AppStartColdboot;
    const auto wrongVpdNotification = common::EVpdNotifications::AppRestart;

    mNotificationReceiver.addHandler(vpdNotification, &mMockVpdNotificationHandler);

    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(_)).Times(0);

    mNotificationReceiver.onNotificationReceived(wrongVpdNotification);
}

TEST_F(NotificationReceiverTest, process_notification_skiped_notification)
{
    using testing::_;
    using testing::Return;

    const auto vpdNotification = common::EVpdNotifications::AppStartColdboot;

    mNotificationReceiver.addHandler(vpdNotification, &mMockVpdNotificationHandler);

    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification))
        .Times(2)
        .WillOnce(Return(false))
        .WillOnce(Return(true));

    mNotificationReceiver.onNotificationReceived(vpdNotification);

    mNotificationReceiver.processReceivedNotifications();

    // verify that handler is called only once
    mNotificationReceiver.processReceivedNotifications();
}

TEST_F(NotificationReceiverTest, process_notification_skiped_multiple_notifications)
{
    using testing::_;
    using testing::Return;

    const auto vpdNotification1 = common::EVpdNotifications::AppStartColdboot;
    const auto vpdNotification2 = common::EVpdNotifications::AppRestart;

    mNotificationReceiver.addHandler(vpdNotification1, &mMockVpdNotificationHandler);
    mNotificationReceiver.addHandler(vpdNotification2, &mMockVpdNotificationHandler);

    // Expect adding vpdNotification1 to postponed notifications at the first call and trying to
    // re-send on vpdNotification2 receiving
    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification1))
        .WillOnce(Return(false))
        .WillOnce(Return(false));
    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification2))
        .WillOnce(Return(false));

    mNotificationReceiver.onNotificationReceived(vpdNotification1);
    mNotificationReceiver.onNotificationReceived(vpdNotification2);

    // Return false for the first time in order to cover all branches.
    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification2))
        .WillOnce(Return(false))
        .WillOnce(Return(true));
    EXPECT_CALL(mMockVpdNotificationHandler, onVpdNotification(vpdNotification1))
        .WillOnce(Return(true));

    mNotificationReceiver.processReceivedNotifications();

    // verify that handler is called only once for each notification
    mNotificationReceiver.processReceivedNotifications();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
