/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsStopCompleteHandler.h"

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

class VpsStopCompleteHandlerTest : public ::testing::Test {
public:
    VpsStopCompleteHandlerTest()
        : mVpsStopCompleteHandler{mMockStopCompleteVps}
    {
    }

    NiceMock<MockStopCompleteVps> mMockStopCompleteVps;

    VpsStopCompleteHandler mVpsStopCompleteHandler;
};

TEST_F(VpsStopCompleteHandlerTest,
       process_method_must_call_onAppStopCompleteVps_on_AppStopCompleteVps_event)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockStopCompleteVps, onAppStopCompleteVps()).Times(1);

    mVpsStopCompleteHandler.Process(dummyClientId, EVpdEvent::AppStopCompleteVps);
}

TEST_F(VpsStopCompleteHandlerTest,
       process_method_must_call_onFwStopCompleteVps_on_FwStopCompleteVps_event)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockStopCompleteVps, onFwStopCompleteVps()).Times(1);

    mVpsStopCompleteHandler.Process(dummyClientId, EVpdEvent::FwStopCompleteVps);
}

TEST_F(VpsStopCompleteHandlerTest, process_method_must_call_onStopFailedVps_on_StopFailedVps_event)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockStopCompleteVps, onStopFailedVps()).Times(1);

    mVpsStopCompleteHandler.Process(dummyClientId, EVpdEvent::StopFailedVps);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
