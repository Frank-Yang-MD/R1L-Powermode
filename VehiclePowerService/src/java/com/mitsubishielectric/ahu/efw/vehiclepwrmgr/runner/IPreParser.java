package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.util.List;

public interface IPreParser {
    /**
     * This method returns list of string containing VPS commands.
     *
     * @param config config contents
     * @return List of strings with each entry containing command in it
     */
    List<String> getLinesFromString(String config);
}
