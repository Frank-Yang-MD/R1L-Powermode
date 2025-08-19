/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriptionManager;

/**
 * Subscription manager. It supports callback list and send power state change notification. When
 * clients request subscription or unsubscription SubscriptionManager send an appropriate event to
 * an external event receiver.
 */
public final class SubscriptionManager implements ISubscriptionManager, ISubscriberNotification {
    private final SubscriptionRemoteCallbackList mVpsListeners;

    /**
     * SubscriptionManager constructor
     *
     * @param callbackList external event receiver
     */
    public SubscriptionManager(SubscriptionRemoteCallbackList callbackList) {
        mVpsListeners = callbackList;
    }

    /** Send APP_RESTART notification to all subscribers */
    @Override
    public void notifyAppRestart() {
        mVpsListeners.notifyAllListeners(EVpsState.APP_RESTART);
    }

    /** Send APP_RESUME notification to all subscribers */
    @Override
    public void notifyAppResume() {
        mVpsListeners.notifyAllListeners(EVpsState.APP_RESUME);
    }

    /** Send APP_START_COLDBOOT notification to all subscribers */
    @Override
    public void notifyAppStart() {
        mVpsListeners.notifyAllListeners(EVpsState.APP_START_COLDBOOT);
    }

    /** Send APP_STOP notification to all subscribers */
    @Override
    public void notifyAppStop() {
        mVpsListeners.notifyAllListeners(EVpsState.APP_STOP);
    }

    /**
     * Subscription request
     *
     * @param pwrListener client callback
     * @param className client className
     */
    @Override
    public void subscribe(IVehiclePowerServiceListener pwrListener, String className) {
        mVpsListeners.subscribe(pwrListener, className);
    }

    /**
     * Unsubscription request
     *
     * @param pwrListener client callback
     * @param className client className
     */
    @Override
    public void unsubscribe(IVehiclePowerServiceListener pwrListener, String className) {
        mVpsListeners.unsubscribe(pwrListener, className);
    }
}
