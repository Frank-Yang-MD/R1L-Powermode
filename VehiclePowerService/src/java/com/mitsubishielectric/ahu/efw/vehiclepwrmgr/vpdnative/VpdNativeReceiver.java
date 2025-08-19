/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ON_VPD_STATE_CHG_CALL_FROM_JNI;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_FW_RESTART_FROM_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_FW_RESUME_FROM_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_FW_STOP_FROM_VPD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_HANDOFF;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_HANDOFF_STARTUP_ANIMATION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_HANDOFF_STARTUP_ANIMATION_DI;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmHandoffEventListener;

/** This class receives calls from JNI and calls corresponding callbacks. */
public final class VpdNativeReceiver {
    private final IVpdStateChangeCallback mVpdStateChangeCallback;
    private final IIsmHandoffEventListener mIsmHandoffEventListener;

    static {
        classInitNative();
    }

    public VpdNativeReceiver(
            IVpdStateChangeCallback vpdStateChangeCallback,
            IIsmHandoffEventListener ismHandoffEventListener) {
        initNative();
        mVpdStateChangeCallback = vpdStateChangeCallback;
        mIsmHandoffEventListener = ismHandoffEventListener;
    }

    void onVpdStateChange(int stateNum) {
        MLog.i(VPS_FUNCTION_ID, ON_VPD_STATE_CHG_CALL_FROM_JNI, stateNum);
        mVpdStateChangeCallback.onStateChange(stateNum);
    }

    void onFwRestart() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_FW_RESTART_FROM_VPD);
        mVpdStateChangeCallback.onFwRestart();
    }

    void onFwResume() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_FW_RESUME_FROM_VPD);
        mVpdStateChangeCallback.onFwResume();
    }

    void onFwStop() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_FW_STOP_FROM_VPD);
        mVpdStateChangeCallback.onFwStop();
    }

    void onTimeOutError() {
        mVpdStateChangeCallback.onCompletionTimeout();
    }

    void requestMount() {
        mVpdStateChangeCallback.requestMount();
    }

    void requestUnmount() {
        mVpdStateChangeCallback.requestUnmount();
    }

    void handOffResponsibility() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_HANDOFF);
        mIsmHandoffEventListener.handOffResponsibility();
    }

    void handOffResponsibilityStartupAnimationDisplayed() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_HANDOFF_STARTUP_ANIMATION);
        mIsmHandoffEventListener.handOffResponsibilityStartupAnimationDisplayed();
    }

    void handOffResponsibilityStartupAnimationAndDiDisplayed() {
        MLog.i(VPS_FUNCTION_ID, RECEIVED_HANDOFF_STARTUP_ANIMATION_DI);
        mIsmHandoffEventListener.handOffResponsibilityStartupAnimationAndDiDisplayed();
    }

    private native void initNative();

    private static native void classInitNative();
}
