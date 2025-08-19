/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_AND_DI_DISPLAYED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_DISPLAYED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_EXCEPTION_ISMSUBSCRIPTION_SUBSCRIBEISM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_NOTIFY_TO_EMPTY_LISTENER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_SEND_HANDOFF;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_SEND_HANDOFF_STARTUP_ANIMATION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_SEND_HANDOFF_STARTUP_ANIMATION_DI;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.os.RemoteException;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EBootDaemonStatus;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EScreenProgress;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg;

public final class IsmSubscription implements IIsmSubscription, IIsmHandoffEventListener {

    private IBootDaemonStatusListener mBootDaemonStatusListener = null;
    private EBootDaemonStatus mBootDaemonStatus = EBootDaemonStatus.ACTIVE;
    private EScreenProgress mScreenProgress = EScreenProgress.UNKNOWN;

    @Override
    public void subscribeIsm(IBootDaemonStatusListener bootDaemonStatusListener) {
        if (bootDaemonStatusListener == null) {
            MLog.w(VPS_FUNCTION_ID, ISM_NOTIFY_TO_EMPTY_LISTENER);
            return;
        }
        mBootDaemonStatusListener = bootDaemonStatusListener;
        try {
            mBootDaemonStatusListener.notifyBootDaemonStatus(mBootDaemonStatus, mScreenProgress);
        } catch (RemoteException e) {
            VPSLogMsg.logException(ISM_EXCEPTION_ISMSUBSCRIPTION_SUBSCRIBEISM, e);
        }
    }

    @Override
    public void handOffResponsibility() {
        mBootDaemonStatus = EBootDaemonStatus.FINISH;
        mScreenProgress = EScreenProgress.BLACK_SCREEN;
        if (mBootDaemonStatusListener == null) {
            MLog.w(VPS_FUNCTION_ID, ISM_NOTIFY_TO_EMPTY_LISTENER);
            return;
        }
        try {
            MLog.i(VPS_FUNCTION_ID, ISM_SEND_HANDOFF);
            mBootDaemonStatusListener.notifyBootDaemonStatus(mBootDaemonStatus, mScreenProgress);
        } catch (RemoteException e) {
            VPSLogMsg.logException(ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF, e);
        }
    }

    @Override
    public void handOffResponsibilityStartupAnimationDisplayed() {
        mBootDaemonStatus = EBootDaemonStatus.FINISH;
        mScreenProgress = EScreenProgress.STARTUP_ANIMATION;
        if (mBootDaemonStatusListener == null) {
            MLog.w(VPS_FUNCTION_ID, ISM_NOTIFY_TO_EMPTY_LISTENER);
            return;
        }
        try {
            MLog.i(VPS_FUNCTION_ID, ISM_SEND_HANDOFF_STARTUP_ANIMATION);
            mBootDaemonStatusListener.notifyBootDaemonStatus(mBootDaemonStatus, mScreenProgress);
        } catch (RemoteException e) {
            VPSLogMsg.logException(ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_DISPLAYED, e);
        }
    }

    @Override
    public void handOffResponsibilityStartupAnimationAndDiDisplayed() {
        mBootDaemonStatus = EBootDaemonStatus.FINISH;
        mScreenProgress = EScreenProgress.DISCLAIMER;
        if (mBootDaemonStatusListener == null) {
            MLog.w(VPS_FUNCTION_ID, ISM_NOTIFY_TO_EMPTY_LISTENER);
            return;
        }
        try {
            MLog.i(VPS_FUNCTION_ID, ISM_SEND_HANDOFF_STARTUP_ANIMATION_DI);
            mBootDaemonStatusListener.notifyBootDaemonStatus(mBootDaemonStatus, mScreenProgress);
        } catch (RemoteException e) {
            VPSLogMsg.logException(
                    ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_AND_DI_DISPLAYED, e);
        }
    }
}
