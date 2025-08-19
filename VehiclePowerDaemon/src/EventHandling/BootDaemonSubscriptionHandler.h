/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONSUBSCRIPTIONHANDLER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONSUBSCRIPTIONHANDLER_H

#include "IEventHandler.h"
#include "ISubscriptionManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles subscription requests from BootDaemon
 */
class BootDaemonSubscriptionHandler final : public IEventHandler {
public:
    BootDaemonSubscriptionHandler(ISubscriptionManager& subscriptionManager);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    ISubscriptionManager& mSubscriptionManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONSUBSCRIPTIONHANDLER_H
