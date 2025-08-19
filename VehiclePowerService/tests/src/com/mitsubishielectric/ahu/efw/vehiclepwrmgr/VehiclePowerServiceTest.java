package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_REBOOT_ACTION_INTENT;
import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_SERVICE;
import static org.junit.Assert.assertNull;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;
import static org.powermock.api.mockito.PowerMockito.verifyNew;
import static org.powermock.api.mockito.PowerMockito.whenNew;
import static org.powermock.api.support.membermodification.MemberMatcher.method;
import static org.powermock.api.support.membermodification.MemberModifier.replace;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.extendedservicemanager.ExtSrvManager;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VehicleConfigManager;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.CompleteNotificationCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueue;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueueHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueueHandlerBuilder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner.ExtSrvManagerRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement.MountManagement;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.JavaRunnerCreator;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigReader;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigReaderWithFallback;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.ServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.StartupInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.stateholder.StateHolder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.StateInitializer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager.SubscriptionManager;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeAdapter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeReceiver;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.stubbing.Answer;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({
    "com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog",
    "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.bootdnative.BootDaemonNativeReceiver",
    "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeReceiver",
    "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VehiclePowerService"
})
@PrepareForTest({
    VehiclePowerService.class,
    ExtSrvManager.class,
    System.class,
    VPSLogMsg.class,
    Service.class,
    EventQueueHandlerBuilder.class
})
public class VehiclePowerServiceTest {
    @Mock private EventQueue mEventQueueMock;

    @Mock private EventQueueHandler mEventQueueHandlerMock;

    @Mock private StateHolder mStateHolderMock;

    @Mock private VpdNativeReceiver mVpdNativeReceiverMock;

    @Mock private VpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private VpsStateMachine mVpsStateMachineMock;

    @Mock private CompleteNotificationCounter mAppStopCompleteCounterMock;

    @Mock private CompleteNotificationCounter mEfwStopCompleteCounterMock;

    @Mock private CompleteNotificationCounter mFwResumeCompleteCounterMock;

    @Mock private CompleteNotificationCounter mFwRestartCompleteCounterMock;

    @Mock private CompleteNotificationCounter mAppResumeCompleteCounterMock;

    @Mock private CompleteNotificationCounter mAppRestartCompleteCounterMock;

    @Mock private StartupInfoContainer mStartupInfoContainerMock;

    @Mock private ExtSrvManagerRunner mExtSrvManagerRunnerMock;

    @Mock private Context mContextMock;

    @Mock private Intent mIntentMock;

    @Mock private IntentFilter mIntentFilterMock;

    @Mock private IBinder mEsmBinderMock;

    @Mock private ExtSrvManager mExtSrvManagerMock;

    @Mock private JavaRunnerCreator mJavaRunnerCreatorMock;

    @Mock private StateInitializer mStateInitializerMock;

    @Mock private VehicleConfigReader mVehicleConfigReaderMock;

    @Mock private VehicleConfigReaderWithFallback mVehicleConfigReaderWithFallbackMock;

    @Mock private VehicleConfigRunner mVehicleConfigRunnerMock;

    @Mock private IJavaRunner mLateEfwServicesRunnerMock;

    @Mock private IJavaRunner mNormalEfwServicesRunnerMock;

    @Mock private IJavaRunner mLateAppServicesRunnerMock;

    @Mock private IJavaRunner mNormalAppServicesRunnerMock;

    @Mock VehicleConfigManager mVehicleConfigManagerMock;

    @Mock ServicesRunningChecker mServiceRunningCheckerMock;

    @Mock Bundle mBundleMock;

    @Mock MountManagement mMountManagementMock;

    VehiclePowerService mVehiclePowerServiceSpy;

    @Before
    public void setUp() throws Exception {
        PowerMockito.mockStatic(VPSLogMsg.class);
        PowerMockito.mockStatic(VpdNativeReceiver.class);
        PowerMockito.mockStatic(VehiclePowerService.class);

        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.everythingDeclaredIn(MLog.class));

        replace(method(EventQueueHandlerBuilder.class, "build"))
                .with((o, method, objects) -> mEventQueueHandlerMock);

        whenNew(EventQueue.class).withAnyArguments().thenReturn(mEventQueueMock);
        whenNew(StateHolder.class).withAnyArguments().thenReturn(mStateHolderMock);
        whenNew(VpdNativeAdapter.class).withNoArguments().thenReturn(mVpdNativeAdapterMock);
        whenNew(VpsStateMachine.class).withNoArguments().thenReturn(mVpsStateMachineMock);
        whenNew(StartupInfoContainer.class).withNoArguments().thenReturn(mStartupInfoContainerMock);

