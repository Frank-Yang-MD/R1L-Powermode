/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import static junit.framework.TestCase.assertFalse;
import static junit.framework.TestCase.assertTrue;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.powermock.api.support.membermodification.MemberMatcher.method;
import static org.powermock.api.support.membermodification.MemberModifier.suppress;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
@PrepareForTest(Object.class)
public class StartupInfoEntryTest {

    @Mock private ServiceInfo mServiceInfoMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private Object mWaitMock;

    private Thread mStartCompletedSetter;

    private final int mStartCompletedSetterDelay = 10;

    private StartupInfoEntry mStartupInfoEntry;

    @Before
    public void setUp() {
        suppress(method(MLog.class, "e"));

        mStartupInfoEntry = new StartupInfoEntry(mServiceInfoMock, mVpdNativeAdapterMock);
        Whitebox.setInternalState(mStartupInfoEntry, "mWait", mWaitMock);

        mStartCompletedSetter =
                new Thread(
                        () -> {
                            try {
                                Thread.sleep(mStartCompletedSetterDelay);
                                mStartupInfoEntry.setIsStartCompleted(true);
                            } catch (InterruptedException e) {
                            }
                        });
    }

    @Test
    public void isStarted() {
        StartupInfoEntry entry = new StartupInfoEntry(mServiceInfoMock, mVpdNativeAdapterMock);
        entry.setIsStarted(true);
        Assert.assertEquals(entry.isStarted(), true);
        entry.setIsStarted(false);
        Assert.assertEquals(entry.isStarted(), false);
    }

    @Test
    public void isStartCompleted() {
        StartupInfoEntry entry = new StartupInfoEntry(mServiceInfoMock, mVpdNativeAdapterMock);
        entry.setIsStartCompleted(true);
        Assert.assertEquals(entry.isStartCompleted(), true);
        entry.setIsStartCompleted(false);
        Assert.assertEquals(entry.isStartCompleted(), false);
    }

    @Test
    public void ServiceState() {
        StartupInfoEntry entry = new StartupInfoEntry(mServiceInfoMock, mVpdNativeAdapterMock);
        Assert.assertEquals(entry.getServiceState(), EServiceState.NOT_STARTED);
        entry.setStopCompleted();
        Assert.assertEquals(entry.getServiceState(), EServiceState.NOT_STARTED);
        entry.setResumedRestarted();
        Assert.assertEquals(entry.getServiceState(), EServiceState.NOT_STARTED);
        entry.setIsStartCompleted(true);
        Assert.assertEquals(entry.getServiceState(), EServiceState.STARTED);
        entry.setStopCompleted();
        Assert.assertEquals(entry.getServiceState(), EServiceState.STOPPED);
        entry.setResumedRestarted();
        Assert.assertEquals(entry.getServiceState(), EServiceState.STARTED);
    }

    @Test
    public void waitStartCompleteIfStartNotCompleted() {
        mStartCompletedSetter.start();

        assertFalse(mStartupInfoEntry.isStartCompleted());
        mStartupInfoEntry.waitStartComplete();
        assertTrue(mStartupInfoEntry.isStartCompleted());
    }

    @Test
    public void waitStartCompleteIfStartCompleted() {
        mStartupInfoEntry.setIsStartCompleted(true);
        mStartupInfoEntry.waitStartComplete();
        verifyNoMoreInteractions(mWaitMock);
    }

    @Test
    public void waitStartCompleteWithTimeoutIfStartCompleted() {
        mStartupInfoEntry.setIsStartCompleted(true);
        assertTrue(mStartupInfoEntry.waitStartComplete(1));
        verifyNoMoreInteractions(mWaitMock);
    }

    @Test
    public void waitStartCompleteWithTimeoutExpired() {
        mStartCompletedSetter.start();
        assertFalse(mStartupInfoEntry.waitStartComplete(1));
    }

    @Test
    public void waitStartCompleteWithTimeout() {
        mStartCompletedSetter.start();

        assertFalse(mStartupInfoEntry.isStartCompleted());
        assertTrue(mStartupInfoEntry.waitStartComplete(mStartCompletedSetterDelay * 10));
        assertTrue(mStartupInfoEntry.isStartCompleted());
    }
}
