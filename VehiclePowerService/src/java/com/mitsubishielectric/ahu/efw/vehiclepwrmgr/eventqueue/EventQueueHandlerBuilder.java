/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventSource;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.CompletionHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.MountEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.PowerStateChangeHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.StartHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsStateMachine;
import java.util.HashMap;
import java.util.concurrent.Executors;

/** Build EventQueueHandler object */
public final class EventQueueHandlerBuilder {
    private final IEventSource mEventSource;
    private final VpsStateMachine mStateMachine;
    private final MountEventHandler mMountEventHandler;

    public EventQueueHandlerBuilder(
            IEventSource mEventSource,
            VpsStateMachine mStateMachine,
            MountEventHandler mountEventHandler) {
        this.mEventSource = mEventSource;
        this.mStateMachine = mStateMachine;
        this.mMountEventHandler = mountEventHandler;
    }

    public EventQueueHandler build() {
        EventQueueHandler eventQueueHandler =
                new EventQueueHandler(
                        mEventSource, Executors.newSingleThreadExecutor(), new HashMap<>());
        addPwrStateChangeHandler(eventQueueHandler);
        addStartHandler(eventQueueHandler);
        addCompletionHandler(eventQueueHandler);
        addVpdEventHandler(eventQueueHandler);
        return eventQueueHandler;
    }

    private void addStartHandler(EventQueueHandler eventQueueHandler) {
        StartHandler startHandler = new StartHandler(mStateMachine);
        eventQueueHandler.addEventHandler(IncomingEvents.LATE_APP_SERVICES_STARTED, startHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.LATE_EFW_SERVICES_STARTED, startHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.NORMAL_EFW_SERVICES_STARTED, startHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.NORMAL_APP_SERVICES_STARTED, startHandler);
    }

    private void addPwrStateChangeHandler(EventQueueHandler eventQueueHandler) {
        PowerStateChangeHandler powerStateChangeHandler =
                new PowerStateChangeHandler(mStateMachine);
        eventQueueHandler.addEventHandler(
                IncomingEvents.APP_START_COLDBOOT, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.APP_RESTART, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.APP_RESUME, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.APP_STOP, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.FW_RESTART, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.FW_RESUME, powerStateChangeHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.FW_STOP, powerStateChangeHandler);
    }

    private void addCompletionHandler(EventQueueHandler eventQueueHandler) {
        final CompletionHandler completionHandler = new CompletionHandler(mStateMachine);
        eventQueueHandler.addEventHandler(
                IncomingEvents.EFW_SERVICES_RESTART_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(
                IncomingEvents.EFW_SERVICES_RESUME_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(
                IncomingEvents.EFW_SERVICES_STOP_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(
                IncomingEvents.EFW_APP_SERVICES_RESTART_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(
                IncomingEvents.EFW_APP_SERVICES_RESUME_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(
                IncomingEvents.APP_SERVICES_STOP_COMPLETE, completionHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.COMPLETION_TIMEOUT, completionHandler);
    }

    private void addVpdEventHandler(EventQueueHandler eventQueueHandler) {
        eventQueueHandler.addEventHandler(IncomingEvents.REQUEST_UNMOUNT, mMountEventHandler);
        eventQueueHandler.addEventHandler(IncomingEvents.REQUEST_MOUNT, mMountEventHandler);
    }
}
