/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism;

import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.os.RemoteException;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EBootDaemonStatus;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EScreenProgress;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentMatchers;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class IsmSubscriptionTest {
    @Mock private IBootDaemonStatusListener mBootDaemonStatusListenerMock;

    IsmSubscription mIsmSubscription;

    @Before
    public void setUp() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mIsmSubscription = new IsmSubscription();
        // mBootDaemonStatus = EBootDaemonStatus.ACTIVE;
        // mScreenProgress = EScreenProgress.UNKNOWN;
    }

    @Test
    public void subscribeIsm() throws RemoteException {
        mIsmSubscription.subscribeIsm(mBootDaemonStatusListenerMock);
        verify(mBootDaemonStatusListenerMock, times(1))
                .notifyBootDaemonStatus(
                        ArgumentMatchers.any(EBootDaemonStatus.class),
                        ArgumentMatchers.any(EScreenProgress.class));
    }

    @Test
    public void subscribeIsmNullListener() throws RemoteException {
        mIsmSubscription.subscribeIsm(null);
        verify(mBootDaemonStatusListenerMock, never())
                .notifyBootDaemonStatus(ArgumentMatchers.any(), ArgumentMatchers.any());
    }

    @Test
    public void handOffResponsibility() throws RemoteException {
        mIsmSubscription.subscribeIsm(mBootDaemonStatusListenerMock);

        mIsmSubscription.handOffResponsibility();
        verify(mBootDaemonStatusListenerMock, times(1))
                .notifyBootDaemonStatus(EBootDaemonStatus.FINISH, EScreenProgress.BLACK_SCREEN);
    }

    @Test
    public void handOffResponsibilityNullListener() throws RemoteException {
        mIsmSubscription.handOffResponsibility();
        verify(mBootDaemonStatusListenerMock, times(0))
                .notifyBootDaemonStatus(ArgumentMatchers.any(), ArgumentMatchers.any());
    }

    @Test
    public void handOffResponsibilityStartupAnimationDisplayed() throws RemoteException {
        mIsmSubscription.subscribeIsm(mBootDaemonStatusListenerMock);

        mIsmSubscription.handOffResponsibilityStartupAnimationDisplayed();
        verify(mBootDaemonStatusListenerMock, times(1))
                .notifyBootDaemonStatus(
                        EBootDaemonStatus.FINISH, EScreenProgress.STARTUP_ANIMATION);
    }

    @Test
    public void handOffResponsibilityStartupAnimationDisplayedNullListener()
            throws RemoteException {
        mIsmSubscription.handOffResponsibilityStartupAnimationDisplayed();
        verify(mBootDaemonStatusListenerMock, times(0))
                .notifyBootDaemonStatus(ArgumentMatchers.any(), ArgumentMatchers.any());
    }

    @Test
    public void handOffResponsibilityStartupAnimationAndDiDisplayed() throws RemoteException {
        mIsmSubscription.subscribeIsm(mBootDaemonStatusListenerMock);

        mIsmSubscription.handOffResponsibilityStartupAnimationAndDiDisplayed();
        verify(mBootDaemonStatusListenerMock, times(1))
                .notifyBootDaemonStatus(EBootDaemonStatus.FINISH, EScreenProgress.DISCLAIMER);
    }

    @Test
    public void handOffResponsibilityStartupAnimationAndDiDisplayedNullListener()
            throws RemoteException {
        mIsmSubscription.handOffResponsibilityStartupAnimationAndDiDisplayed();
        verify(mBootDaemonStatusListenerMock, times(0))
                .notifyBootDaemonStatus(ArgumentMatchers.any(), ArgumentMatchers.any());
    }
}
