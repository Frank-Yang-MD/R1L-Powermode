/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonEventHandler.h"
#include "EVpdNotifications.h"
#include "ISubscriptionManager.h"
#include "IVpdNotificationSender.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

BootDaemonEventHandler::BootDaemonEventHandler(IVpdNotificationSender& vpdNotificationSender,
                                               ISubscriptionManager& subscriptionManager)
    : mVpdNotificationSender{vpdNotificationSender}
    , mSubscriptionManager{subscriptionManager}
{
}

void BootDaemonEventHandler::Process(int clientId, common::EVpdEvent event)
{
    if (clientId == mSubscriptionManager.getBootDaemonID()) {
        processBootDaemonEvent(event);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::BootDaemonEventHandler_Process_Wrong_ClientId, clientId);
    }
}

void BootDaemonEventHandler::processBootDaemonEvent(common::EVpdEvent event)
{
    int vpsId = mSubscriptionManager.getVpsID();

    switch (event) {
    case common::EVpdEvent::handOffResponsibility:
        MLOGI(LogFuncId::VPD, LogId::BootDaemonEventHandler_Process, "handOffResponsibility");
        mVpdNotificationSender.sendNotification(vpsId,
                                                common::EVpdNotifications::HandOffResponsibility);
        break;
    case common::EVpdEvent::handOffResponsibilityStartupAnimationDisplayed:
        MLOGI(LogFuncId::VPD, LogId::BootDaemonEventHandler_Process,
              "handOffResponsibilityStartupAnimationDisplayed");
        mVpdNotificationSender.sendNotification(
            vpsId, common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed);
        break;
    case common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed:
        MLOGI(LogFuncId::VPD, LogId::BootDaemonEventHandler_Process,
              "handOffResponsibilityStartupAnimationAndDiDisplayed");
        mVpdNotificationSender.sendNotification(
            vpsId, common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed);
        break;
    default:
        MLOGW(LogFuncId::VPD,
              LogId::BootDaemonEventHandler_processBootDaemonEvent_Wrong_event_index,
              static_cast<int>(event));
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
