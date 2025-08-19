/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <memory>
#include <string>

#include "VehiclePowerDaemonLog.h"
#include "VpdStateMachine.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class EmptyState : protected VpdStateBase {
    static std::unique_ptr<VpdStateBase> sInstance;

public:
    EmptyState(IVpdStateMachine* sm)
        : VpdStateBase{sm}
    {
    }
    static VpdStateBase* getInstance(IVpdStateMachine* sm)
    {
        if (!sInstance) {
            sInstance.reset(new (std::nothrow) EmptyState(sm));
        }
        return sInstance.get();
    }
};
std::unique_ptr<VpdStateBase> EmptyState::sInstance = std::unique_ptr<VpdStateBase>();

VpdStateMachine::VpdStateMachine()
    : mState{nullptr}
{
    mState = EmptyState::getInstance(this);
}

VpdStateMachine::~VpdStateMachine() {}

void VpdStateMachine::transitionTo(VpdStateBase* newState)
{
    mState->onExit();
    std::string fromStateName = mState->stateName();

    if (!newState) {
        MLOGI(LogFuncId::VPD,
              LogId::VpdStateMachine_transitionTo_State_machine_is_going_to_empty_state);
    }
    mState = newState ? newState : EmptyState::getInstance(this);
    std::string toStateName = mState->stateName();

    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_transitionTo_Transition,
          "from state '" + fromStateName + "', to state '" + toStateName + "'");

    mState->onEnter();
}

void VpdStateMachine::onSubscribeNative(int id)
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onSubscribeNative_id_state, id,
          mState->stateName());

    mState->onSubscribeNative(id);
}

void VpdStateMachine::onSubscribeVps(int id)
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onSubscribeVps_id_state, id, mState->stateName());

    mState->onSubscribeVps(id);
}

void VpdStateMachine::onStartCompleteCpuCom()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStartCompleteCpuCom_state, mState->stateName());

    mState->onStartCompleteCpuCom();
}

void VpdStateMachine::onPowerStateChange(PowerState powerState)
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onPowerStateChange_state, mState->stateName());

    mState->onPowerStateChange(powerState);
}

void VpdStateMachine::onStartCompleteVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStartCompleteVps_state, mState->stateName());

    mState->onStartCompleteVps();
}

void VpdStateMachine::onStartFailedVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStartFailedVps_state, mState->stateName());

    mState->onStartFailedVps();
}

void VpdStateMachine::onStopCompleteNative(int id)
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStopCompleteNative_id_state, id,
          mState->stateName());

    mState->onStopCompleteNative(id);
}

void VpdStateMachine::onStopCompleteLog()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStopCompleteLog_state, mState->stateName());

    mState->onStopCompleteLog();
}

void VpdStateMachine::onTimeout() { mState->onTimeout(); }

void VpdStateMachine::onAppStopCompleteVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onAppStopCompleteVps_state, mState->stateName());

    mState->onAppStopCompleteVps();
}

void VpdStateMachine::onFwStopCompleteVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onFwStopCompleteVps_state, mState->stateName());

    mState->onFwStopCompleteVps();
}

void VpdStateMachine::onStopFailedVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onStopFailedVps_state, mState->stateName());

    mState->onStopFailedVps();
}

void VpdStateMachine::onFwResumeComplete()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onFwResumeComplete_state, mState->stateName());
    mState->onFwResumeComplete();
}

void VpdStateMachine::onFwRestartComplete()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onFwRestartComplete_state, mState->stateName());
    mState->onFwRestartComplete();
}

void VpdStateMachine::onAppResumeComplete()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onAppResumeComplete_state, mState->stateName());
    mState->onAppResumeComplete();
}

void VpdStateMachine::onAppRestartComplete()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onAppRestartComplete_state, mState->stateName());
    mState->onAppRestartComplete();
}

void VpdStateMachine::onVpsUnmountComplete()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onVpsUnmountComplete_state, mState->stateName());
    mState->onVpsUnmountComplete();
}

void VpdStateMachine::onShutdownProcessingStartSent()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onShutdownProcessingStartSent_state,
          mState->stateName());

    mState->onShutdownProcessingStartSent();
}

void VpdStateMachine::onResumeProcessingStartCompleteSentSuccess()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onResumeProcessingStartCompleteSentSuccess_state,
          mState->stateName());

    mState->onResumeProcessingStartCompleteSentSuccess();
}

void VpdStateMachine::onResumeProcessingStartCompleteSentFailure()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onResumeProcessingStartCompleteSentFailure_state,
          mState->stateName());

    mState->onResumeProcessingStartCompleteSentFailure();
}

void VpdStateMachine::onWakeUp()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onWakeUp_state, mState->stateName());

    mState->onWakeUp();
}

void VpdStateMachine::onDisconnectVps()
{
    MLOGI(LogFuncId::VPD, LogId::VpdStateMachine_onDisconnectVps_state, mState->stateName());

    mState->onDisconnectVps();
}

IVpdStateMachineContext& VpdStateMachine::context() { return mContext; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
