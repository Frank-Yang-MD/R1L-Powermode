/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <functional>
#include <future>
#include <gtest/gtest.h>
#include <thread>
#include <utility>

#include "EventHandling.h"
#include "EventHandlingMocks.h"
#include "EventQueueHandler.h"
#include "IEventHandler.h"
#include "IEventReceiverMock.h"
#include "VpdStateBase.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using event_t = std::pair<int, common::EVpdEvent>;

class HandlerForGeneralTest : public IEventHandler {
public:
    HandlerForGeneralTest(std::function<void()> stopCallBack)
        : mStopCallBack{stopCallBack}
    {
    }

    void Process(int, common::EVpdEvent) override { mStopCallBack(); }

private:
    std::function<void()> mStopCallBack;
};

class EventQueueHandlerTest : public Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

    event_t mEvent{0, common::EVpdEvent::Unsupported};
    event_t mBadEvent{1, common::EVpdEvent::Disconnect};
    MockEventSource mMockEventSource;
    IEventReceiverMock mMockEventReceiver;
    NiceMock<EventQueueHandler> mEventQueueHandler{mMockEventSource, mMockEventReceiver};
    std::mutex mMutex;
    std::condition_variable mHandlerIsCalledCv;

    HandlerForGeneralTest mHandler{[this]() { mHandlerIsCalledCv.notify_all(); }};
    HandlerForGeneralTest* mBadHandler{nullptr};

    VpdStateBase mVpdStateBase{nullptr};
    MockSubscriptionManager mMockSubscriptionManager;
};

TEST_F(EventQueueHandlerTest, GeneralTest)
{
    ON_CALL(mMockEventSource, getEvent()).WillByDefault(Return(mEvent));
    EXPECT_CALL(mMockEventSource, getEvent()).Times(AtLeast(1));

    mEventQueueHandler.setEventHandler(mEvent.second, &mHandler);

    std::thread testThread([this]() { mEventQueueHandler.run(); });
    std::unique_lock<std::mutex> lock{mMutex};
    mHandlerIsCalledCv.wait(lock);
    mEventQueueHandler.stop();
    testThread.join();
}

TEST_F(EventQueueHandlerTest, GeneralTestWithBadEvent)
{
    ON_CALL(mMockEventSource, getEvent()).WillByDefault(InvokeWithoutArgs([this]() {
        static int count_{1};
        if (count_--)
            return mBadEvent;
        else
            return mEvent;
    }));
    EXPECT_CALL(mMockEventSource, getEvent()).Times(AtLeast(1));

    mEventQueueHandler.setEventHandler(mEvent.second, &mHandler);

    std::thread testThread([this]() { mEventQueueHandler.run(); });
    std::unique_lock<std::mutex> lock{mMutex};
    mHandlerIsCalledCv.wait(lock);
    mEventQueueHandler.stop();
    testThread.join();
}

TEST_F(EventQueueHandlerTest, GeneralTestWithBadHandler)
{
    ON_CALL(mMockEventSource, getEvent()).WillByDefault(InvokeWithoutArgs([this]() {
        static int count_{1};
        if (count_--)
            return mBadEvent;
        else
            return mEvent;
    }));
    EXPECT_CALL(mMockEventSource, getEvent()).Times(AtLeast(1));

    mEventQueueHandler.setEventHandler(mBadEvent.second, mBadHandler);
    mEventQueueHandler.setEventHandler(mEvent.second, &mHandler);

    std::thread testThread([this]() { mEventQueueHandler.run(); });
    std::unique_lock<std::mutex> lock{mMutex};
    mHandlerIsCalledCv.wait(lock);
    mEventQueueHandler.stop();
    testThread.join();
}

/**
 * Check that setting same event handler several time is failed.
 * Test result is error message in logs
 */
TEST_F(EventQueueHandlerTest, SetEventHandlerTest)
{
    mEventQueueHandler.setEventHandler(mEvent.second, &mHandler);
    mEventQueueHandler.setEventHandler(mEvent.second, &mHandler);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
