/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeClientNotificationHandler.h"
#include "LibVehiclePwrLog.h"

#include <unordered_map>

#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

static const std::unordered_map<common::EVpdNotifications, common::EVpdState>
    NOTIFICATION_TO_STATE = {
        {common::EVpdNotifications::AppResume, common::EVpdState::APP_RESUME},
        {common::EVpdNotifications::AppStartColdboot, common::EVpdState::APP_START_COLDBOOT},
        {common::EVpdNotifications::AppStartNa, common::EVpdState::APP_START_NA},
        {common::EVpdNotifications::AppStop, common::EVpdState::APP_STOP},
        {common::EVpdNotifications::AppRestart, common::EVpdState::APP_RESTART},
};

bool NativeClientNotificationHandler::onVpdNotification(common::EVpdNotifications vpdNotification)
{
    const std::lock_guard<std::mutex> lock{mMutex};
    if (NOTIFICATION_TO_STATE.count(vpdNotification)) {
        mVpdState = NOTIFICATION_TO_STATE.at(vpdNotification);
        if (mNativeCallback != nullptr) {
            mNativeCallback->onSystemStateChange(mVpdState);
        }
        else {
            MLOGW(LogFuncId::VPL,
                  LogId::NativeClientNotificationHandler_onVpdNotification_callback_is_null,
                  getpid());
        }
    }
    else {
        MLOGW(LogFuncId::VPL,
              LogId::NativeClientNotificationHandler_onVpdNotification_invalidNotification,
              static_cast<int>(vpdNotification));
    }

    return mNativeCallback != nullptr;
}

common::EVpdState NativeClientNotificationHandler::getVpdState() { return mVpdState; }

void NativeClientNotificationHandler::setNativeCallback(INativeCallback* nativeCallback)
{
    const std::lock_guard<std::mutex> lock{mMutex};
    mNativeCallback = nativeCallback;
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
