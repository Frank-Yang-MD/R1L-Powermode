/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeClient.h"
#include "LibVehiclePwrLog.h"
#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

NativeClient::NativeClient(std::unique_ptr<IClient> client)
    : mClient{std::move(client)}
{
    mClient->setInternalCallback(this);
}

NativeClient::~NativeClient() { unsubscribe(); }

common::EVpdState NativeClient::GetVPDState() { return mClient->getVpdState(); }

void NativeClient::onConnect()
{
    MLOGI(LogFuncId::VPL, LogId::NativeClient_onConnect_func, getpid());
    mClient->sendVpdEvent(common::EVpdEvent::SubscribeNative);
    MLOGI(LogFuncId::VPL, LogId::NativeClient_onConnect_SubscribeNative_event_sent, getpid());
}

void NativeClient::onDisconnect() {}

void NativeClient::stopProcessingComplete()
{
    mClient->sendVpdEvent(common::EVpdEvent::StopCompleteNative);
    MLOGI(LogFuncId::VPL, LogId::NativeClient_stopProcessingComplete_StopCompleteNative_event_sent,
          getpid());
}

void NativeClient::subscribe(INativeCallback* callback)
{
    MLOGI(LogFuncId::VPL, LogId::NativeClient_subscribeClientPid, getpid());
    mClient->setNativeCallback(callback);
    MLOGI(LogFuncId::VPL, LogId::NativeClient_subscribeClientPid_done, getpid());
}

void NativeClient::unsubscribe()
{
    MLOGI(LogFuncId::VPL, LogId::NativeClient_unsubscribeClientPid, getpid());
    mClient->setNativeCallback(nullptr);
    mClient->sendVpdEvent(common::EVpdEvent::UnsubscribeNative);
}

void NativeClient::startConnection() { mClient->startConnection(); }

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
