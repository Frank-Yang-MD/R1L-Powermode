/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForVpsStart.h"
#include "IVpdStateMachine.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForVpsStartComplete.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForVpsStart::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForVpsStart::initInstance(IVpdStateMachine* sm,
                                   ISubscriptionManager& subscrMgr,
                                   IStateHolder& sh)
{
    sInstance.reset(new WaitForVpsStart(sm, subscrMgr, sh));
}

WaitForVpsStart::WaitForVpsStart(IVpdStateMachine* sm,
                                 ISubscriptionManager& subscrMgr,
                                 IStateHolder& sh)
    : VpdStateBase{sm}
    , mSubscrMgr{subscrMgr}
    , mStateHolder{sh}
{
}

void WaitForVpsStart::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForVpsStart_onEnter);

    mStateHolder.setVpdState(EVpdState::APP_START_COLDBOOT);

    if (-1 != mSubscrMgr.getVpsID()) {
        transitionTo(WaitForVpsStartComplete::getInstance());
    }
}

void WaitForVpsStart::onSubscribeVps(int)
{
    MLOGI(LogFuncId::VPD, LogId::WaitForVpsStart_onSubscribeVps);

    transitionTo(WaitForVpsStartComplete::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
