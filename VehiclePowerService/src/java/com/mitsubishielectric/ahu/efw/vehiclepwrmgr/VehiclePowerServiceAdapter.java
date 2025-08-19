/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.FORCED_COMPLETION_EARLY_SERVICES_NOTIFICATION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.GET_NOTIFIED_EVT_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ISM_READY_NOTIFICATION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.NOTIFY_START_COMPLETE_CALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.REQUEST_END_EARLY_HMI_NOTIFICATION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RESTART_COMPLETE_APP_CALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RESTART_COMPLETE_CALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RESUME_COMPLETE_APP_CALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RESUME_COMPLETE_CALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.STOP_PROC_COMPLETE_APP_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.STOP_PROC_COMPLETE_FW_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCRIBE_APP_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCRIBE_SRV_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.UNSUBSCRIBE_CALLED_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_GET_SERVICE_STATE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_ALL_APP_SERVICES_STARTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_ALL_EFW_SERVICES_STARTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_LATE_APP_SERVICES_START_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_LATE_EFW_SERVICES_START_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_NORMAL_APP_SERVICES_START_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_NORMAL_EFW_SERVICES_START_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_IS_SERVICE_STARTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_NORMAL_APP_SERVICES_STARTED;

import android.content.Context;
import android.os.Binder;
import android.os.RemoteException;
import android.util.Log;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerService;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmSubscription;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

public final class VehiclePowerServiceAdapter extends IVehiclePowerService.Stub {
    private final IEventReceiver mEventReceiver;
    private final ISubscriptionManager mAppSubscriptionManager;
    private final ISubscriptionManager mServiceSubscriptionManager;
    private final IStateHolder mStateHolder;
    private final Context mContext;
    private final IStartupInfo mStartupInfo;
    private final ICompleteNotification mStopCompleteAppHandler;
    private final ICompleteNotification mStopCompleteFwHandler;
    private final ICompleteNotification mAppResumeCompleteHandler;
    private final ICompleteNotification mFwResumeCompleteHandler;
    private final ICompleteNotification mAppRestartCompleteHandler;
    private final ICompleteNotification mFwRestartCompleteHandler;
    private final IIsmSubscription mIsmSubscription;
    private final IVpdNativeAdapter mVpdNativeAdapter;

    VehiclePowerServiceAdapter(
            IEventReceiver eventReceiver,
            ISubscriptionManager appSubscriptionManager,
            ISubscriptionManager serviceSubscriptionManager,
            IStateHolder stateHolder,
            Context context,
            IStartupInfo startupInfo,
            ICompleteNotification stopCompleteAppHandler,
            ICompleteNotification stopCompleteFwHandler,
            ICompleteNotification fwResumeCompleteHandler,
            ICompleteNotification fwRestartCompleteHandler,
            ICompleteNotification appResumeCompleteHandler,
            ICompleteNotification appRestartCompleteHandler,
            IIsmSubscription ismSubscription,
            IVpdNativeAdapter vpdNativeAdapter) {
        mEventReceiver = eventReceiver;
        mAppSubscriptionManager = appSubscriptionManager;
        mServiceSubscriptionManager = serviceSubscriptionManager;
        mStateHolder = stateHolder;
        mContext = context;
        mStartupInfo = startupInfo;
        mStopCompleteAppHandler = stopCompleteAppHandler;
        mStopCompleteFwHandler = stopCompleteFwHandler;
        mFwResumeCompleteHandler = fwResumeCompleteHandler;
        mFwRestartCompleteHandler = fwRestartCompleteHandler;
        mAppResumeCompleteHandler = appResumeCompleteHandler;
        mAppRestartCompleteHandler = appRestartCompleteHandler;
        mIsmSubscription = ismSubscription;
        mVpdNativeAdapter = vpdNativeAdapter;
    }

