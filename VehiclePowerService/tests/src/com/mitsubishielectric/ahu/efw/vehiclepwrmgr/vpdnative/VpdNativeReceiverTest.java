/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmHandoffEventListener;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({
    "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeReceiver",
    "com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"
})
@PrepareForTest(VpdNativeReceiver.class)
public class VpdNativeReceiverTest {
    @Mock private IVpdStateChangeCallback mockVpdStateChangeCallback;

    @Mock private IIsmHandoffEventListener mockIsmHandoffEventListener;

    private VpdNativeReceiver vpdNativeReceiver;

    @Before
    public void setup() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.everythingDeclaredIn(MLog.class));

        MemberModifier.suppress(MemberMatcher.method(VpdNativeReceiver.class, "initNative"));

        vpdNativeReceiver =
                new VpdNativeReceiver(mockVpdStateChangeCallback, mockIsmHandoffEventListener);
    }

    @Test
    public void onVpdStateChangeTest() {
        vpdNativeReceiver.onVpdStateChange(0);
        verify(mockVpdStateChangeCallback, times(1)).onStateChange(0);
    }

    @Test
    public void onFwRestartTest() {
        vpdNativeReceiver.onFwRestart();
        verify(mockVpdStateChangeCallback, times(1)).onFwRestart();
    }

    @Test
    public void onFwResumeTest() {
        vpdNativeReceiver.onFwResume();
        verify(mockVpdStateChangeCallback, times(1)).onFwResume();
    }

    @Test
    public void onFwStopTest() {
        vpdNativeReceiver.onFwStop();
        verify(mockVpdStateChangeCallback, times(1)).onFwStop();
    }

    @Test
    public void onTimeOutErrorTest() {
        vpdNativeReceiver.onTimeOutError();
        verify(mockVpdStateChangeCallback, times(1)).onCompletionTimeout();
    }

    @Test
    public void requestMountTest() {
        vpdNativeReceiver.requestMount();
        verify(mockVpdStateChangeCallback, times(1)).requestMount();
    }

    @Test
    public void requestUnmountTest() {
        vpdNativeReceiver.requestUnmount();
        verify(mockVpdStateChangeCallback, times(1)).requestUnmount();
    }

    @Test
    public void handOffResponsibilityTest() {
        vpdNativeReceiver.handOffResponsibility();
        verify(mockIsmHandoffEventListener, times(1)).handOffResponsibility();
    }

    @Test
    public void handOffResponsibilityStartupAnimationDisplayedTest() {
        vpdNativeReceiver.handOffResponsibilityStartupAnimationDisplayed();
        verify(mockIsmHandoffEventListener, times(1))
                .handOffResponsibilityStartupAnimationDisplayed();
    }

    @Test
    public void handOffResponsibilityStartupAnimationAndDiDisplayedTest() {
        vpdNativeReceiver.handOffResponsibilityStartupAnimationAndDiDisplayed();
        verify(mockIsmHandoffEventListener, times(1))
                .handOffResponsibilityStartupAnimationAndDiDisplayed();
    }
}
