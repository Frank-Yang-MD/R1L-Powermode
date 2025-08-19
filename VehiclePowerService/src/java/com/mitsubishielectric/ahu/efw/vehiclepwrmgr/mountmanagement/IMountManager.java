/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

/** IMountManager - interface for mount and unmount volumes. */
public interface IMountManager {
    /** Mount volumes. */
    void mount();

    /** Unmount volumes. */
    void unmount();
}
