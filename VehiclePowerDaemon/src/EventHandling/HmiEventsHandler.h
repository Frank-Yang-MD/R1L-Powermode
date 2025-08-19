/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_HMIEVENTSHANDLER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_HMIEVENTSHANDLER_H

#include "IEventHandler.h"
#include "ISubscriptionManager.h"
#include "IVpdNotificationSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles HMI events between BootDaemon and VPS
 */
class HmiEventsHandler final : public IEventHandler {
public:
    HmiEventsHandler(IVpdNotificationSender& inVpdNotificationSender,
                     ISubscriptionManager& inSubscriptionManager);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    void processBootdEvent(common::EVpdEvent event);
    void processVpsEvent(common::EVpdEvent event);

    IVpdNotificationSender& mVpdNotificationSender;
    ISubscriptionManager& mSubscriptionManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_HMIEVENTSHANDLER_H
