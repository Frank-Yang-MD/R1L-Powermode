/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

/** Interface for Event Handler to notify processors. */
public interface IStartHandler {
    /** To be called when early apps start process finished */
    void onNormalAppServicesStarted();

    void onLateAppServicesStarted();

    void onNormalEfwServicesStarted();

    void onLateEfwServicesStarted();
}
