/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NOTIFYHELPER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NOTIFYHELPER_H

#include "IClientActions.h"
#include "INotifyHelper.h"
#include "ISubscriptionManager.h"
#include "IVcpuActions.h"
#include "IVpdNotificationSender.h"
#include "IVpdRestartHolder.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Helper class
 */
class NotifyHelper : public INotifyHelper {
public:
    NotifyHelper(IClientActions& clientActions,
                 ISubscriptionManager& subscriptionManager,
                 IVpdNotificationSender& vpdNotificationSender,
                 IVcpuActions& vcpuActions,
                 IVpdRestartHolder& vpdRestartHolder);
    void notifyAllNative(common::EVpdState vpdState) override;
    void notifyVps(common::EVpdState vpdState) override;
    void notifyVps(common::EVpdNotifications vpdNotification) override;
    void notifyLog(common::EVpdState vpdState) override;

private:
    IClientActions& mClientActions;
    ISubscriptionManager& mSubscriptionManager;
    IVpdNotificationSender& mVpdNotificationSender;
    IVcpuActions& mVcpuActions;
    IVpdRestartHolder& mVpdRestartHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NOTIFYHELPER_H
