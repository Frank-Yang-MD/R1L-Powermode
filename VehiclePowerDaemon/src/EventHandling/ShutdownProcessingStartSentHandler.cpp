/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ShutdownProcessingStartSentHandler.h"

#include "IShutdownProcessingStartSent.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ShutdownProcessingStartSentHandler::ShutdownProcessingStartSentHandler(
    IShutdownProcessingStartSent& shutdownProcessingStartSent)
    : mShutdownProcessingStartSent{shutdownProcessingStartSent}
{
}

void ShutdownProcessingStartSentHandler::Process(int, common::EVpdEvent event)
{
    if (common::EVpdEvent::ShutdownProcessingStartSent == event) {
        mShutdownProcessingStartSent.onShutdownProcessingStartSent();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::ShutdownProcessingStartSentHandler_Process_Wrong_event_index,
              event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
