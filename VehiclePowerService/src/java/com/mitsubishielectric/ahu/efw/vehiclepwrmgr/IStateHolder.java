/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;

/** Interface to getting system power state */
public interface IStateHolder {
    /**
     * Get power state
     *
     * @return current power state or {@link EVpsState#APP_START_NA} if current power state is not
     *     defined
     */
    EVpsState getAppState();
}
