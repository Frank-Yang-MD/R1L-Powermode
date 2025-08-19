/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_CR_PARSE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_CR_PARSE_FAIL_FORM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_CR_PARSE_FAIL_NUM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SLEEP_CMD_CR_PARSE_FAIL_VAL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/** Creator of SleepCommand */
final class SleepCommandCreator implements ICommandCreator {
    private static final String CMD_NAME = "sleep";
    private final ICommandParser mCommandParser;

    public SleepCommandCreator(ICommandParser commandParser) {
        mCommandParser = commandParser;
    }

    @Override
    public boolean isMatch(String commandText) {
        return mCommandParser.parse(commandText) && CMD_NAME.equals(mCommandParser.commandName());
    }

    @Override
    public IScriptCommand create(String commandText, int lineNumber) {
        MLog.i(VPS_FUNCTION_ID, RUNNER_SLEEP_CMD_CR_PARSE, commandText);
        if (!isMatch(commandText)) {
            logWarning(RUNNER_SLEEP_CMD_CR_PARSE_FAIL_FORM);
            return new InvalidFormatCommand(commandText);
        }

        String sleepArgument = mCommandParser.argumentValue("value");
        String sleepValue = removeMsFromArgument(sleepArgument);

        final double sleepValueConst = getSleepValue(sleepValue);

        if (sleepValueConst < 0) {
            logWarning(RUNNER_SLEEP_CMD_CR_PARSE_FAIL_VAL);
            return new InvalidFormatCommand(commandText);
        }

        return new SleepCommand(new SimpleSleepImpl(), sleepValueConst);
    }

    private String removeMsFromArgument(String argument) {
        if (argument.endsWith("ms")) {
            return argument.substring(0, argument.length() - 2);
        } else {
            return argument;
        }
    }

    private double getSleepValue(String strVal) {
        try {
            return Double.parseDouble(strVal);
        } catch (NumberFormatException e) {
            logWarning(RUNNER_SLEEP_CMD_CR_PARSE_FAIL_NUM, strVal);
            return 0.0;
        }
    }
}
