/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "NativeClientNotificationHandler.h"

#include "MockINativeCallback.h"
#include "VpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::Eq;
using testing::StrictMock;

class NativeClientNotificationHandlerTest : public testing::Test {
protected:
    StrictMock<MockINativeCallback> mMockNativeCallback;
    NativeClientNotificationHandler mNativeClientNotificatioHandler;
};

TEST_F(NativeClientNotificationHandlerTest, return_false_if_no_callback_set)
{
    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        EXPECT_FALSE(mNativeClientNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(NativeClientNotificationHandlerTest, do_not_call_callback_if_not_bootd_notifcation)
{
    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        if (vpdNotification == common::EVpdNotifications::AppResume ||
            vpdNotification == common::EVpdNotifications::AppStartColdboot ||
            vpdNotification == common::EVpdNotifications::AppStartNa ||
            vpdNotification == common::EVpdNotifications::AppStop ||
            vpdNotification == common::EVpdNotifications::AppRestart) {
            continue;
        }
        EXPECT_TRUE(mNativeClientNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(NativeClientNotificationHandlerTest, call_with_correct_EVpdState_AppResume)
{
    const auto vpdState = common::EVpdState::APP_RESUME;

    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    EXPECT_CALL(mMockNativeCallback, onSystemStateChange(vpdState)).Times(1);

    EXPECT_TRUE(
        mNativeClientNotificatioHandler.onVpdNotification(common::EVpdNotifications::AppResume));

    // also check that getVpdState returns the same value
    EXPECT_THAT(mNativeClientNotificatioHandler.getVpdState(), Eq(vpdState));
}

TEST_F(NativeClientNotificationHandlerTest, call_with_correct_EVpdState_AppStartColdboot)
{
    const auto vpdState = common::EVpdState::APP_START_COLDBOOT;

    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    EXPECT_CALL(mMockNativeCallback, onSystemStateChange(vpdState)).Times(1);

    EXPECT_TRUE(mNativeClientNotificatioHandler.onVpdNotification(
        common::EVpdNotifications::AppStartColdboot));

    // also check that getVpdState returns the same value
    EXPECT_THAT(mNativeClientNotificatioHandler.getVpdState(), Eq(vpdState));
}

TEST_F(NativeClientNotificationHandlerTest, call_with_correct_EVpdState_AppStartNa)
{
    const auto vpdState = common::EVpdState::APP_START_NA;

    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    EXPECT_CALL(mMockNativeCallback, onSystemStateChange(vpdState)).Times(1);

    EXPECT_TRUE(
        mNativeClientNotificatioHandler.onVpdNotification(common::EVpdNotifications::AppStartNa));

    // also check that getVpdState returns the same value
    EXPECT_THAT(mNativeClientNotificatioHandler.getVpdState(), Eq(vpdState));
}

TEST_F(NativeClientNotificationHandlerTest, call_with_correct_EVpdState_AppStop)
{
    const auto vpdState = common::EVpdState::APP_STOP;

    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    EXPECT_CALL(mMockNativeCallback, onSystemStateChange(vpdState)).Times(1);

    EXPECT_TRUE(
        mNativeClientNotificatioHandler.onVpdNotification(common::EVpdNotifications::AppStop));

    // also check that getVpdState returns the same value
    EXPECT_THAT(mNativeClientNotificatioHandler.getVpdState(), Eq(vpdState));
}

TEST_F(NativeClientNotificationHandlerTest, call_with_correct_EVpdState_AppRestart)
{
    const auto vpdState = common::EVpdState::APP_RESTART;

    mNativeClientNotificatioHandler.setNativeCallback(&mMockNativeCallback);

    EXPECT_CALL(mMockNativeCallback, onSystemStateChange(vpdState)).Times(1);

    EXPECT_TRUE(
        mNativeClientNotificatioHandler.onVpdNotification(common::EVpdNotifications::AppRestart));

    // also check that getVpdState returns the same value
    EXPECT_THAT(mNativeClientNotificatioHandler.getVpdState(), Eq(vpdState));
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
