/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForMount.h"
#include "INotifyHelper.h"
#include "RestartNative.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForMount::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForMount::initInstance(IVpdStateMachine* sm, INotifyHelper& notifyHelper)
{
    sInstance.reset(new WaitForMount(sm, notifyHelper));
}

void WaitForMount::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForMount_onEnter);

    mNotifyHelper.notifyVps(common::EVpdNotifications::RequestMount);

    transitionTo(RestartNative::getInstance());
}

WaitForMount::WaitForMount(IVpdStateMachine* sm, INotifyHelper& notifyHelper)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
