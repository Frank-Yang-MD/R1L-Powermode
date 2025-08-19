/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "InitialState.h"
#include "ITimerManager.h"
#include "NormalOperation.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForStartCpuCom.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> InitialState::sInstance = std::unique_ptr<VpdStateBase>();

void InitialState::initInstance(IVpdStateMachine* sm,
                                IWakeLockManager& lm,
                                IVpdRestartHolder& vrh,
                                ICpuCommControl& ccc,
                                IWakeUpEventDetector& wed,
                                ITimerManager& tm)
{
    sInstance.reset(new (std::nothrow) InitialState(sm, lm, vrh, ccc, wed, tm));
}

InitialState::InitialState(IVpdStateMachine* sm,
                           IWakeLockManager& lm,
                           IVpdRestartHolder& vrh,
                           ICpuCommControl& ccc,
                           IWakeUpEventDetector& wed,
                           ITimerManager& tm)
    : VpdStateBase{sm}
    , mWakeLockManager{lm}
    , mVpdRestartHolder{vrh}
    , mCpuComControl{ccc}
    , mWakeUpEventDetector{wed}
    , mTimerManager{tm}
{
}

void InitialState::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::InitialState_onTimeout);
    mWakeUpEventDetector.start();
    if (!mVpdRestartHolder.isVpdRestarted()) {
        mWakeLockManager.acquireWakeLock();
        transitionTo(WaitForStartCpuCom::getInstance());
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::InitialState_onTimeout_vpdRestarted);
        mCpuComControl.start();
        mTimerManager.changeTimerHandler(ETimerType::WDTRTimer);
        transitionTo(NormalOperation::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
