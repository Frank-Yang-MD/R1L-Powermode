/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.START_COMPLETE_TIMEOUT_OCCURR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.concurrent.atomic.AtomicReference;

/** Container for one service to startUp. Same for AppServices and FwServices */
public class StartupInfoEntry {
    private static final int TIMEOUT_STEP_MS = 10;
    private final ServiceInfo mServiceInfo;
    private volatile boolean mIsStarted;
    private volatile boolean mIsStartCompleted = false;
    private AtomicReference<EServiceState> mServiceState;
    private final Object mWait = new Object();
    private final IVpdNativeAdapter mVpd;

    public StartupInfoEntry(ServiceInfo serviceInfo, IVpdNativeAdapter vpd) {
        mServiceInfo = serviceInfo;
        mVpd = vpd;
        mServiceState = new AtomicReference<>(EServiceState.NOT_STARTED);
    }

    /**
     * Check whether startup command was issued
     *
     * @return true if startup command was issued
     */
    public boolean isStarted() {
        return mIsStarted;
    }
    /**
     * This method should be called after startup command was issued for particular Service
     *
     * @param isStarted true if started
     */
    public void setIsStarted(boolean isStarted) {
        mIsStarted = isStarted;
    }

    /**
     * Check whether startup was completed by particular service
     *
     * @return true if completed
     */
    public boolean isStartCompleted() {
        return mIsStartCompleted;
    }

    /**
     * This method should be called when particular service reported that it's startup was finished
     *
     * @param isStartCompleted true if startup completed
     */
    public void setIsStartCompleted(boolean isStartCompleted) {
        synchronized (mWait) {
            mIsStartCompleted = isStartCompleted;
            mServiceState.compareAndSet(EServiceState.NOT_STARTED, EServiceState.STARTED);
            mWait.notifyAll();
        }
    }

    /**
     * Service info object getter
     *
     * @return ServiceInfo object
     */
    public ServiceInfo getServiceInfo() {
        return mServiceInfo;
    }

    /** Wait for start completion of Service stored in this entry */
    public void waitStartComplete() {
        while (!mIsStartCompleted) {
            synchronized (mWait) {
                try {
                    mWait.wait(10);
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }
    }

    /**
     * Wait for start completion of Service stored in this entry for timeoutMs
     *
     * @param timeoutMs waiting timeout
     */
    public boolean waitStartComplete(int timeoutMs) {
        int timeLeft = timeoutMs;
        while (!mIsStartCompleted && timeLeft > 0) {
            synchronized (mWait) {
                try {
                    int timeout = Math.min(timeLeft, TIMEOUT_STEP_MS);
                    mWait.wait(timeout);
                    timeLeft -= timeout;
                } catch (InterruptedException e) {
                    Thread.currentThread().interrupt();
                }
            }
        }

        if (timeLeft > 0) {
            return true;
        }

        mVpd.vpdStartFailed();
        MLog.e(VPS_FUNCTION_ID, START_COMPLETE_TIMEOUT_OCCURR, mServiceInfo.getClassName());
        return false;
    }

    public EServiceState getServiceState() {
        return mServiceState.get();
    }

    public void setStopCompleted() {
        synchronized (mWait) {
            mServiceState.compareAndSet(EServiceState.STARTED, EServiceState.STOPPED);
        }
    }

    public void setResumedRestarted() {
        synchronized (mWait) {
            mServiceState.compareAndSet(EServiceState.STOPPED, EServiceState.STARTED);
        }
    }
}
