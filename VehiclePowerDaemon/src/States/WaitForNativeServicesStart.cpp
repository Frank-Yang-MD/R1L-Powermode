/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForNativeServicesStart.h"
#include "WaitForVpsStart.h"

#define LOG_TAG "VPD.WaitNativeAppStart"
#include <utils/Log.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

std::unique_ptr<VpdStateBase> WaitForNativeServicesStart::sInstance =
    std::unique_ptr<VpdStateBase>();

void WaitForNativeServicesStart::initInstance(IVpdStateMachine* sm,
                                              IVcpuActions& cpuComActions,
                                              INativeRunner& nativeServiceRunner,
                                              ITimer& checkStartNativeTimer,
                                              ITimerManager& timerManager,
                                              INotifyHelper& notifyHelper)
{
    sInstance.reset(new (std::nothrow) WaitForNativeServicesStart(
        sm, cpuComActions, nativeServiceRunner, checkStartNativeTimer, timerManager, notifyHelper));
}

WaitForNativeServicesStart::WaitForNativeServicesStart(IVpdStateMachine* sm,
                                                       IVcpuActions& cpuComActions,
                                                       INativeRunner& nativeServiceRunner,
                                                       ITimer& checkStartNativeTimer,
                                                       ITimerManager& timerManager,
                                                       INotifyHelper& notifyHelper)
    : VpdStateBase{sm}
    , mCpuComActions{cpuComActions}
    , mNativeServiceRunner{nativeServiceRunner}
    , mCheckStartNativeTimer{checkStartNativeTimer}
    , mTimerManager{timerManager}
    , mNotifyHelper{notifyHelper}
{
}

void WaitForNativeServicesStart::onEnter()
{
    mNativeServiceRunner.run();
    mCheckStartNativeTimer.start();
}

void WaitForNativeServicesStart::onExit()
{
    mCpuComActions.sendStartComplete();
    mTimerManager.changeTimerHandler(ETimerType::WDTRTimer);
    mNotifyHelper.notifyAllNative(EVpdState::APP_START_COLDBOOT);
}

void WaitForNativeServicesStart::onTimeout()
{
    if (mCheckStartNativeTimer.isExpired()) {
        if (mNativeServiceRunner.isDone()) {
            transitionTo(WaitForVpsStart::getInstance());
        }
        else {
            mCheckStartNativeTimer.start();  // set to wait again
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
