/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForUnmount.h"
#include "INotifyHelper.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForModulesLoadBeforeUnload.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForUnmount::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForUnmount::initInstance(IVpdStateMachine* sm,
                                  INotifyHelper& notifyHelper,
                                  ITimer& waitForUnmountTimer,
                                  IVcpuActions& cpuComActions)
{
    sInstance.reset(new WaitForUnmount(sm, notifyHelper, waitForUnmountTimer, cpuComActions));
}

void WaitForUnmount::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForUnmount_onEnter);

    mWaitForUnmountTimer.start();

    mNotifyHelper.notifyVps(EVpdNotifications::RequestUnmount);
}

void WaitForUnmount::onVpsUnmountComplete()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForUnmount_onVpsUnmountComplete);
    transitionTo(WaitForModulesLoadBeforeUnload::getInstance());
}

void WaitForUnmount::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForUnmount_onTimeout);

    if (mWaitForUnmountTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForUnmount_timerIsExpired);
        mNotifyHelper.notifyVps(EVpdNotifications::TimeOutError);
        mCpuActions.sendSelfRebootTrue();
        transitionTo(WaitForModulesLoadBeforeUnload::getInstance());
    }
}

WaitForUnmount::WaitForUnmount(IVpdStateMachine* sm,
                               INotifyHelper& notifyHelper,
                               ITimer& waitForUnmountTimer,
                               IVcpuActions& cpuComActions)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
    , mWaitForUnmountTimer{waitForUnmountTimer}
    , mCpuActions{cpuComActions}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com