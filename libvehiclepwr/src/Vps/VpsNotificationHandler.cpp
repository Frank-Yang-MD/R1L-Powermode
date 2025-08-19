/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsNotificationHandler.h"
#include "LibVehiclePwrLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

bool VpsNotificationHandler::onVpdNotification(common::EVpdNotifications vpdNotification)
{
    const std::lock_guard<std::mutex> lock{mMutex};

    bool result = false;

    if (mVpsCallback != nullptr) {
        switch (vpdNotification) {
        case common::EVpdNotifications::FwStop:
            mVpsCallback->onFwStop();
            break;
        case common::EVpdNotifications::FwResume:
            mVpsCallback->onFwResume();
            break;
        case common::EVpdNotifications::FwRestart:
            mVpsCallback->onFwRestart();
            break;
        case common::EVpdNotifications::TimeOutError:
            mVpsCallback->onTimeOutError();
            break;
        case common::EVpdNotifications::RequestUnmount:
            mVpsCallback->requestUnmount();
            break;
        case common::EVpdNotifications::RequestMount:
            mVpsCallback->requestMount();
            break;
        case common::EVpdNotifications::HandOffResponsibility:
            MLOGI(LogFuncId::VPL, LogId::VpsNotificationHandler_HandOff_Called,
                  "HandOffResponsibility");
            mVpsCallback->handOffResponsibility();
            break;
        case common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed:
            MLOGI(LogFuncId::VPL, LogId::VpsNotificationHandler_HandOff_Called,
                  "HandOffResponsibilityStartupAnimationAndDiDisplayed");
            mVpsCallback->handOffResponsibilityStartupAnimationAndDiDisplayed();
            break;
        case common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed:
            MLOGI(LogFuncId::VPL, LogId::VpsNotificationHandler_HandOff_Called,
                  "HandOffResponsibilityStartupAnimationDisplayed");
            mVpsCallback->handOffResponsibilityStartupAnimationDisplayed();
            break;
        default:
            break;
        }
        result = true;
    }
    else {
        MLOGW(LogFuncId::VPL, LogId::VpsNotificationHandler_onVpdNotification_callback_is_null);
    }

    return result;
}

void VpsNotificationHandler::setVpsCallback(IVpsCallback* vpsCallback)
{
    const std::lock_guard<std::mutex> lock{mMutex};
    mVpsCallback = vpsCallback;
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
