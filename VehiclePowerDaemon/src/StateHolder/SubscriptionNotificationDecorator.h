/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONNOTIFICATIONDECORATOR_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONNOTIFICATIONDECORATOR_H

#include "IClientActions.h"
#include "IStateHolder.h"
#include "ISubscriptionNotification.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class SubscriptionNotificationDecorator : public ISubscriptionNotification {
public:
    SubscriptionNotificationDecorator(IClientActions& clientActions,
                                      ISubscriptionNotification& subscriptionNotification,
                                      IStateHolder& stateHolder);

    virtual void onSubscribeNative(int clientId) override;
    virtual void onSubscribeVps(int clientId) override;

private:
    IClientActions& mClientActions;
    ISubscriptionNotification& mSubscriptionNotification;
    IStateHolder& mStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCRIPTIONNOTIFICATIONDECORATOR_H
