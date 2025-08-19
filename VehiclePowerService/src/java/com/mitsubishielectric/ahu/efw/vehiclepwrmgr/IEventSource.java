/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;

public interface IEventSource {
    /**
     * Events getter
     *
     * @return IncomingEvents object in case of success, or null in case of any failure.
     */
    IncomingEvents getEvent();
}
