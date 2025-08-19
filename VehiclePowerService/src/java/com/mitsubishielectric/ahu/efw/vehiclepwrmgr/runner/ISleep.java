/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

/** Sleep interface */
public interface ISleep {
    /**
     * Sleep execution
     *
     * @param sleepTimeNano sleep time in nanoseconds
     */
    void sleep(long sleepTimeNano);
}
