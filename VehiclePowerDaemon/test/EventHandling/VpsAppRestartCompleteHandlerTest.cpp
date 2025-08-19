/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsAppRestartCompleteHandler.h"
#include "MockIAppRestartCompleteNotification.h"

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

class VpsAppRestartCompleteHandlerTest : public Test {
protected:
    ~VpsAppRestartCompleteHandlerTest();

    const int mAnyClient = 103;
    NiceMock<MockIAppRestartCompleteNotification> mAppRestartCompleteNotificationMock;

    VpsAppRestartCompleteHandler mVpsAppRestartCompleteHandler{mAppRestartCompleteNotificationMock};
};

VpsAppRestartCompleteHandlerTest::~VpsAppRestartCompleteHandlerTest() {}

TEST_F(VpsAppRestartCompleteHandlerTest,
       onAppRestartComplete_must_be_called_if_event_is_appRestartComplete)
{
    constexpr EVpdEvent appRestartCompleteEvent = EVpdEvent::AppRestartCompleteVps;

    EXPECT_CALL(mAppRestartCompleteNotificationMock, onAppRestartComplete()).Times(1);

    mVpsAppRestartCompleteHandler.Process(mAnyClient, appRestartCompleteEvent);
}

TEST_F(VpsAppRestartCompleteHandlerTest,
       onAppRestartComplete_must_not_be_called_if_event_is_not_appRestartComplete)
{
    constexpr EVpdEvent rebootEvent = EVpdEvent::Reboot;

    EXPECT_CALL(mAppRestartCompleteNotificationMock, onAppRestartComplete()).Times(0);

    mVpsAppRestartCompleteHandler.Process(mAnyClient, rebootEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
