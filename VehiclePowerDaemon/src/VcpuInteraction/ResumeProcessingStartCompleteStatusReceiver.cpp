/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeProcessingStartCompleteStatusReceiver.h"
#include "EVpdEvent.h"
#include "IEventReceiver.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

static constexpr int INTERNAL_CLIENT_ID = 0;

ResumeProcessingStartCompleteStatusReceiver::ResumeProcessingStartCompleteStatusReceiver(
    IEventReceiver& eventReceiver)
    : mEventReceiver{eventReceiver}
{
}

void ResumeProcessingStartCompleteStatusReceiver::onDeliveryStatus(bool delivered)
{
    if (delivered) {
        mEventReceiver.onEventReceive(INTERNAL_CLIENT_ID,
                                      common::EVpdEvent::ResumeProcessingStartCompleteSentSuccess);
    }
    else {
        MLOGE(LogFuncId::VPD, LogId::ShutdownProcessingStartStatusReceiver_delivered_false);
        mEventReceiver.onEventReceive(INTERNAL_CLIENT_ID,
                                      common::EVpdEvent::ResumeProcessingStartCompleteSentFailure);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
