/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "DisconnectHandler.h"

#include "MockISubscriptionManager.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using testing::Test;

class DisconnectHandlerTest : public Test {
protected:
    DisconnectHandlerTest()
        : mDisconnectHandler{mSubscriptionManagerMock}
    {
    }

    const int mClientId{111};
    MockISubscriptionManager mSubscriptionManagerMock;
    DisconnectHandler mDisconnectHandler;
};

TEST_F(DisconnectHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto disconnectHandler{std::make_unique<DisconnectHandler>(mSubscriptionManagerMock)};
}

TEST_F(DisconnectHandlerTest, must_call_disconnect_on_Disconnect_event)
{
    const common::EVpdEvent event{common::EVpdEvent::Disconnect};
    EXPECT_CALL(mSubscriptionManagerMock, disconnect(mClientId)).Times(1);
    mDisconnectHandler.Process(mClientId, event);
}

TEST_F(DisconnectHandlerTest, must_do_nothing_on_other_events)
{
    for (int i = static_cast<int>(common::EVpdEvent::Unsupported);
         i != static_cast<int>(common::EVpdEvent::EVPDEVENT_COUNT); i++) {
        if (i != common::EVpdEvent::Disconnect) {
            EXPECT_CALL(mSubscriptionManagerMock, disconnect(mClientId)).Times(0);
            mDisconnectHandler.Process(mClientId, static_cast<common::EVpdEvent>(i));
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
