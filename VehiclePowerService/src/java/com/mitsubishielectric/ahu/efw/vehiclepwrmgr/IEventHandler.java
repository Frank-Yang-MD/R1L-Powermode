/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;

/** Incoming event handler interface */
public interface IEventHandler {
    /**
     * Handle an event
     *
     * @param events event to handle
     */
    void process(IncomingEvents events);
}
