/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement.IMountManager;

/** Class for handling events related to mount/unmount process. */
public final class MountEventHandler implements IEventHandler {

    private final IMountManager mMountManager;

    public MountEventHandler(IMountManager mountManager) {
        mMountManager = mountManager;
    }

    @Override
    public void process(IncomingEvents event) {
        if (null == event) {
            throw new IllegalArgumentException(
                    "MountEventHandler.process(): " + "IncomingEvents argument is INVALID");
        } else {
            switch (event) {
                case REQUEST_UNMOUNT:
                    mMountManager.unmount();
                    break;
                case REQUEST_MOUNT:
                    mMountManager.mount();
                    break;
                default:
                    throw new IllegalArgumentException(
                            "MountEventHandler.process(): " + "IncomingEvents argument is INVALID");
            }
        }
    }
}
