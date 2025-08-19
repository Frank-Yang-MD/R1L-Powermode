/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ACCESS_MELCO_SERVICES;
import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_REBOOT_ACTION_INTENT;
import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_SERVICE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ESM_RESTART_BROADCAST_IS_NULL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ESM_RESTART_BROADCAST_RECEIVED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EXT_SRV_MGR_REG_FAILED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EXT_SRV_MGR_REG_SUCCESS;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.NOTIFY_EXT_SRV_MGR_BOUND;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ON_CREATE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_NOT_BINDABLE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.terminateLogDog;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.extendedservicemanager.ExtSrvManager;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.CompleteNotificationCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ISubscriptionNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueue;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueueHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.EventQueueHandlerBuilder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner.ExtSrvManagerRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner.IExtSrvMgrRunNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmHandoffEventListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IIsmSubscription;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism.IsmSubscription;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement.MountManagement;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.JavaRunnerCreator;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.JsonPreParser;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigReader;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigReaderAdapter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigReaderWithFallback;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner.VehicleConfigRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.IServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.ServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.StartupInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.stateholder.StateHolder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.StateInitializer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager.SubscriptionManager;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager.SubscriptionOberserveableDecorator;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager.SubscriptionRemoteCallbackList;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeAdapter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.VpdNativeReceiver;
import java.util.Arrays;
import java.util.concurrent.ArrayBlockingQueue;

public final class VehiclePowerService extends Service implements IExtSrvMgrRunNotification {
    private static final int EVENT_QUEUE_SIZE = 100;
    /** Incoming event queue. */
    private final EventQueue mEventQueue =
            new EventQueue(new ArrayBlockingQueue<>(EVENT_QUEUE_SIZE));
    /** Power state holder */
    private StateHolder mStateHolder = new StateHolder(mEventQueue);

    /** Receiver of vehicle power state change notification */
    private final VpdNativeReceiver mVpdNativeReceiver;

    /** JNI adapter to outgoing start/stop process complete */
    private final VpdNativeAdapter mVpdNativeAdapter = new VpdNativeAdapter();

    /** Holder for objects related to mount management. */
    private final MountManagement mMountManagement = new MountManagement(mVpdNativeAdapter);

    /** State machine */
    private final VpsStateMachine mStateMachine = new VpsStateMachine();

    private StartupInfoContainer mInfoContainer = new StartupInfoContainer();

