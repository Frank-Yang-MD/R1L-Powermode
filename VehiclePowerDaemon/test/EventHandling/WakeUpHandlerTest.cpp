/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUpHandler.h"

#include "MockIWakeUpNotification.h"

#include "EVpdEvent.h"

#include <gtest/gtest.h>

using ::com::mitsubishielectric::ahu::vehiclepwrmgr::common::EVpdEvent;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WakeUpHandlerTest : public ::testing::Test {
public:
    WakeUpHandlerTest()
        : mWakeUpHandler{mMockIWakeUpNotification}
    {
    }

    MockIWakeUpNotification mMockIWakeUpNotification;
    WakeUpHandler mWakeUpHandler;
};

TEST_F(WakeUpHandlerTest, process_method_must_call_onWakeUp)
{
    const int dummyClientId = 1;

    EXPECT_CALL(mMockIWakeUpNotification, onWakeUp()).Times(1);

    mWakeUpHandler.Process(dummyClientId, EVpdEvent::WakeUp);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
