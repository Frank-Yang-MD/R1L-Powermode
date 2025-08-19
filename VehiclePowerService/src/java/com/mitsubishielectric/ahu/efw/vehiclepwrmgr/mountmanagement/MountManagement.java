/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.MountEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.concurrent.Executors;

/** This class is a holder for instances of objects related to mounting/unmounting. */
public final class MountManagement {
    private final IStorageManagerAdapter mStorageManagerAdapter = new StorageManagerAdapter();
    private final IMountManager mMountManager;
    private final MountEventHandler mMountEventHandler;

    public MountManagement(IVpdNativeAdapter vpdNativeAdapter) {
        mMountManager =
                new MountManager(
                        mStorageManagerAdapter,
                        vpdNativeAdapter,
                        Executors.newSingleThreadExecutor());
        mMountEventHandler = new MountEventHandler(mMountManager);
    }

    public MountEventHandler getMountEventHandler() {
        return mMountEventHandler;
    }
}
