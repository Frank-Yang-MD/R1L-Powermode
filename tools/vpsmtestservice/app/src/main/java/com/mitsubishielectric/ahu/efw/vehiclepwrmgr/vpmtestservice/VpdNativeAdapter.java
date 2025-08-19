package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpmtestservice;

public class VpdNativeAdapter implements IVpdNativeAdapter {
    @Override
    public int getVpdState() {
        return getVpdStateNative();
    }

    private static native int getVpdStateNative();
}
