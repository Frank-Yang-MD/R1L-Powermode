/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsClient.h"
#include "LibVehiclePwrLog.h"

#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

VpsClient::VpsClient(std::unique_ptr<IClient> client)
    : NativeClient(std::move(client))
{
}

VpsClient::~VpsClient() { unsubscribe(); }

void VpsClient::subscribeVps(IVpsCallback* vpsCallback)
{
    MLOGI(LogFuncId::VPL, LogId::VpsClient_subscribeVps_SubscribeVps_event_sent, getpid());
    mClient->setVpsCallback(vpsCallback);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_subscribeVps_SubscribeVps_event_sent_done, getpid());
}

void VpsClient::startProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::VpsStarted);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_startProcessingComplete_VpsStarted_event_sent);
}

void VpsClient::startProcessingFailed()
{
    mClient->sendVpdEvent(common::EVpdEvent::StartFailedVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_startProcessingFailed_StartFailedVps_event_sent);
}

void VpsClient::appStopProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::AppStopCompleteVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_appStopProcessingComplete_AppStopCompleteVps_event_sent);
}

void VpsClient::fwRestartProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::FwRestartCompleteVps);
    MLOGI(LogFuncId::VPL,
          LogId::VpsClient_fwRestartProcessingComplete_FwRestartCompleteVps_event_sent);
}

void VpsClient::fwResumeProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::FwResumeCompleteVps);
    MLOGI(LogFuncId::VPL,
          LogId::VpsClient_fwResumeProcessingComplete_FwResumeCompleteVps_event_sent);
}

void VpsClient::appRestartProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::AppRestartCompleteVps);
    MLOGI(LogFuncId::VPL,
          LogId::VpsClient_appRestartProcessingComplete_AppRestartCompleteVps_event_sent);
}

void VpsClient::appResumeProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::AppResumeCompleteVps);
    MLOGI(LogFuncId::VPL,
          LogId::VpsClient_appResumeProcessingComplete_AppResumeCompleteVps_event_sent);
}

void VpsClient::fwStopProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::FwStopCompleteVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_fwStopProcessingComplete_FwStopCompleteVps_event_sent);
}

void VpsClient::stopProcessingFailed()
{
    mClient->sendVpdEvent(common::EVpdEvent::StopFailedVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_stopProcessingFailed_StopFailedVps_event_sent);
}

void VpsClient::unmountComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::UnmountComplete);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_unmountComplete_UnmountComplete_event_sent);
}

void VpsClient::endEarlyHmi()
{
    mClient->sendVpdEvent(common::EVpdEvent::EndEarlyHmi);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_endEarlyHmi_EndEarlyHmi_event_sent);
}

void VpsClient::forcedCompletionEarlyServices()
{
    mClient->sendVpdEvent(common::EVpdEvent::ForcedCompletionEarlyServices);
    MLOGI(LogFuncId::VPL,
          LogId::VpsClient_forcedCompletionEarlyServices_ForcedCompletionEarlyServices_event_sent);
}

void VpsClient::unsubscribe()
{
    mClient->setVpsCallback(nullptr);
    mClient->sendVpdEvent(common::EVpdEvent::UnsubscribeVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_unsubscribe_UnsubscribeVps_event_sent);
}

void VpsClient::onConnect()
{
    NativeClient::onConnect();
    mClient->sendVpdEvent(common::EVpdEvent::SubscribeVps);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_onConnect_SubscribeVps_event_sent);
}

void VpsClient::onDisconnect() {}

void VpsClient::vpsReady()
{
    mClient->sendVpdEvent(common::EVpdEvent::VpsReady);
    MLOGI(LogFuncId::VPL, LogId::VpsClient_vpsReady_VpsReady_event_sent);
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
