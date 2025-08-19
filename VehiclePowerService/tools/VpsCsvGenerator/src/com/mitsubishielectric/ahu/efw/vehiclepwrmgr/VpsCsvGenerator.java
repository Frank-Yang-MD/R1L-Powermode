/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

public final class VpsCsvGenerator {
    public static void main(String args[]) {
        VPSLogMsg.initializeLogDog();
        MLog.dumpToCsv(VPSLogMsg.VPM_FUNCTION_ID, "VPS_java.csv");
        VPSLogMsg.terminateLogDog();
    }
}
