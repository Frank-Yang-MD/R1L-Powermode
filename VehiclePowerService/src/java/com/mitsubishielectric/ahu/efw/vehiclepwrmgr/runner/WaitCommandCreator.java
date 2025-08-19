package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SETUP_START_COMPLETE_TIMEOUT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAITCMDCREATOR_CANNOT_PARSE_CMD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAITCMDCREATOR_SRV_LIST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import java.util.stream.Collectors;

public class WaitCommandCreator implements ICommandCreator {
    private static final String CMD_NAME = "wait_services_started";
    private static final String SERVICE_LIST_PARAM = "service_list";
    private final ICommandParser mCommandParser;
    private final IStartupInfo mStartupInfo;
    private final IInfoContainer mInfoContainer;

    WaitCommandCreator(
            ICommandParser commandParser, IStartupInfo startupInfo, IInfoContainer infoContainer) {
        mCommandParser = commandParser;
        mStartupInfo = startupInfo;
        mInfoContainer = infoContainer;
    }

    @Override
    public boolean isMatch(String commandText) {
        return mCommandParser.parse(commandText) && CMD_NAME.equals(mCommandParser.commandName());
    }

    @Override
    public IScriptCommand create(String commandText, int lineNumber) {
        if (!isMatch(commandText)) {
            logWarning(WAITCMDCREATOR_CANNOT_PARSE_CMD, commandText);
            return new InvalidFormatCommand(commandText);
        }

        final int timeout =
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, SERVICE_LIST_PARAM);
        MLog.i(VPS_FUNCTION_ID, SETUP_START_COMPLETE_TIMEOUT, timeout, commandText);

        return new WaitCommand(getServiceList(commandText), mStartupInfo, mInfoContainer, timeout);
    }

    private List<String> getServiceList(String commandText) {
        if (mCommandParser.parse(commandText)) {
            final String serviceListArray = mCommandParser.argumentValue(SERVICE_LIST_PARAM);
            final String servicesWithoutBrackets = trunc(serviceListArray);
            final String[] serviceList = servicesWithoutBrackets.split(",");
            MLog.d(VPS_FUNCTION_ID, WAITCMDCREATOR_SRV_LIST, servicesWithoutBrackets);
            return Arrays.asList(serviceList).stream()
                    .map(this::trunc)
                    .collect(Collectors.toList());
        }
        return Collections.EMPTY_LIST;
    }

    private String trunc(String str) {
        return str.substring(1, str.length() - 1);
    }
}
