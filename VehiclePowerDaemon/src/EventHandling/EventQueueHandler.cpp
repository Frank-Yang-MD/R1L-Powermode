/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EventQueueHandler.h"
#include "IEventReceiver.h"
#include "VehiclePowerDaemonLog.h"

#include <thread>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

EventQueueHandler::EventQueueHandler(IEventSource& eventSource, IEventReceiver& eventReceiver)
    : mEventSource(eventSource)
    , mEventReceiver(eventReceiver)
    , mExitFlag(false)
{
}

void EventQueueHandler::run()
{
    IEventHandler* handler{nullptr};
    while (!mExitFlag) {
        auto event = mEventSource.getEvent();
        {
            auto it = mHandlers.find(event.second);

            if (it == mHandlers.end()) {
                MLOGW(LogFuncId::VPD,
                      LogId::EventQueueHandler_run_No_event_handler_found_for_event_index,
                      event.second);
                continue;
            }
            handler = it->second;
        }
        if (handler) {
            handler->Process(event.first, event.second);
        }
        else {
            MLOGI(LogFuncId::VPD, LogId::EventQueueHandler_run_Handler_for_event_is_null,
                  event.second);
        }
    }
    mIsTerminated.store(true);
}

void EventQueueHandler::stop()
{
    mExitFlag = true;
    mEventReceiver.onEventReceive(0, common::EVpdEvent::Timeout);
    while (mIsTerminated.load() == false) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void EventQueueHandler::setEventHandler(common::EVpdEvent event, IEventHandler* eventHandler)
{
    if (mHandlers.find(event) != mHandlers.end()) {
        MLOGW(LogFuncId::VPD,
              LogId::EventQueueHandler_setEventHandler_Event_handler_for_event_index_already_exists,
              event);
        return;
    }
    mHandlers[event] = eventHandler;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
