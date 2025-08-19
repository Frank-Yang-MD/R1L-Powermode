/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.Objects;
import java.util.concurrent.Executor;

/**
 * This class starts mounting/unmounting process in a separate thread in order to avoid blocking of
 * main event queue.
 */
public class MountManager implements IMountManager {

    private final IStorageManagerAdapter mStorageManagerAdapter;
    private final IVpdNativeAdapter mVpdNativeAdapter;
    private final Executor mExecutor;

    public MountManager(
            IStorageManagerAdapter storageManagerAdapter,
            IVpdNativeAdapter vpdNativeAdapter,
            Executor executor) {
        mStorageManagerAdapter =
                Objects.requireNonNull(storageManagerAdapter, "StorageManagerAdapter is null");
        mVpdNativeAdapter =
                Objects.requireNonNull(vpdNativeAdapter, "VpdNativeAdapter " + "is null");
        mExecutor = Objects.requireNonNull(executor, "Executor is null");
    }

    @Override
    public void mount() {
        mStorageManagerAdapter.mountUnmounted();
    }

    @Override
    public void unmount() {
        mExecutor.execute(this::doUnmount);
    }

    private void doUnmount() {
        mStorageManagerAdapter.unmountAll();
        mVpdNativeAdapter.unmountComplete();
    }
}