        // CompleteNotificationCounters creating mock
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.APP_SERVICES_STOP_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mAppStopCompleteCounterMock);
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.EFW_SERVICES_STOP_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mEfwStopCompleteCounterMock);
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.EFW_SERVICES_RESUME_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mFwResumeCompleteCounterMock);
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.EFW_SERVICES_RESTART_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mFwRestartCompleteCounterMock);
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.EFW_APP_SERVICES_RESUME_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mAppResumeCompleteCounterMock);
        whenNew(CompleteNotificationCounter.class)
                .withArguments(
                        mEventQueueMock,
                        IncomingEvents.EFW_APP_SERVICES_RESTART_COMPLETE,
                        mStartupInfoContainerMock)
                .thenReturn(mAppRestartCompleteCounterMock);

        whenNew(ExtSrvManagerRunner.class).withAnyArguments().thenReturn(mExtSrvManagerRunnerMock);
        whenNew(IntentFilter.class)
                .withArguments(ESM_REBOOT_ACTION_INTENT)
                .thenReturn(mIntentFilterMock);

        whenNew(VpdNativeReceiver.class).withAnyArguments().thenReturn(mVpdNativeReceiverMock);
        whenNew(JavaRunnerCreator.class).withAnyArguments().thenReturn(mJavaRunnerCreatorMock);
        whenNew(StateInitializer.class).withNoArguments().thenReturn(mStateInitializerMock);

        whenNew(VehicleConfigReader.class).withAnyArguments().thenReturn(mVehicleConfigReaderMock);

        whenNew(VehicleConfigRunner.class).withAnyArguments().thenReturn(mVehicleConfigRunnerMock);
        when(mVehicleConfigRunnerMock.getReadyVehicleConfigManager())
                .thenReturn(mVehicleConfigManagerMock);

        when(VehiclePowerService.class.getName()).thenAnswer((Answer<String>) arg -> "Vps_name");

        whenNew(MountManagement.class).withAnyArguments().thenReturn(mMountManagementMock);

        whenNew(VehicleConfigReaderWithFallback.class)
                .withAnyArguments()
                .thenReturn(mVehicleConfigReaderWithFallbackMock);

        mVehiclePowerServiceSpy = Mockito.spy(new VehiclePowerService());
        when(mVehiclePowerServiceSpy.getApplicationContext()).thenReturn(mContextMock);
    }

    @Test
    public void onCreateTest() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(VehiclePowerService.class, "initNative"));

        mVehiclePowerServiceSpy.onCreate();

        verify(mExtSrvManagerRunnerMock).run();
        verifyNew(IntentFilter.class).withArguments(ESM_REBOOT_ACTION_INTENT);
    }

    @Test
    public void onBindTest() {
        assertNull(mVehiclePowerServiceSpy.onBind(mIntentMock));
    }

    @Test
    public void onDestroyTest() {
        MemberModifier.suppress(MemberMatcher.method(VehiclePowerService.class, "cleanupNative"));

        mVehiclePowerServiceSpy.onDestroy();
    }

    @Test
    public void notifyExtSrvMgrBoundWithCorrectExtSrvManager() throws Exception {
        replace(method(ExtSrvManager.class, "getInstance"))
                .with((o, method, objects) -> mExtSrvManagerMock);

        // avoid onCreate() call and creating new instance of ServiceRunningChecker
        Whitebox.setInternalState(
                mVehiclePowerServiceSpy, "mServicesRunningChecker", mServiceRunningCheckerMock);

        // runners creation
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.LATE_EFW_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mLateEfwServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.NORMAL_EFW_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mNormalEfwServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.LATE_APP_SERVICES_STARTED,
                        mVehicleConfigReaderWithFallbackMock))
                .thenReturn(mLateAppServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.NORMAL_APP_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mNormalAppServicesRunnerMock);

        // create dependency between use()-method call
        mockMethodsInStateInitializer();

        mVehiclePowerServiceSpy.notifyExtSrvMgrBound(mEsmBinderMock);

        // registerInEsm() method execution verification
        verify(mExtSrvManagerMock).addService(anyString(), any(VehiclePowerServiceAdapter.class));

        // createRunners() method execution verification
        verifyNew(VehicleConfigRunner.class)
                .withArguments(any(Service.class), eq(mEsmBinderMock), eq(mExtSrvManagerMock));

        verify(mJavaRunnerCreatorMock)
                .create(IncomingEvents.LATE_EFW_SERVICES_STARTED, mVehicleConfigReaderMock);
        verify(mJavaRunnerCreatorMock)
                .create(IncomingEvents.NORMAL_EFW_SERVICES_STARTED, mVehicleConfigReaderMock);
        verify(mJavaRunnerCreatorMock)
                .create(
                        IncomingEvents.LATE_APP_SERVICES_STARTED,
                        mVehicleConfigReaderWithFallbackMock);
        verify(mJavaRunnerCreatorMock)
                .create(IncomingEvents.NORMAL_APP_SERVICES_STARTED, mVehicleConfigReaderMock);

        // initStates() method execution verification
        verifyNew(StateInitializer.class).withNoArguments();
        verify(mStateInitializerMock).use(mVehiclePowerServiceSpy);
        verify(mStateInitializerMock).use(any(VpsStateMachine.class));
        verify(mStateInitializerMock)
                .use(
                        mLateEfwServicesRunnerMock,
                        mNormalEfwServicesRunnerMock,
                        mLateAppServicesRunnerMock,
                        mNormalAppServicesRunnerMock);
        verify(mStateInitializerMock).use(mVpdNativeAdapterMock);
        verify(mStateInitializerMock)
                .use(any(SubscriptionManager.class), any(SubscriptionManager.class));
        verify(mStateInitializerMock)
                .use(
                        mAppStopCompleteCounterMock,
                        mEfwStopCompleteCounterMock,
                        mFwResumeCompleteCounterMock,
                        mFwRestartCompleteCounterMock,
                        mAppResumeCompleteCounterMock,
                        mAppRestartCompleteCounterMock);
        verify(mStateInitializerMock).use(mStartupInfoContainerMock);
        verify(mStateInitializerMock).use(any(ServicesRunningChecker.class));
        verify(mStateInitializerMock).initStates();

        verify(mEventQueueHandlerMock).start();
    }

    @Test(expected = IllegalArgumentException.class)
    public void notifyExtSrvMgrBoundWithNullBinder() throws Exception {
        replace(method(ExtSrvManager.class, "getInstance"))
                .with((o, method, objects) -> mExtSrvManagerMock);
        whenNew(StateInitializer.class).withNoArguments().thenReturn(mStateInitializerMock);
        mockMethodsInStateInitializer();

        mVehiclePowerServiceSpy.notifyExtSrvMgrBound(null);

        verify(mExtSrvManagerMock, times(0))
                .addService(anyString(), any(VehiclePowerServiceAdapter.class));
        verifyZeroInteractions(mJavaRunnerCreatorMock);
    }

    @Test
    public void notifyExtSrvMgrBoundWithNullExtSrvManager() throws Exception {
        replace(method(ExtSrvManager.class, "getInstance")).with((o, method, objects) -> null);

        // avoid onCreate() call and creating new instance of ServiceRunningChecker
        Whitebox.setInternalState(
                mVehiclePowerServiceSpy, "mServicesRunningChecker", mServiceRunningCheckerMock);

        // runners creation
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.LATE_EFW_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mLateEfwServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.NORMAL_EFW_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mNormalEfwServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.LATE_APP_SERVICES_STARTED,
                        mVehicleConfigReaderWithFallbackMock))
                .thenReturn(mLateAppServicesRunnerMock);
        when(mJavaRunnerCreatorMock.create(
                        IncomingEvents.NORMAL_APP_SERVICES_STARTED, mVehicleConfigReaderMock))
                .thenReturn(mNormalAppServicesRunnerMock);

        mockMethodsInStateInitializer();

        mVehiclePowerServiceSpy.notifyExtSrvMgrBound(mEsmBinderMock);

        verifyZeroInteractions(mExtSrvManagerMock);
    }

    @Test
    public void onReceiveWithCorrectBinder() {
        replace(method(ExtSrvManager.class, "getInstance"))
                .with((o, method, objects) -> mExtSrvManagerMock);
        when(mIntentMock.getExtras()).thenReturn(mBundleMock);
        when(mBundleMock.getBinder(ESM_SERVICE)).thenReturn(mEsmBinderMock);
        when(mEsmBinderMock.toString()).thenReturn("EsmBinder");

        BroadcastReceiver receiver =
                Whitebox.getInternalState(mVehiclePowerServiceSpy, "mExtSrvMgnBroadcastReceiver");

        receiver.onReceive(mContextMock, mIntentMock);

        verify(mExtSrvManagerMock).addService(anyString(), any(VehiclePowerServiceAdapter.class));
    }

    @Test
    public void onReceiveWithNullBinder() {
        when(mIntentMock.getExtras()).thenReturn(mBundleMock);
        when(mBundleMock.getBinder(ESM_SERVICE)).thenReturn(null);

        BroadcastReceiver receiver =
                Whitebox.getInternalState(mVehiclePowerServiceSpy, "mExtSrvMgnBroadcastReceiver");

        receiver.onReceive(mContextMock, mIntentMock);

        verify(mExtSrvManagerMock, times(0))
                .addService(anyString(), any(VehiclePowerServiceAdapter.class));
    }

    private void mockMethodsInStateInitializer() {
        when(mStateInitializerMock.use(mVehiclePowerServiceSpy)).thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(any(VpsStateMachine.class)))
                .thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(
                        mLateEfwServicesRunnerMock,
                        mNormalEfwServicesRunnerMock,
                        mLateAppServicesRunnerMock,
                        mNormalAppServicesRunnerMock))
                .thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(mVpdNativeAdapterMock)).thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(
                        any(SubscriptionManager.class), any(SubscriptionManager.class)))
                .thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(
                        mAppStopCompleteCounterMock,
                        mEfwStopCompleteCounterMock,
                        mFwResumeCompleteCounterMock,
                        mFwRestartCompleteCounterMock,
                        mAppResumeCompleteCounterMock,
                        mAppRestartCompleteCounterMock))
                .thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(mStartupInfoContainerMock))
                .thenReturn(mStateInitializerMock);
        when(mStateInitializerMock.use(any(ServicesRunningChecker.class)))
                .thenReturn(mStateInitializerMock);
    }
}
