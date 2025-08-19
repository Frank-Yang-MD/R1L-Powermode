/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "AbnormalTimerHandler.h"
#include "MockActions.h"
#include "MockTimer.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class TestITimerAdtrTrue : public ITimer {
public:
    void start() override {}
    bool isExpired() const override { return true; }
};

class TestITimerAdtrFalse : public ITimer {
public:
    void start() override {}
    bool isExpired() const override { return false; }
};

/*
This test verifies that the AbnormalTimerHandlerTest calls the interface method start()
from the ITimer interface
*/
TEST(AbnormalTimerHandlerTest, startCall)
{
    // Arrangements
    MockTimer* mpITimer = new MockTimer;
    std::unique_ptr<MockTimer> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    AbnormalTimerHandler mAbnormalTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(*mpITimer, start());

    // Action
    mAbnormalTimerHandler.start();
}

/*
This test verifies that the AbnormalTimerHandlerTest calls the interface method sendAdtrRequest()
from the interface IVcpuActions
*/
TEST(AbnormalTimerHandlerTest, sendAdtrRequestCall)
{
    // Arrangements
    TestITimerAdtrTrue* mpITimer = new TestITimerAdtrTrue;
    std::unique_ptr<TestITimerAdtrTrue> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    AbnormalTimerHandler mAbnormalTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendAdtrRequest());

    // Action
    mAbnormalTimerHandler.onTimeout();
}

/*
This test verifies that the AbnormalTimerHandlerTest does not call the interface method
sendAdtrRequest() from the interface IVcpuActions
*/
TEST(AbnormalTimerHandlerTest, sendAdtrRequestNoCall)
{
    // Arrangements
    TestITimerAdtrFalse* mpITimer = new TestITimerAdtrFalse;
    std::unique_ptr<TestITimerAdtrFalse> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    AbnormalTimerHandler mAbnormalTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendAdtrRequest()).Times(0);

    // Action
    mAbnormalTimerHandler.onTimeout();
}

/*
This test verifies that AbnormalTimerHandler does not invoke methods
on nullptr.
*/
TEST(AbnormalTimerHandlerTest, NullTimer)
{
    std::unique_ptr<MockTimer> mITimer(nullptr);
    MockVcpuActions mVcpuActions;
    AbnormalTimerHandler mAbnormalTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendAdtrRequest()).Times(0);

    mAbnormalTimerHandler.start();
    mAbnormalTimerHandler.onTimeout();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
