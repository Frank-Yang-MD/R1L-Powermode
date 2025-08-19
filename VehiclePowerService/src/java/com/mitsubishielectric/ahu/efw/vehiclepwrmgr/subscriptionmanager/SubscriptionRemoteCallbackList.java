/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_ALL_LISTENERS_NOTIFIED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_BROADCAST_FINISHED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_CANT_CALL_REMOTE_LISTENER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_DISCONNECT_LISTENER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_ILLEG_ARG_EXCEPTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_NOTIFY_ALL_INVALID_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_NOTIFY_ALL_SUBSCR_PWR_STATE_CHG;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_NOTIFY_SUBSCR_PWR_STATE_CHG;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_RECEIVED_SUBSCR_REQUEST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SUBSCR_MAN_RECEIVED_UNSUBSCR_REQUEST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.os.RemoteCallbackList;
import android.os.RemoteException;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStateHolder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ISubscriptionNotification;

/** This class support callback list and power state change client notification */
public class SubscriptionRemoteCallbackList
        extends RemoteCallbackList<IVehiclePowerServiceListener> {
    private final IStateHolder mStateHolder;
    private final ISubscriptionNotification mSubscriptionNotification;

    /**
     * SubscriptionRemoteCallbackList constructor
     *
     * @param stateHolder state holder interface
     * @param subscriptionNotification manager for subscribe / unsubscribe applications and services
     */
    public SubscriptionRemoteCallbackList(
            IStateHolder stateHolder, ISubscriptionNotification subscriptionNotification) {
        mStateHolder = stateHolder;
        mSubscriptionNotification = subscriptionNotification;
    }

    @Override
    public void onCallbackDied(IVehiclePowerServiceListener callback, Object cookie) {
        super.onCallbackDied(callback, cookie);
        MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_DISCONNECT_LISTENER, callback.hashCode());
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCR_MAN_DISCONNECT_LISTENER, callback.hashCode());

        mSubscriptionNotification.onUnsubscribe((String) cookie);
    }

    public void notifyListener(EVpsState state, IVehiclePowerServiceListener listener) {
        if (state == null) {
            throw new IllegalArgumentException("notifyListener(): EVpsState is NULL!");
        }
        if (listener == null) {
            throw new IllegalArgumentException(
                    "notifyListener(): IVehiclePowerServiceListener is" + " NULL!");
        }
        MLog.i(
                VPS_FUNCTION_ID,
                SUBSCR_MAN_NOTIFY_SUBSCR_PWR_STATE_CHG,
                state.toString() + " " + listener.hashCode());
        MLog.i(
                VPS_EARLY_FUNCTION_ID,
                SUBSCR_MAN_NOTIFY_SUBSCR_PWR_STATE_CHG,
                state.toString() + " " + listener.hashCode());
        try {
            switch (state) {
                case APP_RESTART:
                    listener.onAppRestart();
                    break;
                case APP_RESUME:
                    listener.onAppResume();
                    break;
                case APP_START_COLDBOOT:
                    listener.onAppStart();
                    break;
                case APP_STOP:
                    listener.onAppStop();
                    break;
                default:
                    logWarning(SUBSCR_MAN_NOTIFY_ALL_INVALID_EVT, state.toString());
            }
        } catch (RemoteException e) {
            logWarning(
                    SUBSCR_MAN_CANT_CALL_REMOTE_LISTENER,
                    listener.hashCode() + ", error: " + e.getMessage());
        }
    }

    public synchronized void notifyAllListeners(EVpsState state) {
        MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_NOTIFY_ALL_SUBSCR_PWR_STATE_CHG, state.toString());
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCR_MAN_NOTIFY_ALL_SUBSCR_PWR_STATE_CHG, state.toString());
        final int callbackCount = beginBroadcast();
        try {
            for (int i = 0; i < callbackCount; i++) {
                notifyListener(state, getBroadcastItem(i));
            }
        } finally {
            finishBroadcast();
            MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_BROADCAST_FINISHED);
        }
        MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_ALL_LISTENERS_NOTIFIED);
    }

    /**
     * Subscription request
     *
     * @param pwrListener client callback
     * @param className client className
     */
    public void subscribe(IVehiclePowerServiceListener pwrListener, String className) {
        MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_RECEIVED_SUBSCR_REQUEST, pwrListener.hashCode());
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCR_MAN_RECEIVED_SUBSCR_REQUEST, pwrListener.hashCode());
        register(pwrListener, className);

        mSubscriptionNotification.onSubscribe(className);
        try {
            notifyListener(mStateHolder.getAppState(), pwrListener);
        } catch (IllegalArgumentException e) {
            logWarning(SUBSCR_MAN_ILLEG_ARG_EXCEPTION, e.getMessage());
        }
    }

    /**
     * Unsubscription request
     *
     * @param pwrListener client callback
     * @param className client className
     */
    public void unsubscribe(IVehiclePowerServiceListener pwrListener, String className) {
        MLog.i(VPS_FUNCTION_ID, SUBSCR_MAN_RECEIVED_UNSUBSCR_REQUEST, pwrListener.hashCode());
        MLog.i(VPS_EARLY_FUNCTION_ID, SUBSCR_MAN_RECEIVED_UNSUBSCR_REQUEST, pwrListener.hashCode());
        unregister(pwrListener);

        mSubscriptionNotification.onUnsubscribe(className);
    }
}
