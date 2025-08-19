/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.util.List;

/** Read start Apps/Services script */
interface IScriptReader {
    /**
     * Read script text from external source
     *
     * @return script text
     */
    List<String> read();
}
