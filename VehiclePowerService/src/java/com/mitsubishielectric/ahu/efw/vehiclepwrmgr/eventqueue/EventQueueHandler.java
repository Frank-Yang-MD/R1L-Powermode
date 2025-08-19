/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_ADD_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_ALREADY_RUN;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_ALREADY_STOP;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_NULL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_REPL_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_START;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EVT_Q_H_STOP;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventSource;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * EventQueueHandler reads events from the event queue using IEventSource interface and passes the
 * event to the appropriate handler
 */
public final class EventQueueHandler {
    /** Event source */
    private final IEventSource mEventSource;

    /** Map of handlers */
    private final Map<IncomingEvents, IEventHandler> mHandlers;

    /** Stop thread flag */
    private final AtomicBoolean mStopFlag = new AtomicBoolean(true);

    /** Executor */
    private final ExecutorService mExecutiorService;

    private CompletableFuture<Void> mCompletableFuture;

    EventQueueHandler(
            IEventSource eventSource,
            ExecutorService executorService,
            Map<IncomingEvents, IEventHandler> handlers) {
        mExecutiorService = executorService;
        mEventSource = eventSource;

        mHandlers = handlers;
    }

    /**
     * Add event handler for specified event type
     *
     * @param event event type
     * @param handler event handler
     */
    public void addEventHandler(IncomingEvents event, IEventHandler handler) {
        MLog.i(VPS_FUNCTION_ID, EVT_Q_H_ADD_EVT, event.name());
        if (mHandlers.containsKey(event)) {
            logWarning(EVT_Q_H_REPL_EVT, event.name());
        }
        mHandlers.put(event, handler);
    }

    /**
     * Start working thread to handle incoming events
     *
     * @return false if working thread already running
     */
    public boolean start() {
        MLog.i(VPS_FUNCTION_ID, EVT_Q_H_START);
        if (mCompletableFuture != null) {
            logWarning(EVT_Q_H_ALREADY_RUN);
            return false;
        }
        mStopFlag.set(false);
        mCompletableFuture = CompletableFuture.runAsync(this::processEvent, mExecutiorService);
        return true;
    }

    /** Stop working thread */
    public void stop() {
        if (mStopFlag.get()) {
            MLog.i(VPS_FUNCTION_ID, EVT_Q_H_ALREADY_STOP);
            return;
        }
        MLog.i(VPS_FUNCTION_ID, EVT_Q_H_STOP);
        mStopFlag.set(true);
        mExecutiorService.shutdownNow();
        mCompletableFuture.join();
        mCompletableFuture = null;
    }

    private void processEvent() {
        IncomingEvents event = mEventSource.getEvent();
        handleEvent(event);
        if (!stopCondition()) {
            mCompletableFuture = CompletableFuture.runAsync(this::processEvent, mExecutiorService);
        }
    }

    /**
     * Check if thread should be finished
     *
     * @return true if thread should be stopped
     */
    private boolean stopCondition() {
        return mStopFlag.get() || Thread.currentThread().isInterrupted();
    }

    /**
     * Handle an event
     *
     * @param event incoming event
     */
    private void handleEvent(IncomingEvents event) {
        if (event == null) {
            logWarning(EVT_Q_H_NULL);
            return;
        }

        IEventHandler handler = mHandlers.get(event);
        if (handler != null) {
            handler.process(event);
        } else {
            logWarning(VPS_FUNCTION_ID, VPS_FUNCTION_ID, EVT_Q_H_FAIL, event.name());
        }
    }
}