    public void subscribeApp(IVehiclePowerServiceListener listener, String className) {
        String clsNameForLog = shortenClassName(className);
        MLog.i(VPS_FUNCTION_ID, SUBSCRIBE_APP_CALLED_FROM, clsNameForLog);
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCRIBE_APP_CALLED_FROM, clsNameForLog);
        mAppSubscriptionManager.subscribe(listener, className);
    }

    @Override
    public void subscribeFWService(IVehiclePowerServiceListener listener, String className) {
        String clsNameForLog = shortenClassName(className);
        MLog.i(VPS_FUNCTION_ID, SUBSCRIBE_SRV_CALLED_FROM, clsNameForLog);
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCRIBE_SRV_CALLED_FROM, clsNameForLog);
        mServiceSubscriptionManager.subscribe(listener, className);
    }

    @Override
    public void unsubscribe(IVehiclePowerServiceListener listener, String className) {
        String clsNameForLog = shortenClassName(className);
        MLog.i(VPS_FUNCTION_ID, UNSUBSCRIBE_CALLED_FROM, clsNameForLog);
        mAppSubscriptionManager.unsubscribe(listener, className);
        mServiceSubscriptionManager.unsubscribe(listener, className);
    }

    @Override
    public void stopCompleteApp(String className) {
        String clsNameForLog = shortenClassName(className);
        MLog.i(VPS_FUNCTION_ID, STOP_PROC_COMPLETE_APP_CALLED_FROM, clsNameForLog);
        mStopCompleteAppHandler.onComplete(className);
        mStartupInfo.onStopComplete(className);
    }

    @Override
    public void stopCompleteEfw(String className) {
        String clsNameForLog = shortenClassName(className);
        MLog.i(VPS_FUNCTION_ID, STOP_PROC_COMPLETE_FW_CALLED_FROM, clsNameForLog);
        mStopCompleteFwHandler.onComplete(className);
        mStartupInfo.onStopComplete(className);
    }

    @Override
    public boolean isServiceStarted(String className) throws RemoteException {
        final boolean status = mStartupInfo.isServiceStarted(className);
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_SERVICE_STARTED,
                "called from "
                        + mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid());
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_SERVICE_STARTED,
                "Input class name: "
                        + mStartupInfo.getShortClassName(className)
                        + "; Return status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public void startComplete(String className) throws RemoteException {
        MLog.i(
                VPS_FUNCTION_ID,
                NOTIFY_START_COMPLETE_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        MLog.i(
                VPS_EARLY_FUNCTION_ID,
                NOTIFY_START_COMPLETE_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        mStartupInfo.onServiceStartupCompleted(className);
    }

    @Override
    public boolean isAllEfwServicesStarted() throws RemoteException {
        final boolean status = mStartupInfo.isAllServicesStarted();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_ALL_EFW_SERVICES_STARTED,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public boolean normalAppServicesStarted() throws RemoteException {
        final boolean status = mStartupInfo.isEarlyAppsStarted();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_NORMAL_APP_SERVICES_STARTED,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public boolean isAllAppServicesStarted() throws RemoteException {
        final boolean status = mStartupInfo.isAllAppsStarted();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_ALL_APP_SERVICES_STARTED,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public EVpsState getNotifiedEvent() {
        MLog.i(
                VPS_FUNCTION_ID,
                GET_NOTIFIED_EVT_CALLED_FROM,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid());
        return mStateHolder.getAppState();
    }

    @Override
    public int printMarker(String markerTAG) {
        return Log.printMarker(markerTAG);
    }

    @Override
    public void resumeCompleteEfw(String className) {
        MLog.i(
                VPS_FUNCTION_ID,
                RESUME_COMPLETE_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        mFwResumeCompleteHandler.onComplete(className);
        mStartupInfo.onRestartResumeComplete(className);
    }

    @Override
    public void restartCompleteEfw(String className) {
        MLog.i(
                VPS_FUNCTION_ID,
                RESTART_COMPLETE_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        mFwRestartCompleteHandler.onComplete(className);
        mStartupInfo.onRestartResumeComplete(className);
    }

    @Override
    public void resumeCompleteApp(String className) {
        MLog.i(
                VPS_FUNCTION_ID,
                RESUME_COMPLETE_APP_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        mAppResumeCompleteHandler.onComplete(className);
        mStartupInfo.onRestartResumeComplete(className);
    }

    @Override
    public void restartCompleteApp(String className) {
        MLog.i(
                VPS_FUNCTION_ID,
                RESTART_COMPLETE_APP_CALL,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + " : Param: "
                        + mStartupInfo.getShortClassName(className));
        mAppRestartCompleteHandler.onComplete(className);
        mStartupInfo.onRestartResumeComplete(className);
    }

    @Override
    public EServiceState getServiceState(String className) {
        final EServiceState serviceState = mStartupInfo.getServiceState(className);
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_GET_SERVICE_STATE,
                "called from "
                        + mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Input class name: "
                        + mStartupInfo.getShortClassName(className));
        if (serviceState != null) {
            MLog.i(VPS_FUNCTION_ID, VPS_GET_SERVICE_STATE, "EServiceState : " + serviceState);
        }
        return serviceState;
    }

    @Override
    public boolean isNormalEfwServicesStartComplete() {
        final boolean status = mStartupInfo.isNormalEfwServicesStartComplete();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_NORMAL_EFW_SERVICES_START_COMPLETE,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public boolean isNormalAppServicesStartComplete() {
        final boolean status = mStartupInfo.isNormalAppServicesStartComplete();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_NORMAL_APP_SERVICES_START_COMPLETE,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public boolean isLateEfwServicesStartComplete() {
        final boolean status = mStartupInfo.isLateEfwServicesStartComplete();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_LATE_EFW_SERVICES_START_COMPLETE,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public boolean isLateAppServicesStartComplete() {
        final boolean status = mStartupInfo.isLateAppServicesStartComplete();
        MLog.i(
                VPS_FUNCTION_ID,
                VPS_IS_LATE_APP_SERVICES_START_COMPLETE,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid()
                        + "; Status: "
                        + String.valueOf(status));
        return status;
    }

    @Override
    public void ismReady(IBootDaemonStatusListener listener) {
        MLog.i(
                VPS_FUNCTION_ID,
                ISM_READY_NOTIFICATION,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid());
        mIsmSubscription.subscribeIsm(listener);
    }

    @Override
    public void requestEndEarlyHmi() {
        MLog.i(
                VPS_FUNCTION_ID,
                REQUEST_END_EARLY_HMI_NOTIFICATION,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid());
        mVpdNativeAdapter.endEarlyHmi();
    }

    @Override
    public void forcedCompletionEarlyServices() {
        MLog.i(
                VPS_FUNCTION_ID,
                FORCED_COMPLETION_EARLY_SERVICES_NOTIFICATION,
                mContext.getPackageManager().getNameForUid(Binder.getCallingUid())
                        + "; Caller PID: "
                        + Binder.getCallingPid());
        mVpdNativeAdapter.forcedCompletionEarlyServices();
    }

    private String shortenClassName(String className) {
        String clsNameForLog = mStartupInfo.getShortClassName(className);
        String uid = mContext.getPackageManager().getNameForUid(Binder.getCallingUid());
        if (!className.startsWith(uid)) {
            clsNameForLog = uid + "; Caller PID: " + clsNameForLog;
        }
        return clsNameForLog;
    }
}
