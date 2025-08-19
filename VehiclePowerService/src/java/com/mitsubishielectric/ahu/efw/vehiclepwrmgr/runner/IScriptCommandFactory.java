/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

/** Factory for script command */
interface IScriptCommandFactory {
    /**
     * Create command from text
     *
     * @param command text of command
     * @param lineNumber line number in script
     * @return IScriptCommand object
     */
    IScriptCommand createCommand(String command, int lineNumber);
}
