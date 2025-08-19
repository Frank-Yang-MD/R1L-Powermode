/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonUnsubscriptionHandler.h"
#include "SubscriptionManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

BootDaemonUnsubscriptionHandler::BootDaemonUnsubscriptionHandler(
    ISubscriptionManager& subscriptionManager)
    : mSubscriptionManager(subscriptionManager)
{
}

void BootDaemonUnsubscriptionHandler::Process(int clientId, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::UnsubscribeBootDaemon &&
        clientId == mSubscriptionManager.getBootDaemonID()) {
        mSubscriptionManager.setBootDaemonID(SubscriptionManager::voidId);
        mSubscriptionManager.remove(clientId);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com