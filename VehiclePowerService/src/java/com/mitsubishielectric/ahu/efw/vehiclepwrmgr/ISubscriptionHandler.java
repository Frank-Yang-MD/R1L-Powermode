/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

/** Subscription/unsubscription event handler */
public interface ISubscriptionHandler {
    /**
     * Handle subscription event from services
     *
     * @param className - service process className
     */
    void onSubscribeService(String className);

    /**
     * Handle subscription event from applications
     *
     * @param className - application process className
     */
    void onSubscribeApp(String className);

    /**
     * Handle unsubscription event from services
     *
     * @param className - service process className
     */
    void onUnsubscribeService(String className);

    /**
     * Handle unsubscription event from applications
     *
     * @param className - application process className
     */
    void onUnsubscribeApp(String className);
}
