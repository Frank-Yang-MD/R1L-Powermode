/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import java.util.Objects;

/**
 * StartNormalAppServicesState.
 *
 * <p>Used for early apps start.
 */
public class StartNormalEfwServicesState extends VpsAbstractState {
    private static StartNormalEfwServicesState sStartNormalEfwServicesState;
    private final IJavaRunner mRunner;

    /**
     * Has to be run before any method get called
     *
     * @param stateMachine state machine
     * @param runner early apps runner
     */
    public static void initInstance(IVpsStateMachine stateMachine, IJavaRunner runner) {
        sStartNormalEfwServicesState = new StartNormalEfwServicesState(stateMachine, runner);
    }

    public static StartNormalEfwServicesState getInstance() {
        return sStartNormalEfwServicesState;
    }

    /** Start early apps */
    @Override
    public void onEnter() {
        mRunner.start();
    }

    /** When all early apps started need to change state and notify startup info on that. */
    @Override
    public void onNormalEfwServicesStarted() {
        super.transitionTo(StartNormalAppServicesState.getInstance());
    }

    private StartNormalEfwServicesState(IVpsStateMachine stateMachine, IJavaRunner runner) {
        super(
                Objects.requireNonNull(
                        stateMachine,
                        "VPS.StartNormalEfwServicesState IVpsStateMachine object is null"));
        mRunner =
                Objects.requireNonNull(
                        runner, "VPS.StartNormalEfwServicesState IJavaRunner object is null");
    }
}
