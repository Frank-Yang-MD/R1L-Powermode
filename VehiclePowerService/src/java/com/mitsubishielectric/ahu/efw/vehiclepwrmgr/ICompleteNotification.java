/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

/** Stop complete notification handler */
public interface ICompleteNotification {
    /**
     * Handle stop complete notification from clients
     *
     * @param className - client process className
     */
    void onComplete(String className);
}
