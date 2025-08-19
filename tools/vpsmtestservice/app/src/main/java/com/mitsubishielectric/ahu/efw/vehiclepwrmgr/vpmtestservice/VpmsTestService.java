package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpmtestservice;

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import com.mitsubishielectric.ahu.efw.lib.common.Const;
import com.mitsubishielectric.ahu.efw.lib.extendedservicemanager.ExtSrvManager;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.VehiclePowerServiceManager;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.Callable;

public class VpmsTestService extends Service {

    private static final String VPSMTEST = "VPSM_TEST";
    private static final String VPM_TEST_FILTER = "VPM_TEST_FILTER";
    private static final String VPM_TEST_ACTION = "VPM_TEST_ACTION";
    private static final String VPM_TEST_ACTION_PARAMETER = "VPM_TEST_ACTION_PARAMETER";
    private static final int WAIT_LOOP_DELAY = 200;
    private static final int NUMBER_OF_TRIES = 15;
    private final Map<String, Callable<Boolean>> vpsMethods = new HashMap<>();

    VpdNativeAdapter vpdNativeAdapter = new VpdNativeAdapter();

    private class VpsMethodInvoker implements Runnable {
        private final String mMethodName;

        VpsMethodInvoker(String methodName) {
            mMethodName = methodName;
        }

        @Override
        public void run() {
            try {
                callVpsApiMethod(mMethodName);
            } catch (Exception e) {
                Log.w(VPSMTEST, "VpsMethodInvoker. " + mMethodName + e.toString());
            }
        }
    }

    static {
        System.loadLibrary("vpsmtestservice_jni");
    }

    static class VehiclePowerServiceListener extends IVehiclePowerServiceListener.Stub {
        @Override
        public void onAppStart() {
            Log.i(VPSMTEST, "VehiclePowerServiceListener_OnAppStart");
            try {
                VehiclePowerServiceManager.getInstance()
                        .startComplete(VpmsTestService.class.getCanonicalName());
            } catch (RemoteException ex) {
                Log.w(VPSMTEST, "VehiclePowerServiceListener_ExceptionOnStartCompleteNotification");
            }
        }

        @Override
        public void onAppRestart() {
            Log.i(VPSMTEST, "VehiclePowerServiceListener_OnAppRestart");
            try {
                VehiclePowerServiceManager.getInstance()
                        .restartCompleteEfw(VpmsTestService.class.getCanonicalName());
            } catch (RemoteException ex) {
                Log.w(
                        VPSMTEST,
                        "VehiclePowerServiceListener_ExceptionOnRestartCompleteNotification");
            }
        }

        @Override
        public void onAppStop() {
            Log.i(VPSMTEST, "VehiclePowerServiceListener_OnAppStop");
            try {
                VehiclePowerServiceManager.getInstance()
                        .stopCompleteEfw(VpmsTestService.class.getCanonicalName());
            } catch (RemoteException ex) {
                Log.w(VPSMTEST, "VehiclePowerServiceListener_ExceptionOnStopCompleteNotification");
            }
        }

        @Override
        public void onAppResume() {
            Log.i(VPSMTEST, "VehiclePowerServiceListener_OnAppResume");
            try {
                VehiclePowerServiceManager.getInstance()
                        .resumeCompleteEfw(VpmsTestService.class.getCanonicalName());
            } catch (RemoteException ex) {
                Log.w(
                        VPSMTEST,
                        "VehiclePowerServiceListener_ExceptionOnResumeCompleteNotification");
            }
        }
    }

    private final IVehiclePowerServiceListener mVehiclePowerServiceListener =
            new VehiclePowerServiceListener();

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        initNative();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.i(VPSMTEST, "VpmsTestService_onStartCommand");
        if (intent != null && intent.getExtras() != null) {
            IBinder esmBinder = intent.getExtras().getBinder(Const.ESM_SERVICE);
            registerService(esmBinder);
        } else {
            ServiceConnection connection =
                    new ServiceConnection() {
                        public void onServiceConnected(ComponentName name, IBinder esmBinder) {
                            registerService(esmBinder);
                        }

                        public void onServiceDisconnected(ComponentName name) {}
                    };
            Intent esmIntent = new Intent();
            esmIntent.setClassName(Const.ESM_PACKAGE, Const.ESM_SERVICE);
            bindService(esmIntent, connection, BIND_AUTO_CREATE);
        }

