/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsFwResumeCompleteHandler.h"
#include "MockIFwResumeCompleteNotification.h"

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

class VpsFwResumeCompleteHandlerTest : public Test {
protected:
    ~VpsFwResumeCompleteHandlerTest();

    const int mAnyClient = 102;
    NiceMock<MockIFwResumeCompleteNotification> mFwResumeCompleteNotificationMock;

    VpsFwResumeCompleteHandler mVpsFwResumeCompleteHandler{mFwResumeCompleteNotificationMock};
};

VpsFwResumeCompleteHandlerTest::~VpsFwResumeCompleteHandlerTest() {}

TEST_F(VpsFwResumeCompleteHandlerTest,
       onFwResumeComplete_must_be_called_if_event_is_fwResumeComplete)
{
    constexpr EVpdEvent fwResumeCompleteEvent = EVpdEvent::FwResumeCompleteVps;

    EXPECT_CALL(mFwResumeCompleteNotificationMock, onFwResumeComplete()).Times(1);

    mVpsFwResumeCompleteHandler.Process(mAnyClient, fwResumeCompleteEvent);
}

TEST_F(VpsFwResumeCompleteHandlerTest,
       onFwResumeComplete_must_not_be_called_if_event_is_fwResumeComplete)
{
    constexpr EVpdEvent rebootEvent = EVpdEvent::Reboot;

    EXPECT_CALL(mFwResumeCompleteNotificationMock, onFwResumeComplete()).Times(0);

    mVpsFwResumeCompleteHandler.Process(mAnyClient, rebootEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
