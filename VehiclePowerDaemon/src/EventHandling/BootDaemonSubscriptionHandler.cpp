/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonSubscriptionHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

BootDaemonSubscriptionHandler::BootDaemonSubscriptionHandler(
    ISubscriptionManager& subscriptionManager)
    : mSubscriptionManager(subscriptionManager)
{
}

void BootDaemonSubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::SubscribeBootDaemon) {
        mSubscriptionManager.setBootDaemonID(clientId);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com