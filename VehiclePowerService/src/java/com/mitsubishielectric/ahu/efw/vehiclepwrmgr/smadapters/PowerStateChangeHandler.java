/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_ADAPTERS_PWR_CHG_H_EXEC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IAppPowerStateHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;

/**
 * PowerStateChangeHandler.
 *
 * <p>This class handles events by calling corresponding methods of AppPowerStateHandler based on
 * events argument of IncomingEvents type.
 */
public final class PowerStateChangeHandler implements IEventHandler {
    private IAppPowerStateHandler mIAppPowerStateHandler;

    /**
     * Constructor.
     *
     * @param iAppPowerStateHandler by default VpsSM implementation class.
     * @throws IllegalArgumentException if iAppPowerStateHandler is null.
     */
    public PowerStateChangeHandler(IAppPowerStateHandler iAppPowerStateHandler) {
        if (null == iAppPowerStateHandler) {
            throw new IllegalArgumentException(
                    "PowerStateChangeHandler.constructor(): "
                            + "IAppPowerStateHandler argument is NULL");
        } else {
            mIAppPowerStateHandler = iAppPowerStateHandler;
        }
    }

    /**
     * Calls corresponding method of mIAppPowerStateHandler depending on events parameter.
     *
     * @param events event that's happening.
     * @throws IllegalArgumentException if events is null, or invalid.
     */
    @Override
    public void process(IncomingEvents events) {
        MLog.i(VPS_FUNCTION_ID, SM_ADAPTERS_PWR_CHG_H_EXEC);
        if (null == events) {
            throw new IllegalArgumentException(
                    "PowerStateChangeHandler.process(): " + "IncomingEvents argument is INVALID");
        } else {
            switch (events) {
                case APP_START_COLDBOOT:
                    mIAppPowerStateHandler.onAppStartColdboot();
                    break;
                case APP_RESTART:
                    mIAppPowerStateHandler.onAppRestart();
                    break;
                case APP_RESUME:
                    mIAppPowerStateHandler.onAppResume();
                    break;
                case APP_STOP:
                    mIAppPowerStateHandler.onAppStop();
                    break;
                case FW_RESTART:
                    mIAppPowerStateHandler.onFwRestart();
                    break;
                case FW_RESUME:
                    mIAppPowerStateHandler.onFwResume();
                    break;
                case FW_STOP:
                    mIAppPowerStateHandler.onFwStop();
                    break;
                default:
                    throw new IllegalArgumentException(
                            "PowerStateChangeHandler.process(): "
                                    + "IncomingEvents argument is INVALID");
            }
        }
    }
}
