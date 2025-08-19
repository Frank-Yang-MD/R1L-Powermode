/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ShutdownProcessingStartStatusReceiver.h"
#include "EVpdEvent.h"
#include "IEventReceiver.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

static constexpr int INTERNAL_CLIENT_ID = 0;

ShutdownProcessingStartStatusReceiver::ShutdownProcessingStartStatusReceiver(
    IEventReceiver& eventReceiver)
    : mEventReceiver{eventReceiver}
{
}

void ShutdownProcessingStartStatusReceiver::onDeliveryStatus(bool delivered)
{
    if (delivered) {
        mEventReceiver.onEventReceive(INTERNAL_CLIENT_ID,
                                      common::EVpdEvent::ShutdownProcessingStartSent);
    }
    else {
        MLOGE(LogFuncId::VPD, LogId::ShutdownProcessingStartStatusReceiver_delivered_false);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
