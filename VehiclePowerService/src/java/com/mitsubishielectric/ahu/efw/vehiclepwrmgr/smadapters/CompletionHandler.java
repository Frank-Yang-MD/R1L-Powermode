/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.ICompletionHandler;
import java.util.Objects;

/** CompletionHandler should trigger respective completion event in ICompletionHandler object */
public final class CompletionHandler implements IEventHandler {
    private final ICompletionHandler mCompletionHandler;

    public CompletionHandler(ICompletionHandler completionHandler) {
        mCompletionHandler =
                Objects.requireNonNull(completionHandler, "VPS. ICompletionHandler can't be null");
    }

    @Override
    public void process(IncomingEvents events) {
        switch (events) {
            case EFW_SERVICES_RESTART_COMPLETE:
                mCompletionHandler.onEfwRestartCompleted();
                break;
            case EFW_SERVICES_RESUME_COMPLETE:
                mCompletionHandler.onEfwResumeCompleted();
                break;
            case EFW_APP_SERVICES_RESUME_COMPLETE:
                mCompletionHandler.onAppServicesResumeComplete();
                break;
            case EFW_APP_SERVICES_RESTART_COMPLETE:
                mCompletionHandler.onAppServicesRestartComplete();
                break;
            case EFW_SERVICES_STOP_COMPLETE:
                mCompletionHandler.onEfwServicesStopCompleted();
                break;
            case APP_SERVICES_STOP_COMPLETE:
                mCompletionHandler.onAppServicesStopCompleted();
                break;
            case COMPLETION_TIMEOUT:
                mCompletionHandler.onCompletionTimeout();
                break;
            default:
                throw new IllegalArgumentException(
                        "VPS. CompletionHandler.process(): "
                                + "IncomingEvents argument is invalid: "
                                + events);
        }
    }
}
