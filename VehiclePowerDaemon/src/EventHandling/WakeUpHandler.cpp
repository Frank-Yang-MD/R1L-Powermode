/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUpHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

WakeUpHandler::WakeUpHandler(IWakeUpNotification& wakeUpNotification)
    : mWakeUpNotification{wakeUpNotification}
{
}

void WakeUpHandler::Process(int, common::EVpdEvent) { mWakeUpNotification.onWakeUp(); }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
