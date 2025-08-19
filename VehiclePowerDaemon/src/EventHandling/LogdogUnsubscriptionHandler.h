/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_UNSUBSCRIPTIONHANDLERLOGDOG_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_UNSUBSCRIPTIONHANDLERLOGDOG_H_

#include "IEventHandler.h"
#include "ISubscriptionManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles subscription requests for Logdog
 */
class UnsubscriptionHandlerLogdog final : public IEventHandler {
public:
    UnsubscriptionHandlerLogdog(ISubscriptionManager& subscriptionManager);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    ISubscriptionManager& mSubscriptionManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_UNSUBSCRIPTIONHANDLERLOGDOG_H_
