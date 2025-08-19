/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "HmiEventsHandler.h"
#include "EVpdNotifications.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

HmiEventsHandler::HmiEventsHandler(IVpdNotificationSender& inVpdNotificationSender,
                                   ISubscriptionManager& inSubscriptionManager)
    : mVpdNotificationSender(inVpdNotificationSender)
    , mSubscriptionManager(inSubscriptionManager)
{
}

void HmiEventsHandler::Process(int clientId, common::EVpdEvent event)
{
    if (clientId == mSubscriptionManager.getVpsID()) {
        processVpsEvent(event);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::HmiEventsHandler_Process_Wrong_event_index, event);
    }
}

void HmiEventsHandler::processVpsEvent(common::EVpdEvent event)
{
    auto bootdId = mSubscriptionManager.getBootDaemonID();
    switch (event) {
    case common::EVpdEvent::VpsReady:
        MLOGI(LogFuncId::VPD, LogId::HmiEventsHandler_processVpsEvent_sendNotification, "VpsReady");
        mVpdNotificationSender.sendNotification(bootdId,
                                                common::EVpdNotifications::HmiEventVpsReady);
        break;
    case common::EVpdEvent::EndEarlyHmi:
        MLOGI(LogFuncId::VPD, LogId::HmiEventsHandler_processVpsEvent_sendNotification,
              "EndEarlyHmi");
        mVpdNotificationSender.sendNotification(bootdId, common::EVpdNotifications::EndEarlyHmi);
        break;
    case common::EVpdEvent::ForcedCompletionEarlyServices:
        MLOGI(LogFuncId::VPD, LogId::HmiEventsHandler_processVpsEvent_sendNotification,
              "ForcedCompletionEarlyServices");
        mVpdNotificationSender.sendNotification(
            bootdId, common::EVpdNotifications::ForcedCompletionEarlyServices);
        break;
    default:
        MLOGW(LogFuncId::VPD, LogId::HmiEventsHandler_processVpsEvent_Wrong_event_index,
              static_cast<int>(event));
        break;
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com