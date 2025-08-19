/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogdogClient.h"
#include "LibVehiclePwrLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

LogdogClient::LogdogClient(std::unique_ptr<IClient> pClient)
    : NativeClient(std::move(pClient))
{
}

LogdogClient::~LogdogClient() { unsubscribe(); }

void LogdogClient::stopProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::StopCompleteLogdog);
    MLOGI(LogFuncId::VPL, LogId::LogdogClient_stopProcessingComplete_StopCompleteLogdog_event_sent);
}

void LogdogClient::subscribe(INativeCallback* callback)
{
    NativeClient::subscribe(callback);
    MLOGI(LogFuncId::VPL, LogId::LogdogClient_subscribe_SubscribeLogdog_event_sent);
}

void LogdogClient::unsubscribe()
{
    NativeClient::unsubscribe();
    mClient->sendVpdEvent(common::EVpdEvent::UnsubscribeLogdog);
    MLOGI(LogFuncId::VPL, LogId::LogdogClient_unsubscribe_UnsubscribeLogdog_event_sent);
}

void LogdogClient::onConnect()
{
    NativeClient::onConnect();
    mClient->sendVpdEvent(common::EVpdEvent::SubscribeLogdog);
    MLOGI(LogFuncId::VPL, LogId::LogdogClient_onConnect_SubscribeLogdog_event_sent);
}

void LogdogClient::onDisconnect() {}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
