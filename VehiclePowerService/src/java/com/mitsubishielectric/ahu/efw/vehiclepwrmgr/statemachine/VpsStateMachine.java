/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_ENTER_TO;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_EXIT_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_FROM_TO;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_RESTART_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_RESUME_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_SERVICES_RESTART_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_SERVICES_RESUME_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_SERVICES_STOPPED_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_START_COLDBOOT_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_STOPPED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_APP_STOP_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_COMPLETION_TIMEOUT_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_RESTART_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_RESUME_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_SERVICES_RESTART_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_SERVICES_RESUME_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_SERVICES_STOP_COMPLETE_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_FW_STOP_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_LATE_APP_STARTED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_LATE_EFW_SRV_STARTED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_NORMAL_APP_STARTED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_NORMAL_EFW_SRV_STARTED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_RECEIVED_ON_SRV_STOPPED_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_VPS_SM_TRANS_TO_NULL_PTR_PASS;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IAppPowerStateHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStartHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStopHandler;

public class VpsStateMachine
        implements IAppPowerStateHandler,
                IStartHandler,
                IStopHandler,
                ICompletionHandler,
                IVpsStateMachine {
    private IVpsState mState;

    public VpsStateMachine() {
        mState = new DefaultState(this);
    }

    @Override
    public void onAppRestart() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_RESTART_EVT);
        mState.onAppRestart();
    }

    @Override
    public void onAppResume() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_RESUME_EVT);
        mState.onAppResume();
    }

    @Override
    public void onAppStop() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_STOP_EVT);
        mState.onAppStop();
    }

    @Override
    public void onFwRestart() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_RESTART_EVT);
        mState.onFwRestart();
    }

    @Override
    public void onFwResume() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_RESUME_EVT);
        mState.onFwResume();
    }

    @Override
    public void onFwStop() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_STOP_EVT);
        mState.onFwStop();
    }

    @Override
    public void onAppStartColdboot() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_START_COLDBOOT_EVT);
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_RECEIVED_ON_APP_START_COLDBOOT_EVT);
        mState.onAppStartColdboot();
    }

    @Override
    public void onNormalAppServicesStarted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_NORMAL_APP_STARTED_EVT);
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_RECEIVED_ON_NORMAL_APP_STARTED_EVT);
        mState.onNormalAppServicesStarted();
    }

    @Override
    public void onLateAppServicesStarted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_LATE_APP_STARTED_EVT);
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_RECEIVED_ON_LATE_APP_STARTED_EVT);
        mState.onLateAppServicesStarted();
    }

    @Override
    public void onNormalEfwServicesStarted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_NORMAL_EFW_SRV_STARTED_EVT);
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_RECEIVED_ON_NORMAL_EFW_SRV_STARTED_EVT);
        mState.onNormalEfwServicesStarted();
    }

    @Override
    public void onLateEfwServicesStarted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_LATE_EFW_SRV_STARTED_EVT);
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_RECEIVED_ON_LATE_EFW_SRV_STARTED_EVT);
        mState.onLateEfwServicesStarted();
    }

    @Override
    public void onAppsStopped() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_STOPPED_EVT);
        mState.onAppServicesStopComplete();
    }

    @Override
    public void onServicesStopped() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_SRV_STOPPED_EVT);
        mState.onEfwServicesStopComplete();
    }

    @Override
    public void transitionTo(IVpsState state) {
        if (state == null) {
            logWarning(SM_VPS_SM_TRANS_TO_NULL_PTR_PASS);
            return;
        }

        MLog.i(VPS_FUNCTION_ID, SM_FROM_TO, mState.stateName() + " to " + state.stateName());

        MLog.i(VPS_FUNCTION_ID, SM_EXIT_FROM, mState.stateName());

        mState.onExit();
        mState = state;
        MLog.i(VPS_FUNCTION_ID, SM_ENTER_TO, mState.stateName());
        mState.onEnter();
    }

    @Override
    public void onAppServicesStopCompleted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_SERVICES_STOPPED_COMPLETE_EVT);
        mState.onAppServicesStopComplete();
    }

    @Override
    public void onAppServicesRestartComplete() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_SERVICES_RESTART_COMPLETE_EVT);
        mState.onAppServicesRestartComplete();
    }

    @Override
    public void onAppServicesResumeComplete() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_APP_SERVICES_RESUME_COMPLETE_EVT);
        mState.onAppServicesResumeComplete();
    }

    @Override
    public void onEfwRestartCompleted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_SERVICES_RESTART_COMPLETE_EVT);
        mState.onEfwServicesRestartComplete();
    }

    @Override
    public void onEfwResumeCompleted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_SERVICES_RESUME_COMPLETE_EVT);
        mState.onEfwServicesResumeComplete();
    }

    @Override
    public void onEfwServicesStopCompleted() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_FW_SERVICES_STOP_COMPLETE_EVT);
        mState.onEfwServicesStopComplete();
    }

    @Override
    public void onCompletionTimeout() {
        MLog.i(VPS_FUNCTION_ID, SM_RECEIVED_ON_COMPLETION_TIMEOUT_EVT);
        mState.onCompletionTimeout();
    }

    private static final class DefaultState extends VpsAbstractState {
        DefaultState(IVpsStateMachine stateMachine) {
            super(stateMachine);
        }
    }
}
