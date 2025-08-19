/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

#include "MockTimeOutControl.h"
#include "Timer.h"
#include "VpdClock.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::StrEq;

class MockIClock : public IClock {
public:
    MOCK_CONST_METHOD0(clock, std::chrono::time_point<std::chrono::steady_clock>());
};

/*
This test verifies that the Timer calls the interface method setupTimeout()
from the ITimeoutControl1 interface
*/
TEST(TimerTest, setupTimeoutCall)
{
    // Arrangements
    MockTimeOutControl mTimeoutControl;
    MockIClock mIClock;
    Timer mTimer(mTimeoutControl, mIClock, (std::chrono::microseconds)0);

    EXPECT_CALL(mIClock, clock()).Times(1);

    EXPECT_CALL(mTimeoutControl, setupTimeout((std::chrono::microseconds)0)).Times(1);

    // Action
    mTimer.start();
}

/*
This test verifies that the Timer calls the interface method clock()
from the IClock interface
*/
TEST(TimerTest, clockCall)
{
    // Arrangements
    MockTimeOutControl mTimeoutControl;
    MockIClock mIClock;
    Timer mTimer(mTimeoutControl, mIClock, (std::chrono::microseconds)0);

    EXPECT_CALL(mIClock, clock()).Times(1);

    EXPECT_CALL(mTimeoutControl, setupTimeout((std::chrono::microseconds)0)).Times(1);

    // Action
    mTimer.start();
}

TEST(TimerTest, notExpired)
{
    // Arrangements
    MockTimeOutControl mTimeoutControl;
    VpdClock mIClock;
    Timer mTimer(mTimeoutControl, mIClock, (std::chrono::microseconds)1000);

    EXPECT_CALL(mTimeoutControl, setupTimeout(_)).Times(1);

    mTimer.start();

    std::this_thread::sleep_for(std::chrono::microseconds{500});
    EXPECT_FALSE(mTimer.isExpired());
}

TEST(TimerTest, Expired)
{
    // Arrangements
    MockTimeOutControl mTimeoutControl;
    VpdClock mIClock;
    Timer mTimer(mTimeoutControl, mIClock, (std::chrono::microseconds)1000);

    EXPECT_CALL(mTimeoutControl, setupTimeout(_)).Times(1);

    mTimer.start();

    std::this_thread::sleep_for(std::chrono::microseconds{1500});

    EXPECT_TRUE(mTimer.isExpired());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
