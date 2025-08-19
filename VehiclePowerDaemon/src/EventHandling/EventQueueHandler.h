/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUEHANDLER_H_

#include <atomic>
#include <map>

#include "EVpdEvent.h"
#include "IEventHandler.h"
#include "IEventSource.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiver;

class EventQueueHandler {
public:
    EventQueueHandler(IEventSource&, IEventReceiver&);

    /**
     *  Start and run event handling processing
     */
    void run();

    /**
     *  Stop event handling processing
     */
    void stop();

    /**
     *  Set handler
     */
    void setEventHandler(common::EVpdEvent messageType, IEventHandler* eventHandler);

private:
    IEventSource& mEventSource;
    IEventReceiver& mEventReceiver;
    std::map<common::EVpdEvent, IEventHandler*> mHandlers;

    /* Exit flag */
    std::atomic<bool> mExitFlag{false};
    std::atomic<bool> mIsTerminated{false};
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTQUEUEHANDLER_H_
