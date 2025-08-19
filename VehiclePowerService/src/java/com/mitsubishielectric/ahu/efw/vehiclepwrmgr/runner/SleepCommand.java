/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_CREATE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_EXEC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/** Sleep command sleeps script execution */
final class SleepCommand implements IScriptCommand {
    private final ISleep mSleep;
    private final long mSleepValueNanos;

    /**
     * Constructor
     *
     * @param sleep sleep interface
     * @param sleepValMs sleep interval, milliseconds
     */
    SleepCommand(ISleep sleep, double sleepValMs) {
        mSleep = sleep;
        mSleepValueNanos = (long) sleepValMs * 1_000_000;
        MLog.i(VPS_FUNCTION_ID, RUNNER_SLEEP_CMD_CREATE, String.valueOf(sleepValMs));
    }

    @Override
    public void execute() {
        MLog.i(VPS_FUNCTION_ID, RUNNER_SLEEP_CMD_EXEC, String.valueOf(mSleepValueNanos));
        mSleep.sleep(mSleepValueNanos);
    }
}
