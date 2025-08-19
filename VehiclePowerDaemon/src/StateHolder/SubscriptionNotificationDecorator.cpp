/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SubscriptionNotificationDecorator.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

SubscriptionNotificationDecorator::SubscriptionNotificationDecorator(
    IClientActions& clientActions,
    ISubscriptionNotification& subscriptionNotification,
    IStateHolder& stateHolder)
    : mClientActions{clientActions}
    , mSubscriptionNotification{subscriptionNotification}
    , mStateHolder{stateHolder}
{
}

void SubscriptionNotificationDecorator::onSubscribeVps(int clientId)
{
    MLOGI(LogFuncId::VPD, LogId::SubscriptionNotificationDecorator_onSubscribeVps_client_id,
          clientId);
    mSubscriptionNotification.onSubscribeVps(clientId);
}

void SubscriptionNotificationDecorator::onSubscribeNative(int clientId)
{
    MLOGI(LogFuncId::VPD, LogId::SubscriptionNotificationDecorator_onSubscribeNative_client_id,
          clientId);
    mClientActions.notifySystemStateChange(clientId, mStateHolder.getVpdState());
    mSubscriptionNotification.onSubscribeNative(clientId);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
