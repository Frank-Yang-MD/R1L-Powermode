/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EventQueue.h"
#include "EVpdEvent.h"
#include <chrono>
#include <gtest/gtest.h>
#include <thread>
#include <utility>
#include <vector>

using namespace std;
using namespace testing;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

extern vector<EVpdEvent> events;

using eventQueue_t = pair<int, EVpdEvent>;

/**
 * Test  for unit: EventQueue
 */
class EventQueueTest : public Test {
protected:
    EventQueue mEventQueue;
    vector<eventQueue_t> mTransmittedEvents;

    void SetUp() override
    {
        int id{};

        // pairs wich contain all EVpdEvent
        mTransmittedEvents.resize(events.size());
        for (const auto& e : events) {
            mTransmittedEvents[id++] = eventQueue_t(id, e);
        }
    }
};

/**
 * Put event to queue before call receive method
 */
TEST_F(EventQueueTest, PutGetEvent)
{
    for (const auto& e : mTransmittedEvents) {
        mEventQueue.onEventReceive(e.first, e.second);
    }

    for (const auto& e : mTransmittedEvents) {
        ASSERT_EQ(e, mEventQueue.getEvent());
    }
}

/**
 * Blocking test. Put event to queue after call receive method
 */
TEST_F(EventQueueTest, GetPutEvent)
{
    auto sleepFor = std::chrono::milliseconds(100);

    // thread for put events with some delay
    thread producer([&]() {
        for (const auto& e : mTransmittedEvents) {
            this_thread::sleep_for(sleepFor);
            mEventQueue.onEventReceive(e.first, e.second);
        }
    });

    for (const auto& e : mTransmittedEvents) {
        ASSERT_EQ(e, mEventQueue.getEvent());
    }

    producer.join();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
