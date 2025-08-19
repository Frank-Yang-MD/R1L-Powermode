/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "BootdNotificationHandler.h"

#include "MockIBootDaemonCallback.h"
#include "VpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::StrictMock;

class BootdNotificationHandlerTest : public testing::Test {
protected:
    StrictMock<MockIBootDaemonCallback> mMockBootdCallback;
    BootdNotificationHandler mBootdNotificatioHandler;
};

TEST_F(BootdNotificationHandlerTest, return_false_if_no_callback_set)
{
    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        EXPECT_FALSE(mBootdNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(BootdNotificationHandlerTest, do_not_call_callback_if_not_bootd_notifcation)
{
    mBootdNotificatioHandler.setBootDaemonCallback(&mMockBootdCallback);

    for (const auto& vpdNotification : ALL_VPD_NOTIFICATIONS) {
        if (vpdNotification == common::EVpdNotifications::HmiEventVpsReady) {
            continue;
        }
        EXPECT_TRUE(mBootdNotificatioHandler.onVpdNotification(vpdNotification));
    }
}

TEST_F(BootdNotificationHandlerTest, call_with_correct_EHmiEvent_HmiEventVpsReady)
{
    mBootdNotificatioHandler.setBootDaemonCallback(&mMockBootdCallback);

    EXPECT_CALL(mMockBootdCallback, onHMIEvent(common::EHmiEvent::HmiEventVpsReady)).Times(1);

    EXPECT_TRUE(
        mBootdNotificatioHandler.onVpdNotification(common::EVpdNotifications::HmiEventVpsReady));
}

TEST_F(BootdNotificationHandlerTest, call_with_correct_EHmiEvent_EndEarlyHmi)
{
    mBootdNotificatioHandler.setBootDaemonCallback(&mMockBootdCallback);

    EXPECT_CALL(mMockBootdCallback, onHMIEvent(common::EHmiEvent::EndEarlyHmi)).Times(1);

    EXPECT_TRUE(mBootdNotificatioHandler.onVpdNotification(common::EVpdNotifications::EndEarlyHmi));
}

TEST_F(BootdNotificationHandlerTest, call_with_correct_EHmiEvent_ForcedCompletionEarlyServices)
{
    mBootdNotificatioHandler.setBootDaemonCallback(&mMockBootdCallback);

    EXPECT_CALL(mMockBootdCallback, onHMIEvent(common::EHmiEvent::ForcedCompletionEarlyServices))
        .Times(1);

    EXPECT_TRUE(mBootdNotificatioHandler.onVpdNotification(
        common::EVpdNotifications::ForcedCompletionEarlyServices));
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
