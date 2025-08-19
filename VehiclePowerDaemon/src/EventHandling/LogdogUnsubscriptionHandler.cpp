/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogdogUnsubscriptionHandler.h"
#include "SubscriptionManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

UnsubscriptionHandlerLogdog::UnsubscriptionHandlerLogdog(ISubscriptionManager& subscriptionManager)
    : mSubscriptionManager(subscriptionManager)
{
}

void UnsubscriptionHandlerLogdog::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::UnsubscribeLogdog &&
        clientId == mSubscriptionManager.getLogID()) {
        mSubscriptionManager.setLogID(SubscriptionManager::voidId);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::UnsubscriptionHandlerLogdog_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
