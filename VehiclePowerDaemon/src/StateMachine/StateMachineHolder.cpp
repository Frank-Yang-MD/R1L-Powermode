/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StateMachineHolder.h"

#include "AbnormalTimerHandler.h"
#include "InitialState.h"
#include "LogDaemonBusy.h"
#include "NormalOperation.h"
#include "PollingWakeUp.h"
#include "RestartNative.h"
#include "Resume.h"
#include "ResumeNative.h"
#include "ResumeProcessingStartComplete.h"
#include "SendShutdownProcessingStart.h"
#include "WaitBeforeSendingAppStopNativeServices.h"
#include "WaitBeforeSendingFwStopVps.h"
#include "WaitForAppRestartComplete.h"
#include "WaitForAppResumeComplete.h"
#include "WaitForAppStopVps.h"
#include "WaitForFwRestartComplete.h"
#include "WaitForFwResumeComplete.h"
#include "WaitForFwStopVps.h"
#include "WaitForLogDaemonStop.h"
#include "WaitForLogSaving.h"
#include "WaitForModulesLoadAfterSuspendCancel.h"
#include "WaitForModulesLoadBeforeUnload.h"
#include "WaitForModulesUnload.h"
#include "WaitForMount.h"
#include "WaitForNativeServicesStart.h"
#include "WaitForStartCpuCom.h"
#include "WaitForUnmount.h"
#include "WaitForVpsStart.h"
#include "WaitForVpsStartComplete.h"
#include "WaitNativeAppStopProcessingComplete.h"
#include "WakeUp.h"
#include "WatchDogTimerHandler.h"

#include <KernelModulesControl.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

StateMachineHolder::StateMachineHolder(INativeRunnerHolder& nativeRunnerHolder,
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
                                       ITimerManager& timerManager)

{
    // S0
    InitialState::initInstance(&mStateMachine, wakeLockManager, vpdRestartHolder, vcpuInteraction,
                               wakeUpEventDetector, timerManager);

    // S1
    WaitForStartCpuCom::initInstance(&mStateMachine, nativeRunnerHolder, vcpuInteraction);

    // S2
    WaitForNativeServicesStart::initInstance(&mStateMachine, vcpuInteraction, nativeRunnerHolder,
                                             checkStartNativeTimer, timerManager, notifyHelper);

    // S3
    WaitForVpsStart::initInstance(&mStateMachine, subscrMgr, stateHolder);

    // S4
    WaitForVpsStartComplete::initInstance(&mStateMachine, notifyHelper, powerStateHolder,
                                          nativeRunnerHolder, vcpuInteraction);

    // S5
    NormalOperation::initInstance(&mStateMachine, powerStateHolder, vpdRestartHolder, stateHolder);

    // S6
    WaitForAppStopVps::initInstance(&mStateMachine, notifyHelper, appStopVpsTimer, fwStopVpsTimer,
                                    vcpuInteraction, nativeRunnerHolder);

    WaitBeforeSendingFwStopVps::initInstance(&mStateMachine, fwStopVpsTimeout);

    WaitForFwStopVps::initInstance(&mStateMachine, notifyHelper, fwStopVpsTimer, vcpuInteraction);

    WaitBeforeSendingAppStopNativeServices::initInstance(&mStateMachine, appStopVpsTimeout);

    // S8
    LogDaemonBusy::initInstance(&mStateMachine, logDaemonActions, logdogCheckSavingStateTimer,
                                logdogSaveStateTimer, vcpuInteraction);

    // S7
    WaitNativeAppStopProcessingComplete::initInstance(&mStateMachine, notifyHelper, stopNativeTimer,
                                                      subscrMgr, stateHolder, vcpuInteraction);

    // S7.1
    WaitForLogSaving::initInstance(&mStateMachine, logWaitSavingTimer);

    // S9
    WaitForLogDaemonStop::initInstance(&mStateMachine, notifyHelper, logdogStopTimer,
                                       powerStateHolder, vcpuInteraction);

    WaitForModulesLoadBeforeUnload::initInstance(&mStateMachine, modulesUnloadCheckTimer,
                                                 modulesLoadTimeout, kernelModulesControl);

    // S10
    WaitForModulesUnload::initInstance(&mStateMachine, modulesUnloadCheckTimer,
                                       modulesUnloadTimeout, kernelModulesControl,
                                       powerStateHolder);

    // S13.1
    WaitForModulesLoadAfterSuspendCancel::initInstance(&mStateMachine, kernelModulesControl);

    SendShutdownProcessingStart::initInstance(&mStateMachine, vcpuInteraction, powerStateHolder,
                                              timerManager);

    // S12
    PollingWakeUp::initInstance(&mStateMachine, wakeUpEventDetector, wakeLockManager,
                                shutdownManager);

    ResumeProcessingStartComplete::initInstance(&mStateMachine, vcpuInteraction, powerStateHolder,
                                                timerManager);

    // S13
    WakeUp::initInstance(&mStateMachine, kernelModulesControl, wakeUpManager);

    // S14
    Resume::initInstance(&mStateMachine, powerStateHolder);

    // Init RestartNative state
    RestartNative::initInstance(&mStateMachine, notifyHelper, stateHolder);

    // Init WaitForFwRestartComplete state
    WaitForFwRestartComplete::initInstance(&mStateMachine, notifyHelper, appRestartEfwTimer,
                                           vcpuInteraction);

    // Init WaitForAppRestartComplete state
    WaitForAppRestartComplete::initInstance(&mStateMachine, notifyHelper, appRestartAppTimer,
                                            vcpuInteraction, powerStateHolder);

    // Init ResumeNative state
    ResumeNative::initInstance(&mStateMachine, notifyHelper, stateHolder);

    // Init WaitForFwResumeComplete state
    WaitForFwResumeComplete::initInstance(&mStateMachine, notifyHelper, appResumeEfwTimer,
                                          vcpuInteraction);

    // Init WaitForAppResumeComplete state
    WaitForAppResumeComplete::initInstance(&mStateMachine, notifyHelper, appResumeAppTimer,
                                           vcpuInteraction);

    // Init WaitForMount state
    WaitForMount::initInstance(&mStateMachine, notifyHelper);

    // Init WaitForUnmount state
    WaitForUnmount::initInstance(&mStateMachine, notifyHelper, unmountTimer, vcpuInteraction);

    mStateMachine.transitionTo(InitialState::getInstance());
}

