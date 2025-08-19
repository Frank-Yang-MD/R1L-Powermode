/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.util.List;

/** Startup script executor interface */
interface IScriptExecutor {
    /**
     * Execute startup script
     *
     * @param script startup script
     */
    void execute(List<String> script);
}
