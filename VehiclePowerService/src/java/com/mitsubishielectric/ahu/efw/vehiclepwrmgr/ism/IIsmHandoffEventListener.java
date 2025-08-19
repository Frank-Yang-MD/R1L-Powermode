/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism;

/** This interface is intended for sending 'hand off' message to ISM */
public interface IIsmHandoffEventListener {

    /**
     * Notifies, that BootDaemon has finished its work and startup animation or disclamier wasn't
     * displayed.
     */
    public void handOffResponsibility();

    /** Notifies, that BootDaemon has finished its work and only startup animation was displayed. */
    public void handOffResponsibilityStartupAnimationDisplayed();

    /**
     * Notifies, that BootDaemon has finished its work and startup animation and disclaimer was
     * displayed.
     */
    public void handOffResponsibilityStartupAnimationAndDiDisplayed();
}
