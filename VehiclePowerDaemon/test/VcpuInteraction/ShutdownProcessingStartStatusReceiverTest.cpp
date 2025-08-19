/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ShutdownProcessingStartStatusReceiver.h"
#include "EVpdEvent.h"
#include "MockEventReceiver.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::Test;

static constexpr int INTERNAL_CLIENT_ID = 0;

class ShutdownProcessingStartStatusReceiverTest : public Test {
public:
    ShutdownProcessingStartStatusReceiverTest()
        : mShutdownProcessingStartStatusReceiver{mEventReceiver}
    {
    }

    MockEventReceiver mEventReceiver;
    ShutdownProcessingStartStatusReceiver mShutdownProcessingStartStatusReceiver;
};

TEST_F(ShutdownProcessingStartStatusReceiverTest, onDeliveryStatusTrue)
{
    EXPECT_CALL(mEventReceiver,
                onEventReceive(INTERNAL_CLIENT_ID, common::EVpdEvent::ShutdownProcessingStartSent))
        .Times(1);

    mShutdownProcessingStartStatusReceiver.onDeliveryStatus(true);
}

TEST_F(ShutdownProcessingStartStatusReceiverTest, onDeliveryStatusFalse)
{
    EXPECT_CALL(mEventReceiver,
                onEventReceive(INTERNAL_CLIENT_ID, common::EVpdEvent::ShutdownProcessingStartSent))
        .Times(0);

    mShutdownProcessingStartStatusReceiver.onDeliveryStatus(false);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
