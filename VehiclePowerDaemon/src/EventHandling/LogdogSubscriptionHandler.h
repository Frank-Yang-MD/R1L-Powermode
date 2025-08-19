/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONHANDLERLOGDOG_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONHANDLERLOGDOG_H_

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
class SubscriptionHandlerLogdog final : public IEventHandler {
public:
    SubscriptionHandlerLogdog(ISubscriptionManager& subscriptionManager);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    ISubscriptionManager& mSusbscriptionManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONHANDLERLOGDOG_H_