StateMachineHolder::~StateMachineHolder()
{
    mStateMachine.transitionTo(nullptr);

    // Uninit WaitForAppResumeComplete state
    WaitForAppResumeComplete::uninitInstance();

    // Uninit WaitForFwResumeComplete state
    WaitForFwResumeComplete::uninitInstance();

    // Uninit ResumeNative state
    ResumeNative::uninitInstance();

    // Uninit WaitForAppRestartComplete state
    WaitForAppRestartComplete::uninitInstance();

    // Uninit WaitForFwRestartComplete state
    WaitForFwRestartComplete::uninitInstance();

    // Uninit RestartNative state
    RestartNative::uninitInstance();

    WaitForModulesLoadAfterSuspendCancel::uninitInstance();

    // S14
    Resume::uninitInstance();

    // S13
    WakeUp::uninitInstance();

    ResumeProcessingStartComplete::uninitInstance();

    // S12
    PollingWakeUp::uninitInstance();

    // S10
    WaitForModulesUnload::uninitInstance();

    // S9
    WaitForLogDaemonStop::uninitInstance();

    SendShutdownProcessingStart::uninitInstance();

    // S8
    LogDaemonBusy::uninitInstance();

    // S7.1
    WaitForLogSaving::uninitInstance();

    // S7
    WaitNativeAppStopProcessingComplete::uninitInstance();

    WaitBeforeSendingAppStopNativeServices::uninitInstance();

    WaitForFwStopVps::uninitInstance();

    WaitBeforeSendingFwStopVps::uninitInstance();

    // S6
    WaitForAppStopVps::uninitInstance();

    // S5
    NormalOperation::uninitInstance();

    // S4
    WaitForVpsStartComplete::uninitInstance();

    // S3
    WaitForVpsStart::uninitInstance();

    // S2
    WaitForNativeServicesStart::uninitInstance();

    // S1
    WaitForStartCpuCom::uninitInstance();

    // S0
    InitialState::uninitInstance();
}

IVpdState* StateMachineHolder::getVpdState() { return &mStateMachine; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
