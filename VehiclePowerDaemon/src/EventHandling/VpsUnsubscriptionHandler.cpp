/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsUnsubscriptionHandler.h"
#include "SubscriptionManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpsUnsubscriptionHandler::VpsUnsubscriptionHandler(ISubscriptionManager& subscriptionManager)
    : mSubscriptionManager(subscriptionManager)
{
}

void VpsUnsubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::UnsubscribeVps && clientId == mSubscriptionManager.getVpsID()) {
        mSubscriptionManager.setVpsID(SubscriptionManager::voidId);
        mSubscriptionManager.remove(clientId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsUnsubscriptionHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
