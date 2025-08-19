/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.INVALID_VALUE_FOR_START_COMPLETE_TIMEOUT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SETUP_START_COMPLETE_TIMEOUT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * A small helper for reading the timeout from config file. Timeout is used for waiting for the
 * services to start; if timeout is reached, waiting is over and VPS switches to the next service in
 * the list
 */
final class CommandTimeoutGetter {
    private static final String TIMEOUT_PARAM = "timeout";
    private static final String TIMEOUT_VALUE_PATTERN = "(\\d+)(ms)?";
    private static final Pattern mPattern =
            Pattern.compile(TIMEOUT_VALUE_PATTERN, Pattern.CASE_INSENSITIVE);

    /**
     * Basically the only function in the class which serves the its purpose.
     *
     * @param parser CommandParser used for reading the config file
     * @param paramName used for reading correct section of config file (e.g. "service" or
     *     "service_list")
     * @return timeout value in milliseconds
     */
    static int getWaitForStartCompleteTimeoutMs(ICommandParser parser, String paramName) {
        if (((parser == null) || (paramName == null))
                || (!parser.argumentNames().contains(TIMEOUT_PARAM))) {
            return 0;
        }

        String serviceKey = parser.argumentValue(paramName);
        if (serviceKey == null) {
            serviceKey = paramName;
        }

        final String strTimeout = parser.argumentValue(TIMEOUT_PARAM);
        if (strTimeout == null || strTimeout.isEmpty()) {
            MLog.i(VPS_FUNCTION_ID, INVALID_VALUE_FOR_START_COMPLETE_TIMEOUT, serviceKey);
            return 0;
        }

        final Matcher matcher = mPattern.matcher(strTimeout);
        if (!matcher.matches()) {
            MLog.i(VPS_FUNCTION_ID, INVALID_VALUE_FOR_START_COMPLETE_TIMEOUT, serviceKey);
            return 0;
        }

        final int timeout = Integer.valueOf(matcher.group(1));
        MLog.i(VPS_FUNCTION_ID, SETUP_START_COMPLETE_TIMEOUT, timeout, serviceKey);

        return timeout;
    }
}
