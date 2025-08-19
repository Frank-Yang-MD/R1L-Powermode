/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

/**
 * The interface to receive events from application components.
 *
 * <p>All VehiclePowerService components use this interface to transmit their events for processing
 */
public interface IEventReceiver {
    /**
     * Receive event
     *
     * @param evt: type of event
     */
    void onReceiveEvent(IncomingEvents evt);
}
