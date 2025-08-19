/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "BootDaemonClient.h"
#include "LibVehiclePwrLog.h"
#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

BootDaemonClient::BootDaemonClient(std::unique_ptr<IClient> eventSender)
    : NativeClient(std::move(eventSender))
{
}

BootDaemonClient::~BootDaemonClient() { unsubscribe(); }

void BootDaemonClient::subscribeBootdHmiClient(IBootDaemonCallback* callback)
{
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_subscribeClientPid, getpid());
    mClient->setBootDaemonCallback(callback);
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_subscribeClientPid_done, getpid());
}

void BootDaemonClient::unsubscribe()
{
    mClient->setBootDaemonCallback(nullptr);
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_unsubscribeClientPid, getpid());
    mClient->sendVpdEvent(common::EVpdEvent::UnsubscribeBootDaemon);
}

void BootDaemonClient::onConnect()
{
    mClient->sendVpdEvent(common::EVpdEvent::SubscribeBootDaemon);
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_onConnect_SubscribeBootDaemon_event_sent,
          getpid());
}

void BootDaemonClient::onDisconnect() {}

void BootDaemonClient::handOffResponsibility()
{
    mClient->sendVpdEvent(common::EVpdEvent::handOffResponsibility);
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_handOffResponsibility);
}

void BootDaemonClient::handOffResponsibilityStartupAnimationDisplayed()
{
    mClient->sendVpdEvent(common::EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    MLOGI(LogFuncId::VPL, LogId::BootDaemonClient_handOffResponsibilityStartupAnimationDisplayed);
}

void BootDaemonClient::handOffResponsibilityStartupAnimationAndDiDisplayed()
{
    mClient->sendVpdEvent(common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    MLOGI(LogFuncId::VPL,
          LogId::BootDaemonClient_handOffResponsibilityStartupAnimationAndDiDisplayed);
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
