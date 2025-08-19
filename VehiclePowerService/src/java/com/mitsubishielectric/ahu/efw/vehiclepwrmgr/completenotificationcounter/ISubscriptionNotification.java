/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter;

/** Interface to notify about external subscription */
public interface ISubscriptionNotification {
    /** Notifies about external subscription by class name */
    void onSubscribe(String className);

    /** Notifies about external subscription by class name */
    void onUnsubscribe(String className);
}
