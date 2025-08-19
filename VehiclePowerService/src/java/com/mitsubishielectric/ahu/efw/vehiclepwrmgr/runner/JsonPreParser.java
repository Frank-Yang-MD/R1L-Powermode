package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.FAILED_TO_PARSE_JSON_FILE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.JSON_PARSE_ERROR_CONFIG_READER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.UNKNOWN_PAR_CONF_FILE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.ServiceInfo;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Iterator;
import java.util.List;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

/**
 * This class pre-parses VPS's config in JSON format (config format version from R5). It separates
 * startup commands from startup information fields and fills in Startup Info object via
 * IInfoContainer interface.
 */
public class JsonPreParser implements IPreParser {
    private final IInfoContainer mInfoContainer;
    private final EServiceType mServiceType;

    public JsonPreParser(IInfoContainer infoContainer, EServiceType serviceType) {
        if (null == infoContainer) {
            throw new IllegalArgumentException(
                    "JsonPreParser.constructor(): " + "IInfoContainer argument is NULL");
        }
        mInfoContainer = infoContainer;
        mServiceType = serviceType;
    }

    @Override
    public List<String> getLinesFromString(String config) {
        return parseConfig(config);
    }

    private List<String> parseConfig(String rawString) {
        List<String> result = new ArrayList<>();
        try {
            JSONObject config = new JSONObject(rawString);
            JSONArray startSequence = new JSONArray();
            Iterator<String> configIterator = config.keys();
            while (configIterator.hasNext()) {
                String key = configIterator.next();
                if (key.equals("start_sequence")) {
                    startSequence = (JSONArray) config.get(key);
                } else {
                    mInfoContainer.add(key, createInfoFromJSON((JSONObject) config.get(key)));
                }
            }
            for (int i = 0; i < startSequence.length(); i++) {
                result.add(startSequence.getJSONObject(i).toString());
            }
            return result;
        } catch (JSONException e) {
            logWarning(JSON_PARSE_ERROR_CONFIG_READER, "");
            return Collections.emptyList();
        }
    }

    private ServiceInfo createInfoFromJSON(JSONObject jsonObject) {
        String className = null;
        String packageName = null;
        String esmRegistrationName = null;
        String action = null;
        boolean shouldWait = false;
        Iterator<String> iterator = jsonObject.keys();
        while (iterator.hasNext()) {
            String key = iterator.next();
            try {
                switch (key) {
                    case "cls":
                        className = (String) jsonObject.get(key);
                        break;
                    case "pkg":
                        packageName = (String) jsonObject.get(key);
                        break;
                    case "esm":
                        esmRegistrationName = (String) jsonObject.get(key);
                        break;
                    case "action":
                        action = (String) jsonObject.get(key);
                        break;
                    case "wait":
                        shouldWait = jsonObject.getBoolean(key);
                        break;
                    default:
                        logWarning(UNKNOWN_PAR_CONF_FILE, key);
                }
            } catch (JSONException e) {
                logWarning(FAILED_TO_PARSE_JSON_FILE, e.getMessage());
            }
        }
        return new ServiceInfo(
                className, packageName, esmRegistrationName, action, shouldWait, mServiceType);
    }
}