    /** App stop complete counter */
    private final CompleteNotificationCounter mAppStopCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.APP_SERVICES_STOP_COMPLETE, mInfoContainer);

    /** EFW stop complete counter */
    private final CompleteNotificationCounter mEfwStopCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.EFW_SERVICES_STOP_COMPLETE, mInfoContainer);

    /** Resume FW complete counter */
    private final CompleteNotificationCounter mFwResumeCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.EFW_SERVICES_RESUME_COMPLETE, mInfoContainer);

    /** Restart FW complete counter */
    private final CompleteNotificationCounter mFwRestartCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.EFW_SERVICES_RESTART_COMPLETE, mInfoContainer);

    /** Resume app complete counter */
    private final CompleteNotificationCounter mAppResumeCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.EFW_APP_SERVICES_RESUME_COMPLETE, mInfoContainer);

    /** Restart app complete counter */
    private final CompleteNotificationCounter mAppRestartCompleteCounter =
            new CompleteNotificationCounter(
                    mEventQueue, IncomingEvents.EFW_APP_SERVICES_RESTART_COMPLETE, mInfoContainer);

    /** Subscription notifications list for apps */
    private final ISubscriptionNotification[] subscriptionAppNotifications = {
        mAppStopCompleteCounter, mAppResumeCompleteCounter, mAppRestartCompleteCounter
    };

    /** Subscription notifications list for services */
    private final ISubscriptionNotification[] subscriptionEfwNotifications = {
        mEfwStopCompleteCounter, mFwResumeCompleteCounter, mFwRestartCompleteCounter
    };

    /** Decorator to dispatch notifications to EFW subscription list */
    private final SubscriptionOberserveableDecorator mEfwSubscriptionOberserveableDecorator =
            new SubscriptionOberserveableDecorator(Arrays.asList(subscriptionEfwNotifications));

    /** Decorator to dispatch notifications to app subscription list */
    private final SubscriptionOberserveableDecorator mAppSubscriptionOberserveableDecorator =
            new SubscriptionOberserveableDecorator(Arrays.asList(subscriptionAppNotifications));

    /** Subscription Manager for apps */
    private final SubscriptionManager mAppSubcriptionManager =
            new SubscriptionManager(
                    new SubscriptionRemoteCallbackList(
                            mStateHolder, mAppSubscriptionOberserveableDecorator));

    /** Subscription Manager for services */
    private final SubscriptionManager mServiceSubscriptionManager =
            new SubscriptionManager(
                    new SubscriptionRemoteCallbackList(
                            mStateHolder, mEfwSubscriptionOberserveableDecorator));

    private IServicesRunningChecker mServicesRunningChecker;

    private IsmSubscription mIsmSubscription = new IsmSubscription();

    /** Implementation of external API */
    private final VehiclePowerServiceAdapter mVehiclePowerServiceAdapter =
            new VehiclePowerServiceAdapter(
                    mEventQueue,
                    mAppSubcriptionManager,
                    mServiceSubscriptionManager,
                    mStateHolder,
                    this,
                    mInfoContainer,
                    mAppStopCompleteCounter,
                    mEfwStopCompleteCounter,
                    mFwResumeCompleteCounter,
                    mFwRestartCompleteCounter,
                    mAppResumeCompleteCounter,
                    mAppRestartCompleteCounter,
                    (IIsmSubscription) mIsmSubscription,
                    mVpdNativeAdapter);

    /** Incoming event handler */
    private final EventQueueHandler mEventQueueHandler =
            new EventQueueHandlerBuilder(
                            mEventQueue, mStateMachine, mMountManagement.getMountEventHandler())
                    .build();
    /** ESM runner */
    private final ExtSrvManagerRunner mExtSrvManagerRunner = new ExtSrvManagerRunner(this, this);

    /** Java runner for Late services */
    private IJavaRunner mLateEfwServicesRunner;

    /** Java runner for Normal services */
    private IJavaRunner mNormalEfwServicesRunner;
    /** Java runner for Late application services */
    private IJavaRunner mLateAppServicesRunner;

    /** Java runner for Normal application services */
    private IJavaRunner mNormalAppServicesRunner;

    private final BroadcastReceiver mExtSrvMgnBroadcastReceiver =
            new BroadcastReceiver() {
                @Override
                public void onReceive(Context context, Intent intent) {
                    IBinder esmBinder = intent.getExtras().getBinder(ESM_SERVICE);

                    if (esmBinder != null) {
                        MLog.i(
                                VPS_FUNCTION_ID,
                                ESM_RESTART_BROADCAST_RECEIVED,
                                esmBinder.toString());
                        MLog.i(
                                VPS_EARLY_FUNCTION_ID,
                                ESM_RESTART_BROADCAST_RECEIVED,
                                esmBinder.toString());
                        registerInEsm(esmBinder);
                    } else {
                        MLog.i(VPS_FUNCTION_ID, ESM_RESTART_BROADCAST_IS_NULL);
                        MLog.i(VPS_EARLY_FUNCTION_ID, ESM_RESTART_BROADCAST_IS_NULL);
                    }
                }
            };

    static {
        System.loadLibrary("vehicle_power_service_jni");
    }

    public VehiclePowerService() {
        mVpdNativeReceiver =
                new VpdNativeReceiver(mStateHolder, (IIsmHandoffEventListener) mIsmSubscription);
        // This is temporary code.
        mInfoContainer.use(mVpdNativeAdapter);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        MLog.i(VPS_FUNCTION_ID, ON_CREATE);
        MLog.i(VPS_EARLY_FUNCTION_ID, ON_CREATE);
        initNative();
        mExtSrvManagerRunner.run();

        mServicesRunningChecker =
                new ServicesRunningChecker(mInfoContainer, getApplicationContext());

        IntentFilter intentFilter = new IntentFilter(ESM_REBOOT_ACTION_INTENT);
        registerReceiver(mExtSrvMgnBroadcastReceiver, intentFilter, ACCESS_MELCO_SERVICES, null);
    }

    @Override
    public IBinder onBind(Intent intent) {
        MLog.i(VPS_FUNCTION_ID, SERVICE_NOT_BINDABLE);
        MLog.i(VPS_EARLY_FUNCTION_ID, SERVICE_NOT_BINDABLE);
        return null;
    }

    @Override
    public void onDestroy() {
        unregisterReceiver(mExtSrvMgnBroadcastReceiver);
        cleanupNative();
        terminateLogDog();
        super.onDestroy();
    }

    @Override
    public void notifyExtSrvMgrBound(IBinder binder) {
        MLog.i(VPS_FUNCTION_ID, NOTIFY_EXT_SRV_MGR_BOUND);
        MLog.i(VPS_EARLY_FUNCTION_ID, NOTIFY_EXT_SRV_MGR_BOUND);
        registerInEsm(binder);
        createRunners(binder);
        initStates();
    }

    private void initStates() {
        new StateInitializer()
                .use(this)
                .use(mStateMachine)
                .use(
                        mLateEfwServicesRunner,
                        mNormalEfwServicesRunner,
                        mLateAppServicesRunner,
                        mNormalAppServicesRunner)
                .use(mVpdNativeAdapter)
                .use(mServiceSubscriptionManager, mAppSubcriptionManager)
                .use(
                        mAppStopCompleteCounter,
                        mEfwStopCompleteCounter,
                        mFwResumeCompleteCounter,
                        mFwRestartCompleteCounter,
                        mAppResumeCompleteCounter,
                        mAppRestartCompleteCounter)
                .use(mInfoContainer)
                .use(mServicesRunningChecker)
                .initStates();
        mEventQueueHandler.start();
    }

    private void createRunners(IBinder binder) {
        if (binder == null) {
            throw new IllegalArgumentException("binder is NULL!");
        }

        final String LATE_EFW_CONF_KEY_NAME = "VPM.LATE_EFW_SERVICES_START_CONFIG";
        final String NORMAL_EFW_CONF_KEY_NAME = "VPM.NORMAL_EFW_SERVICES_START_CONFIG";
        final String LATE_APP_CONF_KEY_NAME = "VPM.LATE_APP_SERVICES_START_CONFIG";
        final String NORMAL_APP_CONF_KEY_NAME = "VPM.NORMAL_APP_SERVICES_START_CONFIG";

        ExtSrvManager.setBinder(binder);

        VehicleConfigRunner vehicleConfigRunner =
                new VehicleConfigRunner(this, binder, ExtSrvManager.getInstance());
        VehicleConfigReaderAdapter vehicleConfigReaderAdapter =
                new VehicleConfigReaderAdapter(vehicleConfigRunner.getReadyVehicleConfigManager());

        JavaRunnerCreator builder =
                new JavaRunnerCreator(this, binder, mEventQueue, mInfoContainer, mInfoContainer);
        mLateEfwServicesRunner =
                builder.create(
                        IncomingEvents.LATE_EFW_SERVICES_STARTED,
                        new VehicleConfigReader(
                                LATE_EFW_CONF_KEY_NAME,
                                vehicleConfigReaderAdapter,
                                new JsonPreParser(mInfoContainer, EServiceType.LATE_EFW)));

        mNormalEfwServicesRunner =
                builder.create(
                        IncomingEvents.NORMAL_EFW_SERVICES_STARTED,
                        new VehicleConfigReader(
                                NORMAL_EFW_CONF_KEY_NAME,
                                vehicleConfigReaderAdapter,
                                new JsonPreParser(mInfoContainer, EServiceType.NORMAL_EFW)));

        final String DIAG_CONF_KEY_NAME = "VPM.JAVA_DIAG_START_CONFIG";
        mLateAppServicesRunner =
                builder.create(
                        IncomingEvents.LATE_APP_SERVICES_STARTED,
                        new VehicleConfigReaderWithFallback(
                                LATE_APP_CONF_KEY_NAME,
                                DIAG_CONF_KEY_NAME,
                                vehicleConfigReaderAdapter,
                                new JsonPreParser(mInfoContainer, EServiceType.LATE_APP)));

        mNormalAppServicesRunner =
                builder.create(
                        IncomingEvents.NORMAL_APP_SERVICES_STARTED,
                        new VehicleConfigReader(
                                NORMAL_APP_CONF_KEY_NAME,
                                vehicleConfigReaderAdapter,
                                new JsonPreParser(mInfoContainer, EServiceType.NORMAL_APP)));
    }

    private void registerInEsm(IBinder binder) {
        ExtSrvManager.setBinder(binder);
        ExtSrvManager extSrvManager = ExtSrvManager.getInstance();
        if (extSrvManager != null) {
            extSrvManager.addService(
                    VehiclePowerService.class.getName(), mVehiclePowerServiceAdapter);
            MLog.i(VPS_FUNCTION_ID, EXT_SRV_MGR_REG_SUCCESS);
            MLog.i(VPS_EARLY_FUNCTION_ID, EXT_SRV_MGR_REG_SUCCESS);
        } else {
            logWarning(EXT_SRV_MGR_REG_FAILED);
        }
    }

    private native void initNative();

    private native void cleanupNative();
}
