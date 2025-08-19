/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsFwRestartCompleteHandler.h"
#include "MockIFwRestartCompleteNotification.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

using ::testing::NiceMock;
using ::testing::Test;

class VpsFwRestartCompleteHandlerTest : public Test {
protected:
    ~VpsFwRestartCompleteHandlerTest();

    const int mAnyClient = 103;
    NiceMock<MockIFwRestartCompleteNotification> mFwRestartCompleteNotificationMock;

    VpsFwRestartCompleteHandler mVpsFwRestartCompleteHandler{mFwRestartCompleteNotificationMock};
};

VpsFwRestartCompleteHandlerTest::~VpsFwRestartCompleteHandlerTest() {}

TEST_F(VpsFwRestartCompleteHandlerTest,
       onFwRestartComplete_must_be_called_if_event_is_fwRestartComplete)
{
    constexpr EVpdEvent fwRestartCompleteEvent = EVpdEvent::FwRestartCompleteVps;

    EXPECT_CALL(mFwRestartCompleteNotificationMock, onFwRestartComplete()).Times(1);

    mVpsFwRestartCompleteHandler.Process(mAnyClient, fwRestartCompleteEvent);
}

TEST_F(VpsFwRestartCompleteHandlerTest,
       onFwRestartComplete_must_not_be_called_if_event_is_fwRestartComplete)
{
    constexpr EVpdEvent rebootEvent = EVpdEvent::Reboot;

    EXPECT_CALL(mFwRestartCompleteNotificationMock, onFwRestartComplete()).Times(0);

    mVpsFwRestartCompleteHandler.Process(mAnyClient, rebootEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
