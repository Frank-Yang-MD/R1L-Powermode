/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUE_H_

#include "EVpdEvent.h"
#include "IEventReceiver.h"
#include "IEventSource.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <utility>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Thread safe queue for read/write message
 */
class EventQueue
    : public IEventReceiver
    , public IEventSource {
public:
    /**
     * Push new message to queue
     */
    virtual void onEventReceive(int id, common::EVpdEvent event) override;
    /**
     * Get message from Queue
     */
    virtual std::pair<int, common::EVpdEvent> getEvent() override;

private:
    std::queue<std::pair<int, common::EVpdEvent>> mQueue;
    std::mutex mMutex;
    std::condition_variable mCond;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUE_H_
