/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.content.pm.PackageManager;
import android.os.RemoteException;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.CompleteNotificationCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmSubscription;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class VehiclePowerServiceAdapterTest {
    @Mock private IEventReceiver mEventReceiver;
    @Mock private ISubscriptionManager mAppSubscriptionManager;
    @Mock private ISubscriptionManager mServiceSubscriptionManager;
    @Mock private IVehiclePowerServiceListener mListener;
    @Mock private IStateHolder mIStateHolder;
    @Mock private Context mContext;
    @Mock private IStartupInfo mIStartupInfo;
    @Mock PackageManager mPackageManager;
    @Mock CompleteNotificationCounter mStopCompleteAppHandler;
    @Mock CompleteNotificationCounter mStopCompleteFwHandler;
    @Mock CompleteNotificationCounter mResumeCompleteAppHandler;
    @Mock CompleteNotificationCounter mResumeCompleteFwHandler;
    @Mock CompleteNotificationCounter mRestartCompleteAppHandler;
    @Mock CompleteNotificationCounter mRestartCompleteFwHandler;
    @Mock IIsmSubscription mIsmSubscription;
    @Mock IVpdNativeAdapter mVpdNativeAdapter;

    private VehiclePowerServiceAdapter mVehiclePowerServiceAdapter;

    @Before
    public void before() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("Unit_test");
        when(mContext.getPackageManager()).thenReturn(mPackageManager);
        mVehiclePowerServiceAdapter =
                new VehiclePowerServiceAdapter(
                        mEventReceiver,
                        mAppSubscriptionManager,
                        mServiceSubscriptionManager,
                        mIStateHolder,
                        mContext,
                        mIStartupInfo,
                        mStopCompleteAppHandler,
                        mStopCompleteFwHandler,
                        mResumeCompleteFwHandler,
                        mRestartCompleteFwHandler,
                        mResumeCompleteAppHandler,
                        mRestartCompleteAppHandler,
                        mIsmSubscription,
                        mVpdNativeAdapter);
    }

    @Test
    public void onCreateInstance() {
        Assert.assertNotNull(mVehiclePowerServiceAdapter);
    }

    @Test
    public void onSubscribeApp() {
        mVehiclePowerServiceAdapter.subscribeApp(mListener, "classname");
        verify(mAppSubscriptionManager, times(1)).subscribe(mListener, "classname");
    }

    @Test
    public void onSubscribeService() {
        mVehiclePowerServiceAdapter.subscribeFWService(mListener, "classname");
        verify(mServiceSubscriptionManager, times(1)).subscribe(mListener, "classname");
    }

    @Test
    public void onUnsubscribe() {
        mVehiclePowerServiceAdapter.unsubscribe(mListener, "classname");

        verify(mAppSubscriptionManager, times(1)).unsubscribe(mListener, "classname");
        verify(mServiceSubscriptionManager, times(1)).unsubscribe(mListener, "classname");
    }

    @Test
    public void onStopCompleteApp() {
        mVehiclePowerServiceAdapter.stopCompleteApp("class_name");

        verify(mStopCompleteAppHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onStopComplete("class_name");
    }

    @Test
    public void onStopCompleteFw() {
        mVehiclePowerServiceAdapter.stopCompleteEfw("class_name");

        verify(mStopCompleteFwHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onStopComplete("class_name");
    }

    @Test
    public void isServiceStartedTest() throws RemoteException {
        mVehiclePowerServiceAdapter.isServiceStarted("class_name");
        verify(mIStartupInfo, times(1)).isServiceStarted("class_name");
    }

    @Test
    public void startCompleteTest() throws RemoteException {
        mVehiclePowerServiceAdapter.startComplete("class_name");
        verify(mIStartupInfo, times(1)).onServiceStartupCompleted("class_name");
    }

    @Test
    public void isAllEfwServicesStartedTest() throws RemoteException {
        mVehiclePowerServiceAdapter.isAllEfwServicesStarted();
        verify(mIStartupInfo, times(1)).isAllServicesStarted();
    }

    @Test
    public void normalAppServicesStartedTest() throws RemoteException {
        mVehiclePowerServiceAdapter.normalAppServicesStarted();
        verify(mIStartupInfo, times(1)).isEarlyAppsStarted();
    }

    @Test
    public void isAllAppServicesStartedTest() throws RemoteException {
        mVehiclePowerServiceAdapter.isAllAppServicesStarted();
        verify(mIStartupInfo, times(1)).isAllAppsStarted();
    }

    @Test
    public void getNotifiedEventTest() {
        mVehiclePowerServiceAdapter.getNotifiedEvent();
        verify(mIStateHolder, times(1)).getAppState();
    }

    @Test
    public void resumeCompleteEfwTest() {
        mVehiclePowerServiceAdapter.resumeCompleteEfw("class_name");

        verify(mResumeCompleteFwHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onRestartResumeComplete("class_name");
    }

    @Test
    public void restartCompleteEfwTest() {
        mVehiclePowerServiceAdapter.restartCompleteEfw("class_name");

        verify(mRestartCompleteFwHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onRestartResumeComplete("class_name");
    }

    @Test
    public void resumeCompleteApp() {
        mVehiclePowerServiceAdapter.resumeCompleteApp("class_name");

        verify(mResumeCompleteAppHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onRestartResumeComplete("class_name");
    }

    @Test
    public void restartCompleteApp() {
        mVehiclePowerServiceAdapter.restartCompleteApp("class_name");

        verify(mRestartCompleteAppHandler, times(1)).onComplete("class_name");
        verify(mIStartupInfo, times(1)).onRestartResumeComplete("class_name");
    }

    @Test
    public void getServiceState() {
        mVehiclePowerServiceAdapter.getServiceState("class_name");
        verify(mIStartupInfo, times(1)).getServiceState("class_name");
    }

    @Test
    public void ismReady() {
        IBootDaemonStatusListener bootDaemonStatusListener = mock(IBootDaemonStatusListener.class);

        mVehiclePowerServiceAdapter.ismReady(bootDaemonStatusListener);
        verify(mIsmSubscription, times(1)).subscribeIsm(bootDaemonStatusListener);
    }

    @Test
    public void requestEndEarlyHmi() {
        mVehiclePowerServiceAdapter.requestEndEarlyHmi();
        verify(mVpdNativeAdapter, times(1)).endEarlyHmi();
    }

    @Test
    public void forcedCompletionEarlyServices() {
        mVehiclePowerServiceAdapter.forcedCompletionEarlyServices();
        verify(mVpdNativeAdapter, times(1)).forcedCompletionEarlyServices();
    }

    @Test
    public void isNormalEfwServicesStartCompleteTest() {
        mVehiclePowerServiceAdapter.isNormalEfwServicesStartComplete();
        verify(mIStartupInfo, times(1)).isNormalEfwServicesStartComplete();
    }

    @Test
    public void isNormalAppServicesStartCompleteTest() {
        mVehiclePowerServiceAdapter.isNormalAppServicesStartComplete();
        verify(mIStartupInfo, times(1)).isNormalAppServicesStartComplete();
    }

    @Test
    public void isLateEfwServicesStartCompleteTest() {
        mVehiclePowerServiceAdapter.isLateEfwServicesStartComplete();
        verify(mIStartupInfo, times(1)).isLateEfwServicesStartComplete();
    }

    @Test
    public void isLateAppServicesStartCompleteTest() {
        mVehiclePowerServiceAdapter.isLateAppServicesStartComplete();
        verify(mIStartupInfo, times(1)).isLateAppServicesStartComplete();
    }
}
