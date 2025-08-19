/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.util.Set;

interface ICommandParser {
    /**
     * Parse command.
     *
     * @param commandText command represented as a String
     * @return true if command parsed successfully, false otherwise
     */
    boolean parse(String commandText);

    /**
     * Returns command argument names
     *
     * @return Set of string argument names
     */
    Set<String> argumentNames();

    /**
     * Given command argument name returns it's value
     *
     * @param argumentName String representation of argument name
     * @return String value corresponding to given argument
     */
    String argumentValue(String argumentName);

    /**
     * Parsed command Name getter
     *
     * @return command name, String
     */
    String commandName();
}
