/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "MockTimerCreator.h"
#include "MockVcpuResetTimer.h"

#include "TimerManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Test;

class TimerManagerTest : public Test {
public:
    TimerManagerTest()
    {
        ON_CALL(mMockTimerCreator, getAbnormalHandler()).WillByDefault(Return(&mMockAbnormalTimer));
        ON_CALL(mMockTimerCreator, getWatchDogHandler()).WillByDefault(Return(&mMockWatchDogTimer));

        mTimerManager = std::make_unique<TimerManager>(mMockTimerCreator);
    }

    ~TimerManagerTest();

protected:
    NiceMock<MockVcpuResetTimer> mMockAbnormalTimer;
    NiceMock<MockVcpuResetTimer> mMockWatchDogTimer;

    NiceMock<MockTimerCreator> mMockTimerCreator;

    std::unique_ptr<TimerManager> mTimerManager;
};

// Define empty desctructor to supress out-of-line definition warning
TimerManagerTest::~TimerManagerTest() {}

TEST_F(TimerManagerTest, initializeObject)
{
    InSequence seq;

    EXPECT_CALL(mMockTimerCreator, getAbnormalHandler()).Times(1);
    EXPECT_CALL(mMockTimerCreator, getWatchDogHandler()).Times(1);

    EXPECT_CALL(mMockAbnormalTimer, start()).Times(1);
    EXPECT_CALL(mMockWatchDogTimer, start()).Times(1);

    TimerManager timerManager{mMockTimerCreator};
}

TEST_F(TimerManagerTest, onTimeout_NoneTimerHandler)
{
    EXPECT_CALL(mMockAbnormalTimer, onTimeout()).Times(0);
    EXPECT_CALL(mMockWatchDogTimer, onTimeout()).Times(0);

    mTimerManager->onTimeout();
}

TEST_F(TimerManagerTest, onTimeout_ChangeToNoneTimerHandler)
{
    EXPECT_CALL(mMockAbnormalTimer, onTimeout()).Times(0);
    EXPECT_CALL(mMockWatchDogTimer, onTimeout()).Times(0);

    mTimerManager->changeTimerHandler(ETimerType::None);
    mTimerManager->onTimeout();
}

TEST_F(TimerManagerTest, onTimeout_AdtrTimerHandler)
{
    EXPECT_CALL(mMockAbnormalTimer, onTimeout()).Times(1);
    EXPECT_CALL(mMockWatchDogTimer, onTimeout()).Times(0);

    mTimerManager->changeTimerHandler(ETimerType::ADTRTimer);
    mTimerManager->onTimeout();
}

TEST_F(TimerManagerTest, onTimeout_WdtrTimerHandler)
{
    EXPECT_CALL(mMockAbnormalTimer, onTimeout()).Times(0);
    EXPECT_CALL(mMockWatchDogTimer, onTimeout()).Times(1);

    mTimerManager->changeTimerHandler(ETimerType::WDTRTimer);
    mTimerManager->onTimeout();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
