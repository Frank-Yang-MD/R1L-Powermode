/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IAppPowerStateHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class PowerStateChangeHandlerTest {
    @Mock private IAppPowerStateHandler mIAppPowerStateHandler;

    @InjectMocks private PowerStateChangeHandler mPowerStateChangeHandler;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    /** Test for handling null argument in constructor. */
    @Test(expected = IllegalArgumentException.class)
    public void constructor() {
        PowerStateChangeHandler localPowerStateChangeHandler = new PowerStateChangeHandler(null);
    }

    @Test
    public void processAppStartColdboot() {
        mPowerStateChangeHandler.process(IncomingEvents.APP_START_COLDBOOT);
        verify(mIAppPowerStateHandler, times(1)).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processAppResume() {
        mPowerStateChangeHandler.process(IncomingEvents.APP_RESUME);
        verify(mIAppPowerStateHandler, times(1)).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processAppStop() {
        mPowerStateChangeHandler.process(IncomingEvents.APP_STOP);
        verify(mIAppPowerStateHandler, times(1)).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processAppRestart() {
        mPowerStateChangeHandler.process(IncomingEvents.APP_RESTART);
        verify(mIAppPowerStateHandler, times(1)).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processFwRestart() {
        mPowerStateChangeHandler.process(IncomingEvents.FW_RESTART);
        verify(mIAppPowerStateHandler, times(1)).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processFwResume() {
        mPowerStateChangeHandler.process(IncomingEvents.FW_RESUME);
        verify(mIAppPowerStateHandler, times(1)).onFwResume();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }

    @Test
    public void processFwStop() {
        mPowerStateChangeHandler.process(IncomingEvents.FW_STOP);
        verify(mIAppPowerStateHandler, times(1)).onFwStop();
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
    }

    /** Test for properly null argument handling */
    @Test(expected = IllegalArgumentException.class)
    public void processNullArg() {
        try {
            mPowerStateChangeHandler.process(null);
        } finally {
            // For case if exception throws after method calling
            verify(mIAppPowerStateHandler, never()).onAppRestart();
            verify(mIAppPowerStateHandler, never()).onAppResume();
            verify(mIAppPowerStateHandler, never()).onAppStop();
            verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
            verify(mIAppPowerStateHandler, never()).onFwRestart();
            verify(mIAppPowerStateHandler, never()).onFwResume();
            verify(mIAppPowerStateHandler, never()).onFwStop();
        }
    }

    /** Test for properly wrong argument case handling */
    @Test(expected = IllegalArgumentException.class)
    public void processInvalidArg() throws IllegalArgumentException {
        // Passing an argument that is unhandled in the switch
        mPowerStateChangeHandler.process(IncomingEvents.COMPLETION_TIMEOUT);
        // For case if exception throws after method calling
        verify(mIAppPowerStateHandler, never()).onAppRestart();
        verify(mIAppPowerStateHandler, never()).onAppResume();
        verify(mIAppPowerStateHandler, never()).onAppStop();
        verify(mIAppPowerStateHandler, never()).onAppStartColdboot();
        verify(mIAppPowerStateHandler, never()).onFwRestart();
        verify(mIAppPowerStateHandler, never()).onFwResume();
        verify(mIAppPowerStateHandler, never()).onFwStop();
    }
}
