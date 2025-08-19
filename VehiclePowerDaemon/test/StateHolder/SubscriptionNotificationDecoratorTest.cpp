/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SubscriptionNotificationDecorator.h"
#include "MockActions.h"
#include "MockStateHolder.h"
#include "MockSubscriptionNotification.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class SubscriptionNotificationDecoratorTest : public Test {
public:
    SubscriptionNotificationDecoratorTest()
        : mSubscriptionNotificationDecorator{mMockClientActions, mMockSubscriptionNotification,
                                             mMockStateHolder}
    {
    }

protected:
    void SetUp() override
    {
        ON_CALL(mMockStateHolder, getVpdState()).WillByDefault(Return(common::EVpdState::APP_STOP));
    }

    const int clientId{12345};

    MockClientActions mMockClientActions;
    MockSubscriptionNotification mMockSubscriptionNotification;
    MockStateHolder mMockStateHolder;

    SubscriptionNotificationDecorator mSubscriptionNotificationDecorator;
};

TEST_F(SubscriptionNotificationDecoratorTest, onSubscribeVps)
{
    EXPECT_CALL(mMockSubscriptionNotification, onSubscribeVps(clientId)).Times(1);
    mSubscriptionNotificationDecorator.onSubscribeVps(clientId);
}

TEST_F(SubscriptionNotificationDecoratorTest, onSubscribeNative)
{
    EXPECT_CALL(mMockStateHolder, getVpdState()).Times(1);
    EXPECT_CALL(mMockSubscriptionNotification, onSubscribeNative(clientId)).Times(1);
    EXPECT_CALL(mMockClientActions, notifySystemStateChange(clientId, common::EVpdState::APP_STOP));

    mSubscriptionNotificationDecorator.onSubscribeNative(clientId);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
