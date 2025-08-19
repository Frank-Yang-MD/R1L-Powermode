/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SIMPLE_SLEEP_EXC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

/** Implementation of ISleep */
final class SimpleSleepImpl implements ISleep {

    @Override
    public void sleep(long sleepTimeNano) {
        long ms = sleepTimeNano / 1_000_000;
        int ns = (int) (sleepTimeNano % 1_000_000);
        try {
            Thread.sleep(ms, ns);
        } catch (InterruptedException e) {
            logWarning(RUNNER_SIMPLE_SLEEP_EXC, e.getMessage());
            Thread.currentThread().interrupt();
        }
    }
}
