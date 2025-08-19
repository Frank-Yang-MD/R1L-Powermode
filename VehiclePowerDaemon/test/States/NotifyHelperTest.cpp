/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gtest/gtest.h>

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockVcpuInteraction.h"
#include "MockVpdNotificationSender.h"
#include "MockVpdRestartHolder.h"
#include "NotifyHelper.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

extern std::vector<common::EVpdState> vpdStates;

extern std::vector<common::EVpdNotifications> vpdNotifications;

class NotifyHelperTest : public Test {
protected:
    void SetUp() override {}

    StrictMock<MockClientActions> mMockClientActions;
    StrictMock<MockVcpuInteraction> mMockVcpuInteraction;

    MockSubscriptionManager mMockSubscriptionManager;
    MockVpdNotificationSender mMockVpdNotificationSender;
    MockVpdRestartHolder mMockVpdRestartHolder;

    NotifyHelper mNotifyHelper{mMockClientActions, mMockSubscriptionManager,
                               mMockVpdNotificationSender, mMockVcpuInteraction,
                               mMockVpdRestartHolder};
};

/**
 * Check notification of native apps/services
 */
TEST_F(NotifyHelperTest, NotifyAllNative)
{
    const auto subscribers = std::vector<int>{1, 2, 3};

    EXPECT_CALL(mMockSubscriptionManager, getNativeSubscribers())
        .WillRepeatedly(Return(subscribers));

    for (const auto& state : vpdStates) {
        for (const auto& subscriber : subscribers) {
            EXPECT_CALL(mMockClientActions, notifySystemStateChange(subscriber, state)).Times(1);
        }
        mNotifyHelper.notifyAllNative(state);
    }
}

TEST_F(NotifyHelperTest, NotifyAllNegative)
{
    const auto subscribers = std::vector<int>{};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSubscriptionManager, getNativeSubscribers())
        .WillRepeatedly(Return(subscribers));

    for (const auto& state : vpdStates) {
        mNotifyHelper.notifyAllNative(state);
    }
}

/**
 * Check notification of Vps
 */
TEST_F(NotifyHelperTest, NotifyVps)
{
    const int vpsId{123};

    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdStates) {
        EXPECT_CALL(mMockClientActions, notifySystemStateChange(vpsId, state)).Times(1);
        mNotifyHelper.notifyVps(state);
    }
}

TEST_F(NotifyHelperTest, NotifyVpsEVpdNotification)
{
    const int vpsId{123};

    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdNotifications) {
        EXPECT_CALL(mMockVpdNotificationSender, sendNotification(vpsId, state)).Times(1);
        mNotifyHelper.notifyVps(state);
    }
}

TEST_F(NotifyHelperTest, NotifyVpsNegative)
{
    const int vpsId{-1};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdStates) {
        EXPECT_CALL(mMockClientActions, notifySystemStateChange(vpsId, state)).Times(0);
        mNotifyHelper.notifyVps(state);
    }
}

TEST_F(NotifyHelperTest, NotifyVpsEVpdNotificationNegative)
{
    const int vpsId{-1};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdNotifications) {
        mNotifyHelper.notifyVps(state);
    }
}

TEST_F(NotifyHelperTest, NotifyVpsNegativeVpdRestarted)
{
    const int vpsId{-1};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdStates) {
        EXPECT_CALL(mMockVcpuInteraction, sendSelfRebootTrue()).Times(1);
        mNotifyHelper.notifyVps(state);
    }
}

TEST_F(NotifyHelperTest, NotifyVpsEVpdNotificationNegativeVpdRestarted)
{
    const int vpsId{-1};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).WillRepeatedly(Return(vpsId));

    for (const auto& state : vpdNotifications) {
        EXPECT_CALL(mMockVcpuInteraction, sendSelfRebootTrue()).Times(1);
        mNotifyHelper.notifyVps(state);
    }
}

/**
 * Check notification of LogDog
 */
TEST_F(NotifyHelperTest, NotifyLogDog)
{
    const int logId{123};

    EXPECT_CALL(mMockSubscriptionManager, getLogID()).WillRepeatedly(Return(logId));

    for (const auto& state : vpdStates) {
        EXPECT_CALL(mMockClientActions, notifySystemStateChange(logId, state)).Times(1);
        mNotifyHelper.notifyLog(state);
    }
}

TEST_F(NotifyHelperTest, NotifyLogDogNegative)
{
    const int logId{-1};

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSubscriptionManager, getLogID()).WillRepeatedly(Return(logId));

    for (const auto& state : vpdStates) {
        EXPECT_CALL(mMockClientActions, notifySystemStateChange(logId, state)).Times(0);
        mNotifyHelper.notifyLog(state);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
