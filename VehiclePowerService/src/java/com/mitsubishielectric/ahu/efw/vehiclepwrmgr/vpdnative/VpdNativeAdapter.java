/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.BNA_HMI_IS_READY;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_APP_STOP_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_END_EARLY_HMI_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_FORCED_COMPLETION_EARLY_SERVICES_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_FW_RESTART_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_FW_RESUME_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_FW_STOP_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_START_COMPLETE_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_START_FAILED_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_STOP_FAILED_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_UNMOUNT_COMPLETE_TO_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

public final class VpdNativeAdapter implements IVpdNativeAdapter {

    @Override
    public void vpdStartComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_START_COMPLETE_TO_VPD);
        MLog.i(VPS_EARLY_FUNCTION_ID, SEND_START_COMPLETE_TO_VPD);
        startProcessingCompleteNative();
    }

    public void vpdStartFailed() {
        MLog.i(VPS_FUNCTION_ID, SEND_START_FAILED_TO_VPD);
        MLog.i(VPS_EARLY_FUNCTION_ID, SEND_START_FAILED_TO_VPD);
        startProcessingFailedNative();
    }

    @Override
    public void vpdAppStopComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_APP_STOP_TO_VPD);
        appStopProcessingCompleteNative();
    }

    @Override
    public void vpdFwRestartComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_FW_RESTART_TO_VPD);
        fwRestartProcessingCompleteNative();
    }

    @Override
    public void vpdFwResumeComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_FW_RESUME_TO_VPD);
        fwResumeProcessingCompleteNative();
    }

    @Override
    public void vpdAppRestartComplete() {
        appRestartProcessingCompleteNative();
    }

    @Override
    public void vpdAppResumeComplete() {
        appResumeProcessingCompleteNative();
    }

    @Override
    public void vpdFwStopComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_FW_STOP_TO_VPD);
        fwStopProcessingCompleteNative();
    }

    @Override
    public void vpdStopFailed() {
        MLog.i(VPS_FUNCTION_ID, SEND_STOP_FAILED_TO_VPD);
        stopProcessingFailedNative();
    }

    @Override
    public void hmiIsReady() {
        MLog.i(VPS_FUNCTION_ID, BNA_HMI_IS_READY);
        hmiIsReadyNative();
    }

    @Override
    public void unmountComplete() {
        MLog.i(VPS_FUNCTION_ID, SEND_UNMOUNT_COMPLETE_TO_VPD);
        unmountCompleteNative();
    }

    @Override
    public void endEarlyHmi() {
        MLog.i(VPS_FUNCTION_ID, SEND_END_EARLY_HMI_TO_VPD);
        endEarlyHmiNative();
    }

    @Override
    public void forcedCompletionEarlyServices() {
        MLog.i(VPS_FUNCTION_ID, SEND_FORCED_COMPLETION_EARLY_SERVICES_TO_VPD);
        forcedCompletionEarlyServicesNative();
    }

    private static native void hmiIsReadyNative();

    private static native void startProcessingCompleteNative();

    private static native void startProcessingFailedNative();

    private static native void appStopProcessingCompleteNative();

    private static native void fwRestartProcessingCompleteNative();

    private static native void fwResumeProcessingCompleteNative();

    private static native void appRestartProcessingCompleteNative();

    private static native void appResumeProcessingCompleteNative();

    private static native void fwStopProcessingCompleteNative();

    private static native void stopProcessingFailedNative();

    private static native void unmountCompleteNative();

    private static native void endEarlyHmiNative();

    private static native void forcedCompletionEarlyServicesNative();
}
