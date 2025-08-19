/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

/** Create command from one script text line */
interface ICommandCreator {
    /**
     * Check if command text is matched to command
     *
     * @param commandText text of command
     * @return true if text matched
     */
    boolean isMatch(String commandText);

    /**
     * Create IScriptCommand object
     *
     * @param commandText text of script command
     * @param lineNumber number of line in script
     * @return IScriptCommand object
     */
    IScriptCommand create(String commandText, int lineNumber);
}
