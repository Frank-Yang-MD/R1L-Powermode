/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "UnsubscriptionHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

UnsubscriptionHandler::UnsubscriptionHandler(ISubscriptionManager& subscriptionManager)
    : mSusbscriptionManager(subscriptionManager)
{
}

void UnsubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::UnsubscribeNative) {
        mSusbscriptionManager.remove(clientId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::UnsubscriptionHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
