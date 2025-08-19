/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsSubscriptionHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpsSubscriptionHandler::VpsSubscriptionHandler(ISubscriptionManager& subscriptionManager)
    : mSusbscriptionManager(subscriptionManager)
{
}

void VpsSubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::SubscribeVps) {
        mSusbscriptionManager.setVpsID(clientId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsSubscriptionHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
