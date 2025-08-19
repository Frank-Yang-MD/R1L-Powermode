/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_ADAPTERS_START_H_EXEC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStartHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;

/**
 * StartHandler.
 *
 * <p>This class handles events by calling corresponding methods of StartHandler based on events
 * argument of IncomingEvents type.
 */
public final class StartHandler implements IEventHandler {
    private IStartHandler mIStartHandler;
    /**
     * Constructor.
     *
     * @param iStartHandler by default VpsSM implementation class.
     * @throws IllegalArgumentException if iStartHandler is null.
     */
    public StartHandler(IStartHandler iStartHandler) {
        if (null == iStartHandler) {
            throw new IllegalArgumentException(
                    "StartHandler.constructor(): IStartHandler " + "argument is NULL");
        } else {
            mIStartHandler = iStartHandler;
        }
    }
    /**
     * Calls corresponding method of mIStartHandler depending on events parameter.
     *
     * @param events event that's happening.
     * @throws IllegalArgumentException if events is null, or invalid.
     */
    @Override
    public void process(IncomingEvents events) {
        MLog.i(VPS_FUNCTION_ID, SM_ADAPTERS_START_H_EXEC);
        if (null == events) {
            throw new IllegalArgumentException(
                    "StartHandler.process(): " + "IncomingEvents argument is INVALID");
        } else {
            switch (events) {
                case NORMAL_EFW_SERVICES_STARTED:
                    mIStartHandler.onNormalEfwServicesStarted();
                    break;
                case NORMAL_APP_SERVICES_STARTED:
                    mIStartHandler.onNormalAppServicesStarted();
                    break;
                case LATE_EFW_SERVICES_STARTED:
                    mIStartHandler.onLateEfwServicesStarted();
                    break;
                case LATE_APP_SERVICES_STARTED:
                    mIStartHandler.onLateAppServicesStarted();
                    break;
                default:
                    throw new IllegalArgumentException(
                            "StartHandler.process(): IncomingEvents " + "argument is INVALID");
            }
        }
    }
}
