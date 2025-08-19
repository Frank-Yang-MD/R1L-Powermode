/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

#include "MockVpdNotificationSender.h"
#include "StateSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

extern bool convertEVpdState2EVpdNotification(common::EVpdState vpdState,
                                              common::EVpdNotifications& vpdNotification);

extern std::vector<common::EVpdState> vpdStates;

std::vector<common::EVpdNotifications> vpdNotifications{
    common::EVpdNotifications::AppStartNa, common::EVpdNotifications::AppStartColdboot,
    common::EVpdNotifications::AppStop,    common::EVpdNotifications::AppResume,
    common::EVpdNotifications::AppRestart, common::EVpdNotifications::FwStop};

/**
 * Test Fixture Object
 */
class StateSenderTest : public ::testing::Test {
protected:
    MockVpdNotificationSender mockVpdNotificationSender;
};

TEST_F(StateSenderTest, trigger_destructors_for_all_memory_types)
{
    auto stateSender{std::make_unique<StateSender>(mockVpdNotificationSender)};
}

/**
 * In case of valid VpsState parameter 'sendNotification' method should be called with
 * corresponding VpdNotification parameter
 */
TEST_F(StateSenderTest, sendState_validState)
{
    StateSender stateSender(mockVpdNotificationSender);
    int someSocketId{0};

    for (const auto& validState : vpdStates) {
        common::EVpdNotifications validNotification;
        convertEVpdState2EVpdNotification(validState, validNotification);
        for (const auto& notification : vpdNotifications) {
            if (notification == validNotification) {
                EXPECT_CALL(mockVpdNotificationSender, sendNotification(someSocketId, notification))
                    .Times(1);
            }
            else {
                EXPECT_CALL(mockVpdNotificationSender, sendNotification(someSocketId, notification))
                    .Times(0);
            }
        }
        EXPECT_TRUE(stateSender.sendState(someSocketId, validState));
    }
}

/**
 * In case of invalid VpsState parameter 'sendNotification' method should not be called
 */
TEST_F(StateSenderTest, sendState_invalidState)
{
    StateSender stateSender(mockVpdNotificationSender);
    int someSocketId{0};
    common::EVpdNotifications invalidNotification;
    auto invalidState = common::EVpdState(10);
    convertEVpdState2EVpdNotification(invalidState, invalidNotification);

    for (const auto& notification : vpdNotifications) {
        EXPECT_CALL(mockVpdNotificationSender, sendNotification(someSocketId, notification))
            .Times(0);
    }
    EXPECT_CALL(mockVpdNotificationSender, sendNotification(someSocketId, invalidNotification))
        .Times(0);
    EXPECT_FALSE(stateSender.sendState(someSocketId, invalidState));
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
