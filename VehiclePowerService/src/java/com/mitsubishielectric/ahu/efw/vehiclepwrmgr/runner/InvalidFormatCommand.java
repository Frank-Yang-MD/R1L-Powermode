/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_INV_CMD_CANT_EXEC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

/** IScriptCommand implementation for invalid command */
final class InvalidFormatCommand implements IScriptCommand {
    private final String mCommand;

    InvalidFormatCommand(String command) {
        mCommand = command;
    }

    @Override
    public void execute() {
        logWarning(RUNNER_INV_CMD_CANT_EXEC, mCommand);
    }
}
