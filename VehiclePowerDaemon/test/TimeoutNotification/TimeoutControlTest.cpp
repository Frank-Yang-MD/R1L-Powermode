/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <IEventReceiver.h>
#include <TimeoutControl.h>

#include <algorithm>
#include <iostream>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using time_point = std::chrono::time_point<std::chrono::steady_clock>;

using trigger_timepoints = std::vector<time_point>;

const int EXPECTED_DELTA_MICROSECONDS = 2000;

trigger_timepoints actualTimepoints;

class TestEventReceiver : public IEventReceiver {
public:
    virtual void onEventReceive(int, common::EVpdEvent) override
    {
        actualTimepoints.emplace_back(std::chrono::steady_clock::now());
    }
};

class TimeoutControlTest : public ::testing::Test {
public:
    TimeoutControlTest()
        : timeoutControl{eventReceiver}
    {
        actualTimepoints.clear();
    }

    ~TimeoutControlTest() = default;

    time_point start;
    TestEventReceiver eventReceiver;
    TimeoutControl timeoutControl;

    std::vector<int> expectedTimeouts;

    void startTimeoutAccurancyTest()
    {
        start = std::chrono::steady_clock::now();

        for (unsigned int i = 0; i < expectedTimeouts.size(); ++i) {
            timeoutControl.setupTimeout(std::chrono::microseconds{expectedTimeouts[i]});
        }

        auto maxTimeout = *std::max_element(begin(expectedTimeouts), end(expectedTimeouts));

        std::sort(begin(expectedTimeouts), end(expectedTimeouts));

        // wait max timeout time + 1 sec for all timeouts trigger
        std::this_thread::sleep_for(std::chrono::microseconds{maxTimeout + 100000});

        int actualDelta = 0;
        int actualTimeout = 0;
        for (unsigned int i = 0; i < actualTimepoints.size(); ++i) {
            actualTimeout =
                std::chrono::duration_cast<std::chrono::microseconds>(actualTimepoints[i] - start)
                    .count();
            actualDelta = abs(expectedTimeouts[i] - actualTimeout);
            EXPECT_GE(EXPECTED_DELTA_MICROSECONDS, actualDelta);
        }
    }
};

TEST_F(TimeoutControlTest, SequentialSetupTimeoutTest)
{
    expectedTimeouts = {
        10000,  // 10 ms
        35000,  // 35 ms
        52000,  // 52 ms
        90000,  // 90 ms
    };

    startTimeoutAccurancyTest();
}

TEST_F(TimeoutControlTest, RandomSetupTimeoutTest)
{
    expectedTimeouts = {
        52000,  // 52 ms
        10000,  // 10 ms
        90000,  // 90 ms
        35000,  // 35 ms
    };

    startTimeoutAccurancyTest();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
