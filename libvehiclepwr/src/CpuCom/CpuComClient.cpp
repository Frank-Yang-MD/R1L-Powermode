/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "CpuComClient.h"
#include "LibVehiclePwrLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

CpuComClient::CpuComClient(std::unique_ptr<IClient> eventSender)
    : mEventSender{std::move(eventSender)}
{
}

void CpuComClient::CpuComStarted()
{
    mEventSender->sendVpdEvent(common::EVpdEvent::CpuComStarted);
    MLOGI(LogFuncId::VPL,
          LogId::CpuComClient_CpuComStarted_common_EVpdEvent_CpuComStarted_event_sent);
}

void CpuComClient::startConnection() { mEventSender->startConnection(); }

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
