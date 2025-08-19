/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsMountHandler.h"

#include "EventHandlingMocks.h"

#include "EVpdEvent.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::com::mitsubishielectric::ahu::vehiclepwrmgr::common::EVpdEvent;

using ::testing::NiceMock;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpsMountHandlerTest : public ::testing::Test {
public:
    VpsMountHandlerTest()
        : mVpsUnmountCompleteHandler{mMockUnmountCompleteVps}
    {
    }

    NiceMock<MockVpsMount> mMockUnmountCompleteVps;

    VpsMountHandler mVpsUnmountCompleteHandler;
};

TEST_F(VpsMountHandlerTest, process_method_must_call_onVpsUnmountComplete_on_UnmountComplete_event)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockUnmountCompleteVps, onVpsUnmountComplete()).Times(1);

    mVpsUnmountCompleteHandler.Process(dummyClientId, EVpdEvent::UnmountComplete);
}

TEST_F(VpsMountHandlerTest, process_method_must_call_onVpsUnmountComplete_on_default_event)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockUnmountCompleteVps, onVpsUnmountComplete()).Times(0);

    mVpsUnmountCompleteHandler.Process(dummyClientId, EVpdEvent::Timeout);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
