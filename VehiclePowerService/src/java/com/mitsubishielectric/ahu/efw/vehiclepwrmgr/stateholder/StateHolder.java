/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.stateholder;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_STATE_HOLDER_CONV_VALUE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_STATE_HOLDER_ERR_INT_PWR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_STATE_HOLDER_RCV_PWR_CHG;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_STATE_HOLDER_SEND_EVT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStateHolder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdStateChangeCallback;

/** The system power state holder */
public final class StateHolder implements IStateHolder, IVpdStateChangeCallback {

    /** Current power state */
    private EVpsState mAppState;

    /** Event receiver. It uses to notify other application components about power state change */
    private final IEventReceiver mEventReceiver;

    public StateHolder(IEventReceiver eventReceiver) {
        mAppState = EVpsState.APP_START_NA;
        mEventReceiver = eventReceiver;
    }

    @Override
    public EVpsState getAppState() {
        return mAppState;
    }

    @Override
    public void onStateChange(int stateNum) {
        try {
            mAppState = intToVpsState(stateNum);
            MLog.i(VPS_FUNCTION_ID, SM_STATE_HOLDER_RCV_PWR_CHG, mAppState.toString());
            notifyIncomingEvent();
        } catch (IllegalArgumentException e) {
            logWarning(SM_STATE_HOLDER_ERR_INT_PWR, stateNum);
        }
    }

    @Override
    public void onFwRestart() {
        mEventReceiver.onReceiveEvent(IncomingEvents.FW_RESTART);
    }

    @Override
    public void onAppRestart() {
        mEventReceiver.onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Override
    public void onFwResume() {
        mEventReceiver.onReceiveEvent(IncomingEvents.FW_RESUME);
    }

    @Override
    public void onAppResume() {
        mEventReceiver.onReceiveEvent(IncomingEvents.APP_RESUME);
    }

    @Override
    public void onFwStop() {
        mEventReceiver.onReceiveEvent(IncomingEvents.FW_STOP);
    }

    @Override
    public void onCompletionTimeout() {
        mEventReceiver.onReceiveEvent(IncomingEvents.COMPLETION_TIMEOUT);
    }

    @Override
    public void requestMount() {
        mEventReceiver.onReceiveEvent(IncomingEvents.REQUEST_MOUNT);
    }

    @Override
    public void requestUnmount() {
        mEventReceiver.onReceiveEvent(IncomingEvents.REQUEST_UNMOUNT);
    }

    private void notifyIncomingEvent() {
        IncomingEvents incomingEvent = vpsStateToIncomingEvent(mAppState);
        MLog.i(VPS_FUNCTION_ID, SM_STATE_HOLDER_SEND_EVT, mAppState.toString());
        mEventReceiver.onReceiveEvent(incomingEvent);
    }

    private IncomingEvents vpsStateToIncomingEvent(EVpsState vpsState)
            throws IllegalArgumentException {
        IncomingEvents ret = null;
        if (vpsState.equals(EVpsState.APP_START_COLDBOOT)) {
            ret = IncomingEvents.APP_START_COLDBOOT;
        } else if (vpsState.equals(EVpsState.APP_STOP)) {
            ret = IncomingEvents.APP_STOP;
        } else if (vpsState.equals(EVpsState.APP_RESUME)) {
            ret = IncomingEvents.APP_RESUME;
        } else /*if (vpsState.equals(EVpsState.APP_RESTART))*/ {
            /*
             * It is not needed to use if statement because any
             * other VpsState will not passed here. The exception
             * would be thrown in 'intToVpsState'
             */
            ret = IncomingEvents.APP_RESTART;
        }
        return ret;
    }

    private EVpsState intToVpsState(int stateNum) throws IllegalArgumentException {
        MLog.i(VPS_FUNCTION_ID, SM_STATE_HOLDER_CONV_VALUE, stateNum);
        switch (stateNum) {
            case 1:
                return EVpsState.APP_START_COLDBOOT;
            case 2:
                return EVpsState.APP_STOP;
            case 3:
                return EVpsState.APP_RESUME;
            case 4:
                return EVpsState.APP_RESTART;
        }
        throw new IllegalArgumentException();
    }
}
