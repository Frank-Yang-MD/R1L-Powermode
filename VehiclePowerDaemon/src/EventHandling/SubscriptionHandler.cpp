/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SubscriptionHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

SubscriptionHandler::SubscriptionHandler(ISubscriptionManager& subscriptionManager)
    : mSusbscriptionManager(subscriptionManager)
{
}

void SubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::SubscribeNative) {
        mSusbscriptionManager.add(clientId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::SubscriptionHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
