/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockActions.h"
#include "MockTimer.h"
#include "WatchDogTimerHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class TestITimerWdtrTrue : public ITimer {
public:
    void start() override {}
    bool isExpired() const override { return true; }
};

class TestITimerWdtrFalse : public ITimer {
public:
    void start() override {}
    bool isExpired() const override { return false; }
};

/*
This test verifies that the WatchDogTimerHandler calls the interface method start()
from the ITimer interface
*/
TEST(WatchDogTimerHandlerTest, startCall)
{
    // Arrangements
    MockTimer* mpITimer = new MockTimer;
    std::unique_ptr<MockTimer> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    WatchDogTimerHandler mWatchDogTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(*mpITimer, start());

    // Action
    mWatchDogTimerHandler.start();
}

/*
This test verifies that the WatchDogTimerHandler calls the interface method sendWdtrRequest()
from the interface IVcpuActions
*/
TEST(WatchDogTimerHandlerTest, sendWdtrRequestCall)
{
    // Arrangements
    TestITimerWdtrTrue* mpITimer = new TestITimerWdtrTrue;
    std::unique_ptr<TestITimerWdtrTrue> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    WatchDogTimerHandler mWatchDogTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendWdtrRequest());

    // Action
    mWatchDogTimerHandler.onTimeout();
}

/*
This test verifies that the WatchDogTimerHandler does not call the interface method
sendWdtrRequest() from the interface IVcpuActions
*/
TEST(WatchDogTimerHandlerTest, sendWdtrRequestNoCall)
{
    // Arrangements
    TestITimerWdtrFalse* mpITimer = new TestITimerWdtrFalse;
    std::unique_ptr<TestITimerWdtrFalse> mITimer(mpITimer);
    MockVcpuActions mVcpuActions;
    WatchDogTimerHandler mWatchDogTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendWdtrRequest()).Times(0);

    // Action
    mWatchDogTimerHandler.onTimeout();
}

/*
This test verifies that WatchDogTimerHandler does not invoke methods
on nullptr.
*/
TEST(WatchDogTimerHandlerTest, NullTimer)
{
    std::unique_ptr<MockTimer> mITimer(nullptr);
    MockVcpuActions mVcpuActions;
    WatchDogTimerHandler mWatchDogTimerHandler(std::move(mITimer), mVcpuActions);

    EXPECT_CALL(mVcpuActions, sendWdtrRequest()).Times(0);

    mWatchDogTimerHandler.start();
    mWatchDogTimerHandler.onTimeout();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
