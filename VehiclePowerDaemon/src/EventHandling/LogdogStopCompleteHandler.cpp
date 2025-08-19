/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogdogStopCompleteHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

LogdogStopCompleteHandler::LogdogStopCompleteHandler(IStopCompleteLog& inIStopCompleteLog)
    : mIStopCompleteLog(inIStopCompleteLog)
{
}

void LogdogStopCompleteHandler::Process(int, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::StopCompleteLogdog) {
        mIStopCompleteLog.onStopCompleteLog();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::LogdogStopCompleteHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
