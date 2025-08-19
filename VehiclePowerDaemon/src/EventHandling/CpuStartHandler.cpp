/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "CpuStartHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

CpuStartHandler::CpuStartHandler(IStartCompleteCpuCom& inIStartCompleteCpuCom)
    : mIStartCompleteCpuCom(inIStartCompleteCpuCom)
{
}

void CpuStartHandler::Process(int, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::CpuComStarted) {
        mIStartCompleteCpuCom.onStartCompleteCpuCom();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::CpuStartHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
