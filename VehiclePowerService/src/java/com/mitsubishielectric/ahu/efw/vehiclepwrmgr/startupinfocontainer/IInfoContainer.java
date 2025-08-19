/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

public interface IInfoContainer {
    /**
     * Adds entry in Info container
     *
     * @param name entry name
     * @param serviceInfo entry info
     */
    void add(String name, ServiceInfo serviceInfo);

    /**
     * get service info object for particular service
     *
     * @param entryName config-related service name, String
     * @return ServiceInfo object with information about startup of corresponding service
     */
    ServiceInfo getServiceInfo(String entryName);
}
