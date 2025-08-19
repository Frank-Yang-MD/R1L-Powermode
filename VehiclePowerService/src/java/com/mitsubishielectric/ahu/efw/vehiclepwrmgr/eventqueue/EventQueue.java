/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_Q_INC_EVT_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_Q_INC_EVT_GET;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_Q_REC_EVT_ADD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_Q_REC_EVT_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventSource;
import java.util.Objects;
import java.util.concurrent.BlockingQueue;

/**
 * EventQueue is simple wrapper around standard BlockingQueue. EventQueue implements IEventReceiver
 * interface to receive the events from other components and implements IEventSource interface to
 * provide the ability to receive events from the queue
 */
public final class EventQueue implements IEventReceiver, IEventSource {
    private final BlockingQueue<IncomingEvents> mQueue;

    public EventQueue(BlockingQueue<IncomingEvents> queue) {
        mQueue = Objects.requireNonNull(queue, "EventQueue() incoming queue should be not null");
    }

    @Override
    public void onReceiveEvent(IncomingEvents event) {
        try {
            MLog.i(VPS_FUNCTION_ID, EVT_Q_Q_REC_EVT_ADD, event.name());

            mQueue.put(event);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            logWarning(EVT_Q_Q_REC_EVT_FAIL, e.getMessage());
        }
    }

    @Override
    public IncomingEvents getEvent() {
        try {
            IncomingEvents event = mQueue.take();
            MLog.i(VPS_FUNCTION_ID, EVT_Q_Q_INC_EVT_GET, event.name());
            return event;
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
            logWarning(EVT_Q_Q_INC_EVT_FAIL, e.getMessage());
            return null;
        }
    }
}
