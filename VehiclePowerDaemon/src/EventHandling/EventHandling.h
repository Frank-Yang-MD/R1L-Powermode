/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because it's hard to test.

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLING_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLING_H_

#include "EVpdEvent.h"
#include "EventQueueHandler.h"
#include "IEventHandler.h"
#include "IEventSource.h"
#include "IPowerStateHolder.h"
#include "ISubscriptionManager.h"
#include "ITimeoutNotification.h"
#include "IVpdNotificationSender.h"
#include "IVpdState.h"

#include <map>
#include <memory>

using namespace std;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiver;

class EventHandling final {
public:
    EventHandling(IEventSource& inIEventSource,
                  IVpdState& inIVpdState,
                  ISubscriptionManager& inISubscriptionManager,
                  IPowerStateHolder& inIPowerStateHolder,
                  IVpdNotificationSender& inVpdNotificationSender,
                  IEventReceiver& inIEventReceiver,
                  ITimeoutNotification& inTimeoutNotification,
                  IPowerStateChangeListener& powerStateChangeListener);

    void run();
    void stop();

private:
    map<common::EVpdEvent, shared_ptr<IEventHandler>> mHandlers;
    shared_ptr<EventQueueHandler> mEventQueueHandler;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLING_H_

// LCOV_EXCL_STOP