        super.onStartCommand(intent, flags, startId);
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        cleanupNative();
        super.onDestroy();
    }

    private void registerService(IBinder esmBinder) {
        try {
            ExtSrvManager.setBinder(esmBinder);

            IBinder vpsBinder = ExtSrvManager.getInstance().getService(Const.VEHICLE_POWER_SERVICE);
            VehiclePowerServiceManager.setBinder(vpsBinder);
            VehiclePowerServiceManager.getInstance()
                    .subscribeFWService(
                            mVehiclePowerServiceListener, VpmsTestService.class.getCanonicalName());
            registerBR();
            waitAllServicesStarted();
            waitUntilIsmStarted();
        } catch (RemoteException rex) {
            Log.w(VPSMTEST, "VpmsTestService_OnRegisterService_Exception");
        }
    }

    private void waitAllServicesStarted() {
        vpsMethods.put(
                "waitNormalAppServicesStarted",
                VehiclePowerServiceManager.getInstance()::normalAppServicesStarted);
        vpsMethods.put(
                "waitAllFwServicesStarted",
                VehiclePowerServiceManager.getInstance()::isAllEfwServicesStarted);
        vpsMethods.put(
                "waitAllAppServicesStarted",
                VehiclePowerServiceManager.getInstance()::isAllAppServicesStarted);
        vpsMethods.put(
                "waitNormalFwServicesStartComplete",
                VehiclePowerServiceManager.getInstance()::isNormalEfwServicesStartComplete);
        vpsMethods.put(
                "waitNormalAppServicesStartComplete",
                VehiclePowerServiceManager.getInstance()::isNormalAppServicesStartComplete);
        vpsMethods.put(
                "waitLateFwServicesStartComplete",
                VehiclePowerServiceManager.getInstance()::isLateEfwServicesStartComplete);
        vpsMethods.put(
                "waitLateAppServicesStartComplete",
                VehiclePowerServiceManager.getInstance()::isLateAppServicesStartComplete);

        for (String methodName : vpsMethods.keySet()) {
            new Thread(new VpsMethodInvoker(methodName)).start();
        }
    }

    private void waitUntilIsmStarted() {
        Log.i(VPSMTEST, "WaitUntilIsmStarted start polling");
        EServiceState state = null;
        while (state != EServiceState.STARTED) {
            try {
                state = VehiclePowerServiceManager.getInstance().getServiceState(Const.ISM_SERVICE);
                Log.i(VPSMTEST, "WaitUntilIsmStarted. Returned status: " + state);
                Thread.sleep(WAIT_LOOP_DELAY);
            } catch (Exception e) {
                Log.w(VPSMTEST, "VpmsTestService. WaitUntilIsmStarted. Exception: " + e.toString());
            }
        }
    }

    private void callVpsApiMethod(String methodName) {
        Log.i(VPSMTEST, methodName + " start polling");
        boolean status = false;
        int count = 0;
        while (!status && count < NUMBER_OF_TRIES) {
            try {
                status = vpsMethods.get(methodName).call();
                Log.i(VPSMTEST, methodName + ". Returned status: " + status);
                Thread.sleep(WAIT_LOOP_DELAY);
                count++;
            } catch (Exception e) {
                Log.w(VPSMTEST, "VpmsTestService. " + methodName + ". Exception: " + e.toString());
            }
        }
        Log.i(
                VPSMTEST,
                methodName + " end polling. Status: " + (status ? "completed" : "not completed"));
    }

    private void registerBR() {
        IntentFilter intentFilter = new IntentFilter(VPM_TEST_FILTER);

        VpmsTestService.this
                .getApplicationContext()
                .registerReceiver(
                        new BroadcastReceiver() {
                            @Override
                            public void onReceive(Context context, Intent intent) {
                                String action = intent.getStringExtra(VPM_TEST_ACTION);
                                String parameter = intent.getStringExtra(VPM_TEST_ACTION_PARAMETER);
                                if (action != null) {
                                    try {
                                        applyAction(
                                                VehiclePowerServiceManager.getInstance(),
                                                action,
                                                parameter);
                                    } catch (RemoteException e) {
                                        Log.w(VPSMTEST, "VpmsTestService_BR_RemoteException");
                                    }
                                } else {
                                    Log.w(VPSMTEST, "EMPTY ACTION");
                                }
                            }
                        },
                        intentFilter);
    }

    private void applyAction(
            VehiclePowerServiceManager vehiclePowerServiceManager,
            String action,
            String serviceName)
            throws RemoteException {
        Log.i(VPSMTEST, "Receive action:" + action);
        switch (action) {
            case "getVpsState":
                {
                    EVpsState state = VehiclePowerServiceManager.getInstance().getNotifiedEvent();
                    Log.i(VPSMTEST, "VpmsTestService: getNotifiedEvent. " + state);
                    vpdNativeAdapter.getVpdState();
                    break;
                }
            case "isServiceStarted":
                {
                    if (serviceName != null) {
                        Log.i(VPSMTEST, "Receive service name:" + serviceName);
                        Log.i(
                                VPSMTEST,
                                "Result:"
                                        + vehiclePowerServiceManager.isServiceStarted(serviceName));
                    } else {
                        Log.w(VPSMTEST, "Receive empty service name");
                    }
                    break;
                }

            case "getServiceState":
                {
                    if (serviceName != null) {
                        Log.i(VPSMTEST, "Receive service name:" + serviceName);
                        Log.i(
                                VPSMTEST,
                                "Result:"
                                        + vehiclePowerServiceManager.getServiceState(serviceName));
                    } else {
                        Log.w(VPSMTEST, "Receive empty service name");
                    }
                    break;
                }

            case "isAllAppServicesStarted":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:" + vehiclePowerServiceManager.isAllAppServicesStarted());
                    break;
                }

            case "isAllEfwServicesStarted":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:" + vehiclePowerServiceManager.isAllEfwServicesStarted());
                    break;
                }

            case "normalAppServicesStarted":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:" + vehiclePowerServiceManager.normalAppServicesStarted());
                    break;
                }

            case "isNormalEfwServicesStartComplete":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:"
                                    + vehiclePowerServiceManager
                                            .isNormalEfwServicesStartComplete());
                    break;
                }

            case "isNormalAppServicesStartComplete":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:"
                                    + vehiclePowerServiceManager
                                            .isNormalAppServicesStartComplete());
                    break;
                }

            case "isLateEfwServicesStartComplete":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:"
                                    + vehiclePowerServiceManager.isLateEfwServicesStartComplete());
                    break;
                }

            case "isLateAppServicesStartComplete":
                {
                    Log.i(
                            VPSMTEST,
                            "Result:"
                                    + vehiclePowerServiceManager.isLateAppServicesStartComplete());
                    break;
                }

            default:
                Log.w(VPSMTEST, "UNDEFINED ACTION");
        }
    }

    private native void initNative();

    private native void cleanupNative();
}
