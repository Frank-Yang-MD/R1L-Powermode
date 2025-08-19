/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONEVENTHANDLER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONEVENTHANDLER_H

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVpdNotificationSender;
class ISubscriptionManager;

/*
 * Handles events from BootDaemon
 */
class BootDaemonEventHandler final : public IEventHandler {
public:
    BootDaemonEventHandler(IVpdNotificationSender& vpdNotificationSender,
                           ISubscriptionManager& subscriptionManager);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    void processBootDaemonEvent(common::EVpdEvent event);

private:
    IVpdNotificationSender& mVpdNotificationSender;
    ISubscriptionManager& mSubscriptionManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_BOOTDAEMONEVENTHANDLER_H
