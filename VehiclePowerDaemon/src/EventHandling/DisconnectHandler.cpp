/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "DisconnectHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

DisconnectHandler::DisconnectHandler(ISubscriptionManager& subscriptionManager)
    : mSusbscriptionManager(subscriptionManager)
{
}

void DisconnectHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::Disconnect) {
        mSusbscriptionManager.disconnect(clientId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::DisconnectHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
