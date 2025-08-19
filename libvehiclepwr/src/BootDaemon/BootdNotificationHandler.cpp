/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootdNotificationHandler.h"

#include "LibVehiclePwrLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

bool BootdNotificationHandler::onVpdNotification(common::EVpdNotifications vpdNotification)
{
    const std::lock_guard<std::mutex> lock{mMutex};

    bool result = false;

    if (mBootdCallback != nullptr) {
        switch (vpdNotification) {
        case common::EVpdNotifications::HmiEventVpsReady:
            MLOGI(LogFuncId::VPL, LogId::BootdNotificationHandler_onVpdNotification_Event_Sent,
                  "HmiEventVpsReady");
            mBootdCallback->onHMIEvent(common::EHmiEvent::HmiEventVpsReady);
            break;
        case common::EVpdNotifications::EndEarlyHmi:
            MLOGI(LogFuncId::VPL, LogId::BootdNotificationHandler_onVpdNotification_Event_Sent,
                  "EndEarlyHmi");
            mBootdCallback->onHMIEvent(common::EHmiEvent::EndEarlyHmi);
            break;
        case common::EVpdNotifications::ForcedCompletionEarlyServices:
            MLOGI(LogFuncId::VPL, LogId::BootdNotificationHandler_onVpdNotification_Event_Sent,
                  "ForcedCompletionEarlyServices");
            mBootdCallback->onHMIEvent(common::EHmiEvent::ForcedCompletionEarlyServices);
            break;
        default:
            break;
        }
        result = true;
    }
    else {
        MLOGW(LogFuncId::VPL, LogId::BootdNotificationHandler_callback_is_null);
    }

    return result;
}

void BootdNotificationHandler::setBootDaemonCallback(IBootDaemonCallback* bootdCallback)
{
    const std::lock_guard<std::mutex> lock{mMutex};
    mBootdCallback = bootdCallback;
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
