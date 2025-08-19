package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import java.util.List;

public interface IServiceListProvider {

    /**
     * Returns list of all services started by VehiclePowerService.
     *
     * @return list of services
     */
    List<ServiceInfo> getAllStartedServicesList();
}
