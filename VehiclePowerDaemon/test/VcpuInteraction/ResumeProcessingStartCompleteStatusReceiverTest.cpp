/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeProcessingStartCompleteStatusReceiver.h"
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

class ResumeProcessingStartCompleteStatusReceiverTest : public Test {
public:
    ResumeProcessingStartCompleteStatusReceiverTest()
        : mResumeProcessingStartCompleteStatusReceiver{mEventReceiver}
    {
    }

    MockEventReceiver mEventReceiver;
    ResumeProcessingStartCompleteStatusReceiver mResumeProcessingStartCompleteStatusReceiver;
};

TEST_F(ResumeProcessingStartCompleteStatusReceiverTest, onDeliveryStatusTrue)
{
    EXPECT_CALL(mEventReceiver,
                onEventReceive(INTERNAL_CLIENT_ID,
                               common::EVpdEvent::ResumeProcessingStartCompleteSentSuccess))
        .Times(1);

    mResumeProcessingStartCompleteStatusReceiver.onDeliveryStatus(true);
}

TEST_F(ResumeProcessingStartCompleteStatusReceiverTest, onDeliveryStatusFalse)
{
    EXPECT_CALL(mEventReceiver,
                onEventReceive(INTERNAL_CLIENT_ID,
                               common::EVpdEvent::ResumeProcessingStartCompleteSentFailure))
        .Times(1);

    mResumeProcessingStartCompleteStatusReceiver.onDeliveryStatus(false);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
