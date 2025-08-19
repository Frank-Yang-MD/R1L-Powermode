/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StateSender.h"
#include "EVpdNotifications.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;
using common::EVpdState;

bool convertEVpdState2EVpdNotification(EVpdState vpdState, EVpdNotifications& vpdNotification)
{
    bool result{true};

    switch (vpdState) {
    case EVpdState::APP_START_NA:
        vpdNotification = EVpdNotifications::AppStartNa;
        break;

    case EVpdState::APP_START_COLDBOOT:
        vpdNotification = EVpdNotifications::AppStartColdboot;
        break;

    case EVpdState::APP_STOP:
        vpdNotification = EVpdNotifications::AppStop;
        break;

    case EVpdState::APP_RESUME:
        vpdNotification = EVpdNotifications::AppResume;
        break;

    case EVpdState::APP_RESTART:
        vpdNotification = EVpdNotifications::AppRestart;
        break;

    default:
        result = false;
    }
    return result;
}

StateSender::StateSender(IVpdNotificationSender& vpdNotificationSender)
    : mVpdNotificationSender{vpdNotificationSender}
{
}

bool StateSender::sendState(int socketId, EVpdState state)
{
    MLOGI(LogFuncId::VPD, LogId::StateSender_sendState_socket_id_state, socketId,
          static_cast<int>(state));
    EVpdNotifications vpdNotification;
    bool convertResult{convertEVpdState2EVpdNotification(state, vpdNotification)};

    if (convertResult) {
        mVpdNotificationSender.sendNotification(socketId, vpdNotification);
    }
    else {
        MLOGI(LogFuncId::VPD,
              LogId::StateSender_sendState_incoming_VPD_state_is_undefined_VPD_state,
              static_cast<int>(state));
    }

    return convertResult;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
