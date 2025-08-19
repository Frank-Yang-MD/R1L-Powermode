/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForStartCpuCom.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForNativeServicesStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

const std::string START_PROP_NAME("vendor.cpucomdaemon");
const std::string START_PROP_VALUE("start");

std::unique_ptr<VpdStateBase> WaitForStartCpuCom::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForStartCpuCom::initInstance(IVpdStateMachine* sm,
                                      IAndroidInit& androidInit,
                                      ICpuCommControl& cpuComControl)
{
    sInstance.reset(new WaitForStartCpuCom(sm, androidInit, cpuComControl));
}

WaitForStartCpuCom::WaitForStartCpuCom(IVpdStateMachine* sm,
                                       IAndroidInit& androidInit,
                                       ICpuCommControl& cpuComControl)
    : VpdStateBase(sm)
    , mAndroidInit(androidInit)
    , mCpuComControl(cpuComControl)
{
}

WaitForStartCpuCom::~WaitForStartCpuCom() { mCpuComControl.stop(); }
void WaitForStartCpuCom::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForStartCpuCom_onEnter);

    mAndroidInit.propertySet(START_PROP_NAME, START_PROP_VALUE);
}

void WaitForStartCpuCom::onStartCompleteCpuCom()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForStartCpuCom_onStartCompleteCpuCom);

    mCpuComControl.start();
    transitionTo(WaitForNativeServicesStart::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
