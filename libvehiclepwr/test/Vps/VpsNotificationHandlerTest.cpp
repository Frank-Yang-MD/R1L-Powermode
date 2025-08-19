/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "VpsNotificationHandler.h"

#include "MockIVpsCallback.h"
#include "VpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::StrictMock;

class VpsNotificationHandlerTest : public testing::Test {
protected:
    StrictMock<MockIVpsCallback> mMockVpsCallback;
    VpsNotificationHandler mVpsNotificatioHandler;
};

TEST_F(VpsNotificationHandlerTest, return_false_if_no_callback_set)
{
    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        EXPECT_FALSE(mVpsNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(VpsNotificationHandlerTest, do_not_call_callback_if_not_bootd_notifcation)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        if (vpdNotification == common::EVpdNotifications::FwStop ||
            vpdNotification == common::EVpdNotifications::FwResume ||
            vpdNotification == common::EVpdNotifications::FwRestart ||
            vpdNotification == common::EVpdNotifications::TimeOutError ||
            vpdNotification == common::EVpdNotifications::HandOffResponsibility ||
            vpdNotification ==
                common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed ||
            vpdNotification ==
                common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed) {
            continue;
        }
        EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(VpsNotificationHandlerTest, call_FwStop_on_FwStop_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, onFwStop()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::FwStop));
}

TEST_F(VpsNotificationHandlerTest, call_FwResume_on_FwResume_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, onFwResume()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::FwResume));
}

TEST_F(VpsNotificationHandlerTest, call_FwRestart_on_FwRestart_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, onFwRestart()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::FwRestart));
}

TEST_F(VpsNotificationHandlerTest, call_TimeOutError_on_TimeOutError_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, onTimeOutError()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::TimeOutError));
}

TEST_F(VpsNotificationHandlerTest, call_HandOffResponsibility_on_HandOffResponsibility_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, handOffResponsibility()).Times(1);

    EXPECT_TRUE(
        mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::HandOffResponsibility));
}

TEST_F(
    VpsNotificationHandlerTest,
    call_HandOffResponsibilityStartupAnimationAndDiDisplayed_on_HandOffResponsibilityStartupAnimationAndDiDisplayed_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, handOffResponsibilityStartupAnimationAndDiDisplayed()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(
        common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));
}

TEST_F(
    VpsNotificationHandlerTest,
    call_HandOffResponsibilityStartupAnimationDisplayed_on_HandOffResponsibilityStartupAnimationDisplayed_notification)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, handOffResponsibilityStartupAnimationDisplayed()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(
        common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));
}

TEST_F(VpsNotificationHandlerTest, onVpdNotificationCheckRequestUnmountCalled)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, requestUnmount()).Times(1);

    EXPECT_TRUE(
        mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::RequestUnmount));
}

TEST_F(VpsNotificationHandlerTest, onVpdNotificationCheckRequestMountCalled)
{
    mVpsNotificatioHandler.setVpsCallback(&mMockVpsCallback);

    EXPECT_CALL(mMockVpsCallback, requestMount()).Times(1);

    EXPECT_TRUE(mVpsNotificatioHandler.onVpdNotification(common::EVpdNotifications::RequestMount));
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
