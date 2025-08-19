/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.FAILED_TO_PARSE_COMMAND_JSON_PARSER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.JSONCMDPARS_FAILED_TO_PARSE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logException;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import java.util.HashSet;
import java.util.Iterator;
import java.util.Set;
import org.json.JSONException;
import org.json.JSONObject;

/** Parses startup Log for VPS in JSON format line by line */
public class JsonCommandParser implements ICommandParser {
    private static String COMMAND_IDENTIFIER = "command";
    private String mCommandName;
    private JSONObject mJsonCommand;
    private Set<String> mCommandKeys;

    @Override
    public boolean parse(String commandText) {
        boolean returnValue = false;
        mCommandKeys = new HashSet<>();
        try {
            mJsonCommand = new JSONObject(commandText);
            Iterator<String> iterator = mJsonCommand.keys();
            while (iterator.hasNext()) {
                String key = iterator.next();
                mCommandKeys.add(key);
            }
            if (mCommandKeys.contains(COMMAND_IDENTIFIER)) {
                mCommandName = mJsonCommand.getString(COMMAND_IDENTIFIER);
                returnValue = true;
            }
        } catch (JSONException e) {
            logWarning(FAILED_TO_PARSE_COMMAND_JSON_PARSER, commandText);
        }
        return returnValue;
    }

    @Override
    public Set<String> argumentNames() {
        return new HashSet<>(mCommandKeys);
    }

    @Override
    public String argumentValue(String argumentName) {
        String returnValue = "";
        try {
            returnValue = mJsonCommand.getString(argumentName);
        } catch (JSONException e) {
            logException(JSONCMDPARS_FAILED_TO_PARSE, e);
        }
        return returnValue;
    }

    @Override
    public String commandName() {
        return mCommandName;
    }
}
