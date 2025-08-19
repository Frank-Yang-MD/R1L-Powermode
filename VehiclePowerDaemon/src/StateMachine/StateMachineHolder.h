/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEMACHINEHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEMACHINEHOLDER_H_

#include "CpuComClient.h"
#include "IKernelModulesControl.h"
#include "ILogDaemonActions.h"
#include "IStateSender.h"
#include "ITimerManager.h"
#include "IWakeLockManager.h"
#include "IWakeUpManager.h"
#include "NotifyHelper.h"
#include "ShutdownManager.h"
#include "SubscriptionManager.h"
#include "TimeoutControl.h"
#include "Timer.h"
#include "VpdClock.h"
#include "VpdStateMachine.h"

#include "INativeRunnerHolder.h"
#include "IStateHolder.h"
#include "IVehicleConfigProvider.h"
#include "IWakeUpEventDetector.h"

#include "IPowerStateHolder.h"
#include "ITimerCreator.h"
#include "IVpdRestartHolder.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StateMachineHolder {
public:
    StateMachineHolder(INativeRunnerHolder& nativeRunnerHolder,
                       IVcpuInteraction& vcpuInteraction,
                       INotifyHelper& notifyHelper,
                       ILogDaemonActions& logDaemonActions,
                       ShutdownManager& shutdownManager,
                       IWakeLockManager& wakeLockManager,
                       IWakeUpManager& wakeUpManager,
                       ISubscriptionManager& subscrMgr,
                       IWakeUpEventDetector& wakeUpEventDetector,
                       IKernelModulesControl& kernelModulesControl,

                       IPowerStateHolder& powerStateHolder,

                       ITimer& checkStartNativeTimer,

                       ITimer& appStopVpsTimer,
                       ITimer& appStopVpsTimeout,
                       ITimer& fwStopVpsTimer,
                       ITimer& fwStopVpsTimeout,
                       ITimer& stopNativeTimer,
                       ITimer& logWaitSavingTimer,

                       ITimer& logdogCheckSavingStateTimer,
                       ITimer& logdogSaveStateTimer,
                       ITimer& logdogStopTimer,

                       ITimer& modulesLoadTimeout,
                       ITimer& modulesUnloadCheckTimer,
                       ITimer& modulesUnloadTimeout,
                       ITimer& appResumeEfwTimer,
                       ITimer& appResumeAppTimer,
                       ITimer& appRestartEfwTimer,
                       ITimer& appRestartAppTimer,

                       ITimer& unmountTimer,

                       IStateHolder& stateHolder,
                       IVpdRestartHolder& vpdRestartHolder,
                       ITimerManager& timerManager);
    ~StateMachineHolder();

    IVpdState* getVpdState();

private:
    VpdStateMachine mStateMachine;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEMACHINEHOLDER_H_
