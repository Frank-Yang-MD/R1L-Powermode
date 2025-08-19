/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_CR_PARSE_CMD_TXT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.content.ComponentName;
import android.content.Context;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import java.util.Set;

/** Creator of start servie/application commandName */
final class StartCommandCreator implements ICommandCreator {
    private static final String CMD_NAME = "start";
    private static final String SERVICE_PARAM = "service";
    private static final String WAIT_PARAM_NAME = "wait_startup_completion";

    private final ICommandParser mCommandParser;
    private final Context mContext;
    private final IBinder mBinder;
    private final IInfoContainer mInfoContainer;
    private final IStartupInfo mStartupInfo;
    private String mServiceKey;

    StartCommandCreator(
            Context context,
            IBinder binder,
            ICommandParser commandParser,
            IInfoContainer infoContainer,
            IStartupInfo startupInfo) {
        mContext = context;
        mBinder = binder;
        mCommandParser = commandParser;
        mInfoContainer = infoContainer;
        mStartupInfo = startupInfo;
    }

    @Override
    public boolean isMatch(String commandText) {
        if (!mCommandParser.parse(commandText)) {
            return false;
        }
        String commandName = mCommandParser.commandName();

        return commandName != null && commandName.equals(CMD_NAME);
    }

    @Override
    public IScriptCommand create(String commandText, int lineNumber) {
        MLog.i(VPS_FUNCTION_ID, RUNNER_START_CMD_CR_PARSE_CMD_TXT, commandText);
        if (!validate(commandText)) {
            return new InvalidFormatCommand(commandText);
        }
        mServiceKey = mCommandParser.argumentValue(SERVICE_PARAM);
        String pkg = mInfoContainer.getServiceInfo(mServiceKey).getPackageName();
        String cls = mInfoContainer.getServiceInfo(mServiceKey).getClassName();

        StartCommand startCommand =
                new StartCommand(mContext, mBinder, new ComponentName(pkg, cls), mStartupInfo);
        setAction(startCommand);
        setWaitForStartComplete(startCommand);
        return startCommand;
    }

    private void setWaitForStartComplete(StartCommand startCommand) {
        if (mCommandParser.argumentNames().contains(WAIT_PARAM_NAME)) {
            final int timeout =
                    CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                            mCommandParser, SERVICE_PARAM);
            final Boolean isWaitNeeded =
                    Boolean.valueOf(mCommandParser.argumentValue(WAIT_PARAM_NAME));
            startCommand.enableWaitForStartComplete(isWaitNeeded, timeout);
        }
    }

    private boolean validate(String commandText) {
        if (!isMatch(commandText)) {
            return false;
        }

        Set<String> args = mCommandParser.argumentNames();
        return args.contains(SERVICE_PARAM);
    }

    private void setAction(StartCommand startCommand) {
        if (mInfoContainer.getServiceInfo(mServiceKey).getAction() != null) {
            String action = mInfoContainer.getServiceInfo(mServiceKey).getAction();
            startCommand.setAction(action);
        }
    }
}
