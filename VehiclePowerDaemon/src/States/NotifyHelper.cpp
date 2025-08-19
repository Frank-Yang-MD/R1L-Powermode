/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NotifyHelper.h"
#include "SubscriptionManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

NotifyHelper::NotifyHelper(IClientActions& clientActions,
                           ISubscriptionManager& subscriptionManager,
                           IVpdNotificationSender& vpdNotificationSender,
                           IVcpuActions& vcpuActions,
                           IVpdRestartHolder& vpdRestartHolder)
    : mClientActions{clientActions}
    , mSubscriptionManager{subscriptionManager}
    , mVpdNotificationSender{vpdNotificationSender}
    , mVcpuActions{vcpuActions}
    , mVpdRestartHolder{vpdRestartHolder}
{
}

void NotifyHelper::notifyAllNative(common::EVpdState vpdState)
{
    const auto subscribers = mSubscriptionManager.getNativeSubscribers();

    if (subscribers.empty()) {
        MLOGW(LogFuncId::VPD, LogId::NotifyHelper_noNativeSubscribers);
        return;
    }

    for (const auto& subscriber : subscribers) {
        MLOGI(LogFuncId::VPD, LogId::NotifyHelper_notifyAllNative_client_id_state, subscriber,
              static_cast<int>(vpdState));
        mClientActions.notifySystemStateChange(subscriber, vpdState);
    }
}  // LCOV_EXCL_LINE

void NotifyHelper::notifyVps(common::EVpdState vpdState)
{
    if (mSubscriptionManager.getVpsID() == SubscriptionManager::voidId) {
        MLOGW(LogFuncId::VPD, LogId::NotifyHelper_vpsNotSubscribed);
        if (mVpdRestartHolder.isVpdRestarted()) {
            mVcpuActions.sendSelfRebootTrue();
        }
        return;
    }
    mClientActions.notifySystemStateChange(mSubscriptionManager.getVpsID(), vpdState);
}

void NotifyHelper::notifyVps(common::EVpdNotifications vpdNotification)
{
    if (mSubscriptionManager.getVpsID() == SubscriptionManager::voidId) {
        MLOGW(LogFuncId::VPD, LogId::NotifyHelper_vpsNotSubscribed);
        if (mVpdRestartHolder.isVpdRestarted()) {
            mVcpuActions.sendSelfRebootTrue();
        }
        return;
    }
    mVpdNotificationSender.sendNotification(mSubscriptionManager.getVpsID(), vpdNotification);
}

void NotifyHelper::notifyLog(common::EVpdState vpdState)
{
    if (mSubscriptionManager.getLogID() == SubscriptionManager::voidId) {
        MLOGW(LogFuncId::VPD, LogId::NotifyHelper_logdogNotSubscribed);
        return;
    }
    mClientActions.notifySystemStateChange(mSubscriptionManager.getLogID(), vpdState);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
