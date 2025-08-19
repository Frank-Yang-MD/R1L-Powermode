/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsAppResumeCompleteHandler.h"
#include "MockIAppResumeCompleteNotification.h"

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

class VpsAppResumeCompleteHandlerTest : public Test {
protected:
    ~VpsAppResumeCompleteHandlerTest();

    const int mAnyClient = 102;
    NiceMock<MockIAppResumeCompleteNotification> mAppResumeCompleteNotificationMock;

    VpsAppResumeCompleteHandler mVpsAppResumeCompleteHandler{mAppResumeCompleteNotificationMock};
};

VpsAppResumeCompleteHandlerTest::~VpsAppResumeCompleteHandlerTest() {}

TEST_F(VpsAppResumeCompleteHandlerTest,
       onAppResumeComplete_must_be_called_if_event_is_appResumeComplete)
{
    constexpr EVpdEvent appResumeCompleteEvent = EVpdEvent::AppResumeCompleteVps;

    EXPECT_CALL(mAppResumeCompleteNotificationMock, onAppResumeComplete()).Times(1);

    mVpsAppResumeCompleteHandler.Process(mAnyClient, appResumeCompleteEvent);
}

TEST_F(VpsAppResumeCompleteHandlerTest,
       onAppResumeComplete_must_not_be_called_if_event_is_appResumeComplete)
{
    constexpr EVpdEvent rebootEvent = EVpdEvent::Reboot;

    EXPECT_CALL(mAppResumeCompleteNotificationMock, onAppResumeComplete()).Times(0);

    mVpsAppResumeCompleteHandler.Process(mAnyClient, rebootEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
