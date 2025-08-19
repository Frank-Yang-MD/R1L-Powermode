/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SCRIPT_EXEC_FAIL_CMD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SCRIPT_EXEC_PARSE_CMD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SCRIPT_EXEC_SEND_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_SCRIPT_EXEC_START_LEN;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import java.util.List;

/** Executor of start script */
final class ScriptExecutor implements IScriptExecutor {
    private final IScriptCommandFactory mFactory;
    private final IEventReceiver mEventReceiver;
    private final IncomingEvents mEvent;

    /**
     * Constructor
     *
     * @param factory factory to produce IScriptCommand
     * @param eventReceiver event receiver interface
     * @param event type of event will be sent to eventReceiver when script finish
     */
    ScriptExecutor(
            IScriptCommandFactory factory, IEventReceiver eventReceiver, IncomingEvents event) {
        mFactory = factory;
        mEventReceiver = eventReceiver;
        mEvent = event;
    }

    /** @param script startup script */
    @Override
    public void execute(List<String> script) {
        MLog.i(VPS_FUNCTION_ID, RUNNER_SCRIPT_EXEC_START_LEN, script.size());
        int lineNumber = 1;
        for (String command : script) {
            MLog.i(VPS_FUNCTION_ID, RUNNER_SCRIPT_EXEC_PARSE_CMD, command);
            IScriptCommand cmd = mFactory.createCommand(command, lineNumber++);
            if (cmd != null) {
                cmd.execute();
            } else {
                logWarning(RUNNER_SCRIPT_EXEC_FAIL_CMD, command);
            }
        }
        MLog.i(VPS_FUNCTION_ID, RUNNER_SCRIPT_EXEC_SEND_EVT, mEvent.name());
        mEventReceiver.onReceiveEvent(mEvent);
    }
}
