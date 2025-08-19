/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForVpsStartComplete.h"

#include "IAndroidInit.h"
#include "NormalOperation.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForAppStopVps.h"

#include <string>

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

namespace {
const std::string MELCO_SERVICES_START_COMPLETE_PROP_NAME{"vendor.melco.boot_completed"};
const std::string MELCO_SERVICES_START_COMPLETE_PROP_VALUE{"1"};
}  // namespace

std::unique_ptr<VpdStateBase> WaitForVpsStartComplete::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForVpsStartComplete::initInstance(IVpdStateMachine* sm,
                                           INotifyHelper& pNotifyHelper,
                                           IPowerStateHolder& powerStateHolder,
                                           const IAndroidInit& androidInit,
                                           IVcpuActions& va)
{
    sInstance.reset(
        new WaitForVpsStartComplete(sm, pNotifyHelper, powerStateHolder, androidInit, va));
}

WaitForVpsStartComplete::WaitForVpsStartComplete(IVpdStateMachine* sm,
                                                 INotifyHelper& pNotifyHelper,
                                                 IPowerStateHolder& powerStateHolder,
                                                 const IAndroidInit& androidInit,
                                                 IVcpuActions& va)
    : VpdStateBase{sm}
    , mNotifyHelper{pNotifyHelper}
    , mPowerStateHolder{powerStateHolder}
    , mAndroidInit{androidInit}
    , mVcpuActions{va}
{
}

void WaitForVpsStartComplete::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForVpsStartComplete_onEnter);

    if (mStateMachine->context().isVpsStartCompleted()) {
        onStartCompleteVps();
    } else {
        mNotifyHelper.notifyVps(EVpdState::APP_START_COLDBOOT);
    }
}

void WaitForVpsStartComplete::onExit() {
    mStateMachine->context().setVpsDisconnected(false);
    mStateMachine->context().setVpsStartCompleted(false);
}

void WaitForVpsStartComplete::onStartCompleteVps()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForVpsStartComplete_onStartCompleteVps);

    if (mPowerStateHolder.isShutdownState()) {
        mAndroidInit.propertySet(MELCO_SERVICES_START_COMPLETE_PROP_NAME,
                                 MELCO_SERVICES_START_COMPLETE_PROP_VALUE);
        transitionTo(WaitForAppStopVps::getInstance());
    }
    else {
        mVcpuActions.sendMainStartComplete();
        mAndroidInit.propertySet(MELCO_SERVICES_START_COMPLETE_PROP_NAME,
                                 MELCO_SERVICES_START_COMPLETE_PROP_VALUE);
        transitionTo(NormalOperation::getInstance());
    }
}

void WaitForVpsStartComplete::onStartFailedVps()
{
    MLOGW(LogFuncId::VPD, LogId::WaitForVpsStartComplete_onStartFailedVps);
    mVcpuActions.sendSelfRebootTrue();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
