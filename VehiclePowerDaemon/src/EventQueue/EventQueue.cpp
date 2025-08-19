/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EventQueue.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

void EventQueue::onEventReceive(int id, common::EVpdEvent event)
{
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mQueue.push(std::make_pair(id, event));
    }

    mCond.notify_one();
}

std::pair<int, common::EVpdEvent> EventQueue::getEvent()
{
    std::unique_lock<std::mutex> lock(mMutex);
    mCond.wait(lock, [this] { return !mQueue.empty(); });

    auto ret = mQueue.front();
    mQueue.pop();

    return ret;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
