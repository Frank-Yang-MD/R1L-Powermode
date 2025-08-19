/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

/** Interface for handling completion related events */
public interface ICompletionHandler {
    void onAppServicesStopCompleted();

    void onAppServicesRestartComplete();

    void onAppServicesResumeComplete();

    void onEfwRestartCompleted();

    void onEfwResumeCompleted();

    void onEfwServicesStopCompleted();

    void onCompletionTimeout();
}
