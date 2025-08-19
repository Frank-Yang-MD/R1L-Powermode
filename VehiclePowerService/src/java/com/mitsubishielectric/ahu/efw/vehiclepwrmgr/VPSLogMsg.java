/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog.DisplayTypeDecInt;
import static com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog.DisplayTypeString;
import static com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog.LogFormat;
import static java.util.Arrays.asList;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.List;

public class VPSLogMsg {

    public static final int VPS_FUNCTION_ID = 1072;
    public static final int VPM_FUNCTION_ID = 1073;
    public static final int VPS_EARLY_FUNCTION_ID = 1157;

    public static void logWarning(int logId, Object... args) {
        MLog.w(VPS_FUNCTION_ID, logId, args);
        MLog.w(VPM_FUNCTION_ID, logId, args);
        MLog.w(VPS_EARLY_FUNCTION_ID, logId, args);
    }

    public static void logError(int logId, Object... args) {
        MLog.e(VPS_FUNCTION_ID, logId, args);
        MLog.e(VPM_FUNCTION_ID, logId, args);
        MLog.e(VPS_EARLY_FUNCTION_ID, logId, args);
    }

    public static void logException(int logId, Exception e) {
        MLog.w(VPS_FUNCTION_ID, logId, e.getMessage() != null ? e.getMessage() : e.toString());
        MLog.w(VPM_FUNCTION_ID, logId, e.getMessage() != null ? e.getMessage() : e.toString());
    }

    public static void initializeLogDog() {
        MLog.initialize(VPS_FUNCTION_ID, logMessageFormats);
        MLog.initialize(VPM_FUNCTION_ID, logMessageFormats);
        MLog.initialize(VPS_EARLY_FUNCTION_ID, logMessageFormats);
    }

    public static void terminateLogDog() {
        MLog.terminate(VPS_FUNCTION_ID);
        MLog.terminate(VPM_FUNCTION_ID);
        MLog.terminate(VPS_EARLY_FUNCTION_ID);
    }

    public static final int ON_CREATE = 1000;
    public static final int SERVICE_NOT_BINDABLE = 1002;
    public static final int NOTIFY_EXT_SRV_MGR_BOUND = 1003;
    public static final int EXT_SRV_MGR_REG_SUCCESS = 1004;
    public static final int EXT_SRV_MGR_REG_FAILED = 1005;
    public static final int SUBSCRIBE_APP_CALLED_FROM = 1006;
    public static final int SUBSCRIBE_SRV_CALLED_FROM = 1007;
    public static final int UNSUBSCRIBE_CALLED_FROM = 1008;
    public static final int STOP_PROC_COMPLETE_APP_CALLED_FROM = 1009;
    public static final int GET_NOTIFIED_EVT_CALLED_FROM = 1010;
    public static final int RECEIVED_ACTION = 1011;
    public static final int SERVICE_START_SUCCESS = 1012;
    public static final int SERVICE_START_FAIL = 1013;
    public static final int EXCEPTION_IN_ON_RECEIVE = 1014;
    public static final int ON_VPD_STATE_CHG_CALL_FROM_JNI = 1015;
    public static final int SEND_START_COMPLETE_TO_VPD = 1016;
    public static final int RECEIVED_FW_STOP_FROM_VPD = 1018;

    public static final int ESM_BOUND_SUCCESS = 1019;
    public static final int ESM_BIND_SERVICE_RETURN = 1020;
    public static final int ESM_BIND_FAILED = 1021;

    public static final int SUBSCR_MAN_DISCONNECT_LISTENER = 1022;
    public static final int SUBSCR_MAN_NOTIFY_SUBSCR_PWR_STATE_CHG = 1025;
    public static final int SUBSCR_MAN_NOTIFY_ALL_INVALID_EVT = 1026;
    public static final int SUBSCR_MAN_CANT_CALL_REMOTE_LISTENER = 1027;
    public static final int SUBSCR_MAN_NOTIFICATION_SUCCESS = 1028;
    public static final int SUBSCR_MAN_RECEIVED_SUBSCR_REQUEST = 1029;
    public static final int SUBSCR_MAN_RECEIVED_UNSUBSCR_REQUEST = 1030;
    public static final int SUBSCR_MAN_ILLEG_ARG_EXCEPTION = 1031;
    public static final int ISM_READY_NOTIFICATION = 1032;
    public static final int REQUEST_END_EARLY_HMI_NOTIFICATION = 1033;
    public static final int FORCED_COMPLETION_EARLY_SERVICES_NOTIFICATION = 1034;
    public static final int SUBSCR_MAN_BROADCAST_FINISHED = 1260;
    public static final int SUBSCR_MAN_ALL_LISTENERS_NOTIFIED = 1261;
    public static final int STOP_PROC_COMPLETE_FW_CALLED_FROM = 1262;

    public static final int SM_INIT_STATES = 1042;
    public static final int SM_INITIAL_STATE = 1043;

    public static final int SM_RECEIVED_ON_APP_RESTART_EVT = 1045;
    public static final int SM_RECEIVED_ON_APP_RESUME_EVT = 1046;
    public static final int SM_RECEIVED_ON_APP_STOP_EVT = 1047;
    public static final int SM_RECEIVED_ON_APP_START_COLDBOOT_EVT = 1048;
    public static final int SM_RECEIVED_ON_NORMAL_APP_STARTED_EVT = 1069;
    public static final int SM_RECEIVED_ON_LATE_APP_STARTED_EVT = 1049;
    public static final int SM_RECEIVED_ON_NORMAL_EFW_SRV_STARTED_EVT = 1050;
    public static final int SM_RECEIVED_ON_APP_STOPPED_EVT = 1051;
    public static final int SM_RECEIVED_ON_SRV_STOPPED_EVT = 1052;
    public static final int SM_VPS_SM_TRANS_TO_NULL_PTR_PASS = 1054;
    public static final int SM_FROM_TO = 1055;
    public static final int SM_EXIT_FROM = 1056;
    public static final int SM_ENTER_TO = 1057;

    public static final int SM_STATE_HOLDER_RCV_PWR_CHG = 1060;
    public static final int SM_STATE_HOLDER_ERR_INT_PWR = 1061;
    public static final int SM_STATE_HOLDER_SEND_EVT = 1062;
    public static final int SM_STATE_HOLDER_CONV_VALUE = 1063;

    public static final int SM_ADAPTERS_PWR_CHG_H_EXEC = 1064;
    public static final int SM_ADAPTERS_START_H_EXEC = 1065;

    public static final int RUNNER_INV_CMD_CANT_EXEC = 1070;

    public static final int RUNNER_JAVA_START_SCRIPT = 1071;
    public static final int RUNNER_JAVA_START_THREAD = 1072;

    public static final int RUNNER_JAVA_CREAT_BUILD = 1073;

    public static final int RUNNER_SCRIPT_EXEC_START_LEN = 1074;
    public static final int RUNNER_SCRIPT_EXEC_PARSE_CMD = 1075;
    public static final int RUNNER_SCRIPT_EXEC_FAIL_CMD = 1076;
    public static final int RUNNER_SCRIPT_EXEC_SEND_EVT = 1077;

    public static final int SM_RECEIVED_ON_LATE_EFW_SRV_STARTED_EVT = 1078;

    public static final int RUNNER_SIMPLE_SLEEP_EXC = 1089;

    public static final int RUNNER_SLEEP_CMD_CREATE = 1090;
    public static final int RUNNER_SLEEP_CMD_EXEC = 1091;

    public static final int RUNNER_SLEEP_CMD_CR_PARSE = 1092;
    public static final int RUNNER_SLEEP_CMD_CR_PARSE_FAIL_FORM = 1093;
    public static final int RUNNER_SLEEP_CMD_CR_PARSE_FAIL_VAL = 1094;
    public static final int RUNNER_SLEEP_CMD_CR_PARSE_FAIL_NUM = 1095;

    public static final int RUNNER_START_CMD_CREATE = 1096;
    public static final int RUNNER_START_CMD_EXEC = 1097;
    public static final int RUNNER_START_CMD_FAIL = 1098;
    public static final int RUNNER_START_CMD_SRV_OK = 1099;
    public static final int RUNNER_START_CMD_START_EXC = 1100;

    public static final int RUNNER_START_CMD_CR_PARSE_CMD_TXT = 1106;

    public static final int RUNNER_VCONF_READ_ERROR = 1107;

    public static final int RUNNER_VCONF_RUN_VPS_FAIL = 1108;
    public static final int RUNNER_VCONF_RUN_VPS_WAIT = 1109;
    public static final int RUNNER_VCONF_RUN_BIND_NULL = 1110;
    public static final int RUNNER_VCONF_RUN_SRV_OK = 1111;
    public static final int RUNNER_VCONF_RUN_SRV_FAIL = 1112;
    public static final int RUNNER_VCONF_RUN_START_FAIL = 1113;
    public static final int RUNNER_VCONF_RUN_WAIT_ESM = 1114;
    public static final int RUNNER_VCONF_RUN_DONE_ESM = 1115;
    public static final int RUNNER_VCONF_RUN_EXC_ESM = 1116;

    public static final int EVT_Q_Q_REC_EVT_ADD = 1117;
    public static final int EVT_Q_Q_REC_EVT_FAIL = 1118;
    public static final int EVT_Q_Q_INC_EVT_GET = 1119;
    public static final int EVT_Q_Q_INC_EVT_FAIL = 1120;

    public static final int EVT_Q_H_ADD_EVT = 1121;
    public static final int EVT_Q_H_REPL_EVT = 1122;
    public static final int EVT_Q_H_START = 1123;
    public static final int EVT_Q_H_ALREADY_RUN = 1124;
    public static final int EVT_Q_H_ALREADY_STOP = 1125;
    public static final int EVT_Q_H_STOP = 1126;
    public static final int EVT_Q_H_NULL = 1127;
    public static final int EVT_Q_H_FAIL = 1128;

    public static final int SUBSCR_MAN_NOTIFY_ALL_SUBSCR_PWR_STATE_CHG = 1129;

    public static final int JSON_PARSE_ERROR_CONFIG_READER = 1131;
    public static final int IS_SERVICE_STARTED_INVALID_CLASSNAME = 1132;
    public static final int SERVICE_STARTED_INVALID_CLASSNAME = 1133;
    public static final int SERVICE_START_COMPLETE_INVALID_CLASSNAME = 1134;
    public static final int NOTIFY_START_COMPLETE_CALL = 1136;
    public static final int SERVICE_STOP_INVALID_CLASSNAME = 1137;
    public static final int SERVICE_RESUME_RESTART_INVALID_CLASSNAME = 1138;
    public static final int FAILED_TO_PARSE_COMMAND_JSON_PARSER = 1139;
    public static final int UNKNOWN_PAR_CONF_FILE = 1140;
    public static final int FAILED_TO_PARSE_JSON_FILE = 1141;

    public static final int SERVICE_STOP_COMPLETE = 1143;
    public static final int SERVICE_RESUME_RESTART_COMPLETE = 1144;
    public static final int SERVICE_START_COMPLETE = 1145;

    public static final int RESUME_COMPLETE_CALL = 1150;
    public static final int RESTART_COMPLETE_CALL = 1151;
    public static final int RESUME_COMPLETE_APP_CALL = 1152;
    public static final int RESTART_COMPLETE_APP_CALL = 1153;

    public static final int JSONCMDPARS_FAILED_TO_PARSE = 1158;
    public static final int WAITCMD_WAITING_FOR = 1159;
    public static final int WAITCMDCREATOR_CANNOT_PARSE_CMD = 1160;
    public static final int WAITCMDCREATOR_SRV_LIST = 1161;

    public static final int START_COMPLETE_TIMEOUT_OCCURR = 1162;

    public static final int SETUP_START_COMPLETE_TIMEOUT = 1163;
    public static final int INVALID_VALUE_FOR_START_COMPLETE_TIMEOUT = 1164;

    public static final int SEND_STOP_FAILED_TO_VPD = 1165;

    public static final int STOPPED_SERVICES_ERROR = 1166;

    public static final int WAITCMD_EXEC_FAILED = 1167;

    public static final int WAIT_STARTUP_COMPLETE = 1168;
    public static final int WAIT_STARTUP_FAILED_SERVICE_DOESNT_EXIST = 1169;
    public static final int WAIT_STARTUP_FINISHED = 1170;

    public static final int SEND_START_FAILED_TO_VPD = 1180;
    public static final int SEND_APP_STOP_TO_VPD = 1181;
    public static final int SEND_FW_STOP_TO_VPD = 1182;
    public static final int SEND_FW_RESTART_TO_VPD = 1183;
    public static final int SEND_FW_RESUME_TO_VPD = 1184;

    public static final int RECEIVED_FW_RESTART_FROM_VPD = 1185;
    public static final int RECEIVED_FW_RESUME_FROM_VPD = 1186;
    public static final int RECEIVED_APP_RESUME_FROM_VPD = 1187;
    public static final int RECEIVED_APP_RESTART_FROM_VPD = 1188;

    public static final int SMA_COULDNT_OBTAIN_BINDER = 1190;
    public static final int SMA_UNMOUNTING_NULL_BINDER = 1191;
    public static final int SMA_UNMOUNTING_ALL = 1192;
    public static final int SMA_OBTAINING_VOLUME_LIST = 1193;
    public static final int SMA_VOLUME_LIST_OBTAINED = 1194;
    public static final int SMA_UNMOUNTING_VOLUME = 1195;
    public static final int SMA_VOLUME_UNMOUNTED = 1196;
    public static final int SMA_UNMOUNTING_REMOTE_EXCEPTION = 1197;

    public static final int SMA_MOUNTING_NULL_BINDER = 1198;
    public static final int SMA_MOUNTING_UNMOUNTED = 1199;
    public static final int RECEIVED_HANDOFF = 1200;
    public static final int RECEIVED_HANDOFF_STARTUP_ANIMATION = 1201;
    public static final int RECEIVED_HANDOFF_STARTUP_ANIMATION_DI = 1202;
    public static final int SMA_MOUNTING_VOLUME = 1250;
    public static final int SMA_VOLUME_MOUNTED = 1251;
    public static final int SMA_MOUNTING_REMOTE_EXCEPTION = 1252;

    public static final int SMA_UNMOUNTING_RUNTIME_EXCEPTION = 1253;
    public static final int SMA_MOUNTING_RUNTIME_EXCEPTION = 1254;

    public static final int HAS_NULL_OBJ_LISTENER_SUBS = 1310;
    public static final int HAS_NULL_OBJ_LISTENER_UNSUBS = 1311;
    public static final int HAS_DISPLAY_MODE_CHANGING = 1313;

    public static final int SIC_TRY_TO_ADD_SERVICEINFO = 1401;
    public static final int SIC_SERVICEINFO_WAS_ADDED_TO = 1402;
    public static final int SIC_SERVICEINFO_ALREADY_EXIST = 1403;

    public static final int ESM_RESTART_BROADCAST_RECEIVED = 1410;
    public static final int ESM_RESTART_BROADCAST_IS_NULL = 1411;

    public static final int RUNNER_VCONF_READ_FALLBACK = 1428;

    public static final int BNA_HMI_IS_READY = 1450;
    public static final int SEND_UNMOUNT_COMPLETE_TO_VPD = 1452;
    public static final int SEND_END_EARLY_HMI_TO_VPD = 1453;
    public static final int SEND_FORCED_COMPLETION_EARLY_SERVICES_TO_VPD = 1454;

    public static final int SM_RECEIVED_ON_FW_RESTART_EVT = 1600;
    public static final int SM_RECEIVED_ON_FW_RESUME_EVT = 1601;
    public static final int SM_RECEIVED_ON_FW_STOP_EVT = 1602;
    public static final int SM_RECEIVED_ON_APP_SERVICES_STOPPED_COMPLETE_EVT = 1603;
    public static final int SM_RECEIVED_ON_FW_SERVICES_RESTART_COMPLETE_EVT = 1604;
    public static final int SM_RECEIVED_ON_FW_SERVICES_RESUME_COMPLETE_EVT = 1605;
    public static final int SM_RECEIVED_ON_FW_SERVICES_STOP_COMPLETE_EVT = 1606;
    public static final int SM_RECEIVED_ON_APP_SERVICES_RESTART_COMPLETE_EVT = 1608;
    public static final int SM_RECEIVED_ON_APP_SERVICES_RESUME_COMPLETE_EVT = 1609;

    public static final int SEND_BROADCAST_MELCO_BOOT_COMPLETE = 1610;

    public static final int SM_RECEIVED_ON_COMPLETION_TIMEOUT_EVT = 1611;

    public static final int VPS_START_NORMAL_APP_SERVICES_STATE_ONEXIT_NOTIFY_BOOTD = 1700;

    public static final int VPS_IS_SERVICE_STARTED = 1701;
    public static final int VPS_IS_ALL_EFW_SERVICES_STARTED = 1702;
    public static final int VPS_NORMAL_APP_SERVICES_STARTED = 1703;
    public static final int VPS_IS_ALL_APP_SERVICES_STARTED = 1704;
    public static final int VPS_GET_SERVICE_STATE = 1705;
    public static final int VPS_IS_NORMAL_EFW_SERVICES_START_COMPLETE = 1706;
    public static final int VPS_IS_NORMAL_APP_SERVICES_START_COMPLETE = 1707;
    public static final int VPS_IS_LATE_EFW_SERVICES_START_COMPLETE = 1708;
    public static final int VPS_IS_LATE_APP_SERVICES_START_COMPLETE = 1709;

    public static final int ISM_NOTIFY_TO_EMPTY_LISTENER = 1900;
    public static final int ISM_EXCEPTION_ISMSUBSCRIPTION_SUBSCRIBEISM = 1901;
    public static final int ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF = 1902;
    public static final int ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_DISPLAYED = 1903;
    public static final int ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_DI_DISPLAYED = 1904;
    public static final int ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_AND_DI_DISPLAYED = 1905;
    public static final int ISM_SEND_HANDOFF = 1906;
    public static final int ISM_SEND_HANDOFF_STARTUP_ANIMATION = 1907;
    public static final int ISM_SEND_HANDOFF_STARTUP_ANIMATION_DI = 1908;
    public static final int ISM_RECEIVE_NULL_LISTENER = 1909;

    public static final int CNC_ON_COMPLETE_NOT_SUBSCRIBED = 2000;
    public static final int CNC_ON_SUBSCRIBE_ACCEPTED = 2001;
    public static final int CNC_ON_SUBSCRIBE_DECLINED = 2002;
    public static final int CNC_ON_UNSUBSCRIBE_ACCEPTED = 2003;
    public static final int CNC_ON_UNSUBSCRIBE_DECLINED = 2004;
    public static final int CNC_ON_RESET = 2005;
    public static final int CNC_NOT_COMPLETE = 2006;
    public static final int CNC_ON_COMPLETE_ALREADY_UNSUBSCRIBED = 2007;
    public static final int CNC_ON_COMPLETE_FROM = 2008;

    private static final List<LogFormat> logMessageFormats =
            asList(
                    new LogFormat(ON_CREATE, "VPS. VehiclePowerService. onCreate()"),
                    new LogFormat(
                            SERVICE_NOT_BINDABLE, "VPS. VehiclePowerService. Service not bindable"),
                    new LogFormat(
                            NOTIFY_EXT_SRV_MGR_BOUND,
                            "VPS. VehiclePowerService. notifyExtSrvMgrBound"),
                    new LogFormat(
                            EXT_SRV_MGR_REG_SUCCESS,
                            "VPS. VehiclePowerService. Successfully registered in ExtSrvManager!"),
                    new LogFormat(
                            EXT_SRV_MGR_REG_FAILED,
                            "VPS. VehiclePowerService. Can't obtain instance of ExtSrvManager!"),
                    new LogFormat(
                            SUBSCRIBE_APP_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. subscribeApp called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            SUBSCRIBE_SRV_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. subscribeFWService called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            UNSUBSCRIBE_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. unsubscribe called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            STOP_PROC_COMPLETE_APP_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. stopProcessingCompleteApp called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            STOP_PROC_COMPLETE_FW_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. stopProcessingCompleteFw called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            GET_NOTIFIED_EVT_CALLED_FROM,
                            "VPS. VehiclePowerServiceAdapter. getNotifiedEvent called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            RECEIVED_ACTION,
                            "VPS. VpsBootReceiver. Received action: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_START_SUCCESS,
                            "VPS. VpsBootReceiver. Service started successful: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SERVICE_START_FAIL, "VPS. VpsBootReceiver. FAILED to start service."),
                    new LogFormat(
                            EXCEPTION_IN_ON_RECEIVE,
                            "VPS. VpsBootReceiver. Exception in onReceive : %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ON_VPD_STATE_CHG_CALL_FROM_JNI,
                            "VPS. VpdNativeReceiver. onVpdStateChange call from JNI: %d",
                            asList(new DisplayTypeDecInt("Call from"))),
                    new LogFormat(
                            RECEIVED_FW_RESTART_FROM_VPD,
                            "VPS. VpdNativeReceiver. onFwRestart call from JNI"),
                    new LogFormat(
                            RECEIVED_FW_RESUME_FROM_VPD,
                            "VPS. VpdNativeReceiver. onFwResume call from JNI"),
                    new LogFormat(
                            RECEIVED_FW_STOP_FROM_VPD,
                            "VPS. VpdNativeReceiver. onFwStop call from JNI"),
                    new LogFormat(
                            SEND_START_COMPLETE_TO_VPD,
                            "VPS. VpdNativeAdapter. Send start complete to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_START_FAILED_TO_VPD,
                            "VPS. VpdNativeAdapter. Send start failed to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_APP_STOP_TO_VPD,
                            "VPS. VpdNativeAdapter. Send AppStop complete to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_FW_STOP_TO_VPD,
                            "VPS. VpdNativeAdapter. Send FwStop complete to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_FW_RESTART_TO_VPD,
                            "VPS. VpdNativeAdapter. Send FwRestart complete to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_FW_RESUME_TO_VPD,
                            "VPS. VpdNativeAdapter. Send FwResume complete to VehiclePowerDaemon."),
                    new LogFormat(
                            SEND_STOP_FAILED_TO_VPD,
                            "VPS. VpdNativeAdapter. Send stop failed to VehiclePowerDaemon."),
                    new LogFormat(
                            ESM_BOUND_SUCCESS,
                            "VPS. ExtSrvManagerRunner. ESM Successfull bounded! Name : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            ESM_BIND_SERVICE_RETURN,
                            "VPS. ExtSrvManagerRunner. bindService return %s",
                            asList(new DisplayTypeString(1024, "Return"))),
                    new LogFormat(
                            ESM_BIND_FAILED,
                            "VPS. ExtSrvManagerRunner. bind failed! %s",
                            asList(new DisplayTypeString(1024, "Return"))),
                    new LogFormat(
                            SUBSCR_MAN_DISCONNECT_LISTENER,
                            "VPS. SubscriptionRemoteCallbackList. Disconnect listener; HashCode: %d",
                            asList(new DisplayTypeDecInt("HashCode"))),
                    new LogFormat(
                            SUBSCR_MAN_NOTIFY_SUBSCR_PWR_STATE_CHG,
                            "VPS. SubscriptionRemoteCallbackList. Notify subscriber "
                                    + "about power state change. Power state/Listener HashCode: %s",
                            asList(new DisplayTypeString(1024, "HashCode"))),
                    new LogFormat(
                            SUBSCR_MAN_NOTIFY_ALL_INVALID_EVT,
                            "VPS. SubscriptionRemoteCallbackList. "
                                    + "notifyAllListeners(): invalid event %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            SUBSCR_MAN_CANT_CALL_REMOTE_LISTENER,
                            "VPS. SubscriptionRemoteCallbackList."
                                    + " Can't call remote listener %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SUBSCR_MAN_NOTIFICATION_SUCCESS,
                            "VPS. SubscriptionRemoteCallbackList. "
                                    + "Notification Successful: listener %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SUBSCR_MAN_RECEIVED_SUBSCR_REQUEST,
                            "VPS. SubscriptionRemoteCallbackList. Receive subscription request. "
                                    + "Listener HashCode: %d",
                            asList(new DisplayTypeDecInt("HashCode"))),
                    new LogFormat(
                            SUBSCR_MAN_RECEIVED_UNSUBSCR_REQUEST,
                            "VPS. SubscriptionRemoteCallbackList. Receive unsubscription request."
                                    + " Listener HashCode: %d",
                            asList(new DisplayTypeDecInt("HashCode"))),
                    new LogFormat(
                            SUBSCR_MAN_ILLEG_ARG_EXCEPTION,
                            "VPS. SubscriptionRemoteCallbackList. "
                                    + "subscribe(): IllegalArgumentException %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_READY_NOTIFICATION,
                            "VPS. VehiclePowerServiceAdapter. ismReady(): Received from %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            REQUEST_END_EARLY_HMI_NOTIFICATION,
                            "VPS. VehiclePowerServiceAdapter. endEarlyHmi(): Received from %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            FORCED_COMPLETION_EARLY_SERVICES_NOTIFICATION,
                            "VPS. VehiclePowerServiceAdapter. forcedCompletionEarlyServices(): Received from %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SUBSCR_MAN_BROADCAST_FINISHED,
                            "VPS. SubscriptionRemoteCallbackList. " + "Broadcast finished."),
                    new LogFormat(
                            SUBSCR_MAN_ALL_LISTENERS_NOTIFIED,
                            "VPS. SubscriptionRemoteCallbackList. " + "All listeners notified."),
                    new LogFormat(SM_INIT_STATES, "VPS. StateInitializer. initStates."),
                    new LogFormat(
                            SM_INITIAL_STATE,
                            "VPS. StateInitializer. initalState : %s",
                            asList(new DisplayTypeString(1024, "State"))),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_RESTART_EVT,
                            "VPS. VpsStateMachine. Receive onAppRestart() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_RESUME_EVT,
                            "VPS. VpsStateMachine. Receive onAppResume() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_STOP_EVT,
                            "VPS. VpsStateMachine. Receive onAppStopEvent() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_RESTART_EVT,
                            "VPS. VpsStateMachine. Receive onFwRestart() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_RESUME_EVT,
                            "VPS. VpsStateMachine. Receive onFwResume() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_STOP_EVT,
                            "VPS. VpsStateMachine. Receive onFwStop() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_SERVICES_STOPPED_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onAppServicesStopCompleted() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_SERVICES_RESTART_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onEfwRestartCompleted() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_SERVICES_RESUME_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onEfwResumeCompleted() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_FW_SERVICES_STOP_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onEfwServicesStopCompleted() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_START_COLDBOOT_EVT,
                            "VPS. VpsStateMachine. Receive onAppStartColdboot() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_NORMAL_APP_STARTED_EVT,
                            "VPS. VpsStateMachine. Receive onNormalAppServicesStarted event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_LATE_APP_STARTED_EVT,
                            "VPS. VpsStateMachine. Receive onLateAppServicesStarted event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_NORMAL_EFW_SRV_STARTED_EVT,
                            "VPS. VpsStateMachine. Receive onNormalEfwServicesStarted event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_LATE_EFW_SRV_STARTED_EVT,
                            "VPS. VpsStateMachine. Receive onLateEfwServicesStarted event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_STOPPED_EVT,
                            "VPS. VpsStateMachine. Receive onAppsStopped event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_SRV_STOPPED_EVT,
                            "VPS. VpsStateMachine. Receive onServicesStopped event!"),
                    new LogFormat(
                            SM_VPS_SM_TRANS_TO_NULL_PTR_PASS,
                            "VPS. VpsStateMachine. Pass null pointer to VpsStateMachine.transitionTo() method!"),
                    new LogFormat(
                            SM_FROM_TO,
                            "VPS. VpsStateMachine. SM transition from %s",
                            asList(new DisplayTypeString(1024, "From"))),
                    new LogFormat(
                            SM_EXIT_FROM,
                            "VPS. VpsStateMachine. Exit from state: %s",
                            asList(new DisplayTypeString(1024, "State"))),
                    new LogFormat(
                            SM_ENTER_TO,
                            "VPS. VpsStateMachine. Enter to state: %s",
                            asList(new DisplayTypeString(1024, "State"))),
                    new LogFormat(
                            SM_STATE_HOLDER_RCV_PWR_CHG,
                            "VPS. StateHolder. " + "Receive power state change! State : %s",
                            asList(new DisplayTypeString(1024, "State"))),
                    new LogFormat(
                            SM_STATE_HOLDER_ERR_INT_PWR,
                            "VPS. StateHolder. Can't aplly " + "integer power state : %d",
                            asList(new DisplayTypeDecInt("State"))),
                    new LogFormat(
                            SM_STATE_HOLDER_SEND_EVT,
                            "VPS. StateHolder. Send event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            SM_STATE_HOLDER_CONV_VALUE,
                            "VPS. StateHolder. Try to convert " + "int value to EVpsState: %d",
                            asList(new DisplayTypeDecInt("State"))),
                    new LogFormat(
                            SM_ADAPTERS_PWR_CHG_H_EXEC,
                            "VPS. PowerStateChangeHandler. process(): is executed."),
                    new LogFormat(
                            SM_ADAPTERS_START_H_EXEC,
                            "VPS. StartHandler. StartHandler.process(): is executed."),
                    new LogFormat(
                            RUNNER_INV_CMD_CANT_EXEC,
                            "VPS. InvalidFormatCommand. " + "Can't execute invalid command : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_JAVA_START_SCRIPT, "VPS. JavaRunner. Start script execution!"),
                    new LogFormat(RUNNER_JAVA_START_THREAD, "VPS. JavaRunner. Start thread proc."),
                    new LogFormat(
                            RUNNER_JAVA_CREAT_BUILD,
                            "VPS. JavaRunnerCreator. " + "Build JavaRunner. Event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            RUNNER_SCRIPT_EXEC_START_LEN,
                            "VPS. ScriptExecutor. Execute "
                                    + "startup script! Script len(lines) : %d",
                            asList(new DisplayTypeDecInt("Parameter"))),
                    new LogFormat(
                            RUNNER_SCRIPT_EXEC_PARSE_CMD,
                            "VPS. ScriptExecutor. " + "Try to parse command : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_SCRIPT_EXEC_FAIL_CMD,
                            "VPS. ScriptExecutor. Failed to " + "create command from string: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_SCRIPT_EXEC_SEND_EVT,
                            "VPS. ScriptExecutor. send event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            RUNNER_SIMPLE_SLEEP_EXC,
                            "VPS. SimpleSleepImpl. The sleep interval was reduced due to the exception : %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_CREATE,
                            "VPS. SleepCommand. Create 'SLEEP' command with sleep value (ms) : %s",
                            asList(new DisplayTypeString(1024, "Time"))),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_EXEC,
                            "VPS. SleepCommand. Execute 'SLEEP' command with sleep value (nanos) : %s",
                            asList(new DisplayTypeString(1024, "Time"))),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_CR_PARSE,
                            "VPS. SleepCommandCreator. " + "Parse 'SLEEP' command from text : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_CR_PARSE_FAIL_FORM,
                            "VPS. SleepCommandCreator. Can't parse command! Invalid command format!"),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_CR_PARSE_FAIL_VAL,
                            "VPS. SleepCommandCreator. Can't parse command! Invalid sleep value!"),
                    new LogFormat(
                            RUNNER_SLEEP_CMD_CR_PARSE_FAIL_NUM,
                            "VPS. SleepCommandCreator. Can't parse sleep value (Using 0.0ms instead!): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_START_CMD_CREATE,
                            "VPS. StartCommand. " + "Create 'START' command for component : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            RUNNER_START_CMD_EXEC,
                            "VPS. StartCommand. "
                                    + "Execute 'START' commandName for component : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            RUNNER_START_CMD_FAIL,
                            "VPS. StartCommand. FAILED to " + "execute startService for '%s'",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            RUNNER_START_CMD_SRV_OK,
                            "VPS. StartCommand. startService executed successful. Return value : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            RUNNER_START_CMD_START_EXC,
                            "VPS. StartCommand. Executing " + "'START' commandName  FAILED : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_START_CMD_CR_PARSE_CMD_TXT,
                            "VPS. StartCommand. " + "Parse/commandName from text : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_VCONF_READ_ERROR,
                            "VPS. VehicleConfigReader. "
                                    + "Error reading config string using key: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_VCONF_RUN_VPS_FAIL,
                            "VPS. VehicleConfigRunner. Can't start VehiclePowerService!"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_VPS_WAIT,
                            "VPS. VehicleConfigRunner. Waiting registration for VehiclePowerService"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_BIND_NULL, "VehicleConfigRunner. Binder is null!"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_SRV_OK,
                            "VPS. VehicleConfigRunner. startService executed successful. "
                                    + "Return value : %s",
                            asList(new DisplayTypeString(1024, "Return"))),
                    new LogFormat(
                            RUNNER_VCONF_RUN_SRV_FAIL,
                            "VPS. VehicleConfigRunner. FAILED to execute startService!"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_START_FAIL,
                            "VPS. VehicleConfigRunner. Executing 'START' commandName  FAILED : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            RUNNER_VCONF_RUN_WAIT_ESM,
                            "VPS. VehicleConfigRunner. Wait EsmRegistration... (Loop)"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_DONE_ESM,
                            "VPS. VehicleConfigRunner. EsmRegistration is done!"),
                    new LogFormat(
                            RUNNER_VCONF_RUN_EXC_ESM,
                            "VPS. VehicleConfigRunner. Exception in waitEsmRegistration : %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            EVT_Q_Q_REC_EVT_ADD,
                            "VPS. EventQueue. Add event to queue: %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            EVT_Q_Q_REC_EVT_FAIL,
                            "VPS. EventQueue. Failed to add event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            EVT_Q_Q_INC_EVT_GET,
                            "VPS. EventQueue. Get event from queue: %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            EVT_Q_Q_INC_EVT_FAIL,
                            "VPS. EventQueue. Failed to get event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            EVT_Q_H_ADD_EVT,
                            "VPS. EventQueueHandler. Add event handler for event : %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(
                            EVT_Q_H_REPL_EVT,
                            "VPS. EventQueueHandler. " + "Event handler was replaced for event: %s",
                            asList(new DisplayTypeString(1024, "Event"))),
                    new LogFormat(EVT_Q_H_START, "VPS. Start EventQueueHandler."),
                    new LogFormat(EVT_Q_H_ALREADY_RUN, "VPS. EventQueueHandler already running!"),
                    new LogFormat(EVT_Q_H_ALREADY_STOP, "VPS. EventQueueHandler already stopped!"),
                    new LogFormat(EVT_Q_H_STOP, "VPS. Stop EventQueueHandler."),
                    new LogFormat(EVT_Q_H_NULL, "VPS. EventQueueHandler. Can't handle null value!"),
                    new LogFormat(
                            EVT_Q_H_FAIL,
                            "VPS. EventQueueHandler. Handle event FAILED, because handler doesn't exist: %s ",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SUBSCR_MAN_NOTIFY_ALL_SUBSCR_PWR_STATE_CHG,
                            "VPS. SubscriptionRemoteCallbackList. Notify all subscribers about "
                                    + "power state change. Power state : %s",
                            asList(new DisplayTypeString(1024, "State"))),
                    new LogFormat(
                            JSON_PARSE_ERROR_CONFIG_READER,
                            "VPS. JSON parse error at Config reader"),
                    new LogFormat(
                            IS_SERVICE_STARTED_INVALID_CLASSNAME,
                            "VPS. StartupInfoContainer. Invalid service name in isServiceStarted: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SERVICE_STARTED_INVALID_CLASSNAME,
                            "VPS. StartupInfoContainer. Invalid service name in onServiceStarted: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SERVICE_START_COMPLETE_INVALID_CLASSNAME,
                            "VPS. StartupInfoContainer. Invalid service name in serviceStartComplete: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            NOTIFY_START_COMPLETE_CALL,
                            "VPS. VehiclePowerServiceAdapter."
                                    + " notifyStartComplete called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            FAILED_TO_PARSE_COMMAND_JSON_PARSER,
                            "VPS. VehicleConfigReader. JSON parser: failed to parse command: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            UNKNOWN_PAR_CONF_FILE,
                            "VPS. VehicleConfigReader. Got unknown "
                                    + "parameter in config file: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            FAILED_TO_PARSE_JSON_FILE,
                            "VPS. VehicleConfigReader. " + "Failed to parse JSON file: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            JSONCMDPARS_FAILED_TO_PARSE,
                            "VPS. JsonCommandParser. Failed to parse JSON command with key: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            WAITCMD_WAITING_FOR,
                            "VPS. WaitCommand. waiting for %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            WAITCMDCREATOR_CANNOT_PARSE_CMD,
                            "VPS. WaitCommandCreator. Can't parse command. commandText: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            WAITCMDCREATOR_SRV_LIST,
                            "VPS. WaitCommandCreator. serviceList : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            START_COMPLETE_TIMEOUT_OCCURR,
                            "VPS. Wait start complete timeout occurred for service : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SETUP_START_COMPLETE_TIMEOUT,
                            "VPS. Setup start complete timeout value : %d for service[s] : %s",
                            asList(
                                    new DisplayTypeDecInt("Timeout"),
                                    new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            INVALID_VALUE_FOR_START_COMPLETE_TIMEOUT,
                            "VPS. Invalid start complete timeout value for service[s]: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            STOPPED_SERVICES_ERROR,
                            "VPS. Error, there are not running services: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            WAITCMD_EXEC_FAILED,
                            "VPS. Failed to execute 'wait_services_started' command. Reason : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            WAIT_STARTUP_COMPLETE,
                            "VPS. StartupInfoContainer. Waiting startup complete. Timeout : %d, Service : %s",
                            asList(
                                    new DisplayTypeDecInt("Timeout"),
                                    new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            WAIT_STARTUP_FAILED_SERVICE_DOESNT_EXIST,
                            "VPS. StartupInfoContainer. Waiting startup complete FAILED. Service : %s doesn't exist!",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            WAIT_STARTUP_FINISHED,
                            "VPS. StartupInfoContainer. Waiting startup complete finished. Service : %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            HAS_NULL_OBJ_LISTENER_SUBS,
                            "VPS. SystemUIWindowsSubscription. "
                                    + "Got null object instead of listener in subscription."),
                    new LogFormat(
                            HAS_NULL_OBJ_LISTENER_UNSUBS,
                            "VPS. SystemUIWindowsSubscription. "
                                    + "Got null object instead of listener in unsubscription."),
                    new LogFormat(
                            HAS_DISPLAY_MODE_CHANGING,
                            "VPS. SystemUiWindowsSubscription. " + "HMI display mode changed: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SIC_TRY_TO_ADD_SERVICEINFO,
                            "VPS. Try to add a ServiceInfo for service: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SIC_SERVICEINFO_WAS_ADDED_TO,
                            "VPS. ServiceInfo was added for service: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            SIC_SERVICEINFO_ALREADY_EXIST,
                            "VPS. ServiceInfo already exists in container for service: %s",
                            asList(new DisplayTypeString(1024, "Name"))),
                    new LogFormat(
                            ESM_RESTART_BROADCAST_RECEIVED,
                            "VPS. ESM restart broadcast received! ESM IBinder : %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            ESM_RESTART_BROADCAST_IS_NULL,
                            "VPS. ESM restart broadcast received! ESM IBinder is NULL."),
                    new LogFormat(
                            BNA_HMI_IS_READY, "VPS_EVENT_LOG[VPS->BOOTD] hmiIsReady event sent"),
                    new LogFormat(
                            SEND_UNMOUNT_COMPLETE_TO_VPD,
                            "VPS_EVENT_LOG[VPS->BOOTD] unmountComplete event sent"),
                    new LogFormat(
                            SEND_END_EARLY_HMI_TO_VPD,
                            "VPS_EVENT_LOG[VPS->BOOTD] endEarlyHmi event sent"),
                    new LogFormat(
                            SEND_FORCED_COMPLETION_EARLY_SERVICES_TO_VPD,
                            "VPS_EVENT_LOG[VPS->BOOTD] forcedCompletionEarlyServices event sent"),
                    new LogFormat(
                            RESUME_COMPLETE_CALL,
                            "VPS. VehiclePowerServiceAdapter." + " resumeComplete called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            RESTART_COMPLETE_CALL,
                            "VPS. VehiclePowerServiceAdapter." + " restartComplete called from: %s",
                            asList(new DisplayTypeString(1024, "Called from"))),
                    new LogFormat(
                            SEND_BROADCAST_MELCO_BOOT_COMPLETE,
                            "VPS. BroadcastMelcoBootCompleteSender" + ".sendBroadcastAsUser(): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            VPS_START_NORMAL_APP_SERVICES_STATE_ONEXIT_NOTIFY_BOOTD,
                            "VPS. StartNormalAppServicesState. onExit. Notifying BootDaemon."),
                    new LogFormat(
                            RESUME_COMPLETE_APP_CALL,
                            "VPS. VehiclePowerServiceAdapter."
                                    + " resumeCompleteApp called from: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RESTART_COMPLETE_APP_CALL,
                            "VPS. VehiclePowerServiceAdapter."
                                    + " restartCompleteApp called from: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            VPS_IS_SERVICE_STARTED,
                            "VPS. VehiclePowerServiceAdapter. isServiceStarted : %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_ALL_EFW_SERVICES_STARTED,
                            "VPS. VehiclePowerServiceAdapter. isAllEfwServicesStarted called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_NORMAL_APP_SERVICES_STARTED,
                            "VPS. VehiclePowerServiceAdapter. normalAppServicesStarted called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_ALL_APP_SERVICES_STARTED,
                            "VPS. VehiclePowerServiceAdapter. isAllAppServicesStarted called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_GET_SERVICE_STATE,
                            "VPS. VehiclePowerServiceAdapter. getServiceState: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_NORMAL_EFW_SERVICES_START_COMPLETE,
                            "VPS. VehiclePowerServiceAdapter. isNormalEfwServicesStartComplete called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_NORMAL_APP_SERVICES_START_COMPLETE,
                            "VPS. VehiclePowerServiceAdapter. isNormalAppServicesStartComplete called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_LATE_EFW_SERVICES_START_COMPLETE,
                            "VPS. VehiclePowerServiceAdapter. isLateEfwServicesStartComplete called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            VPS_IS_LATE_APP_SERVICES_START_COMPLETE,
                            "VPS. VehiclePowerServiceAdapter. isLateAppServicesStartComplete called from: %s",
                            asList(new DisplayTypeString(128, "Parameter"))),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_SERVICES_RESTART_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onAppServicesRestartComplete() event!"),
                    new LogFormat(
                            SM_RECEIVED_ON_APP_SERVICES_RESUME_COMPLETE_EVT,
                            "VPS. VpsStateMachine. Receive onAppServicesResumeComplete() event!"),
                    new LogFormat(
                            RECEIVED_APP_RESUME_FROM_VPD,
                            "VPS. VpdNativeReceiver. onAppResume call from JNI"),
                    new LogFormat(
                            RECEIVED_APP_RESTART_FROM_VPD,
                            "VPS. VpdNativeReceiver. onAppRestart call from JNI"),
                    new LogFormat(
                            SM_RECEIVED_ON_COMPLETION_TIMEOUT_EVT,
                            "VPS. VpsStateMachine. Receive onCompletionTimeout() event!"),
                    new LogFormat(
                            SERVICE_STOP_INVALID_CLASSNAME,
                            "VPS. StartupInfoContainer. Invalid service name in onStopComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_RESUME_RESTART_INVALID_CLASSNAME,
                            "VPS. StartupInfoContainer. Invalid service name in resume/restartComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_STOP_COMPLETE,
                            "VPS. StartupInfoContainer. onStopComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_RESUME_RESTART_COMPLETE,
                            "VPS. StartupInfoContainer. resume/restartComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_STOP_COMPLETE,
                            "VPS. StartupInfoContainer. onStopComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SERVICE_START_COMPLETE,
                            "VPS. StartupInfoContainer. startComplete: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_COULDNT_OBTAIN_BINDER,
                            "VPS. StorageManagerAdapter. "
                                    + "Couldn't obtain storage manager binder."),
                    new LogFormat(
                            SMA_UNMOUNTING_NULL_BINDER,
                            "VPS. StorageManagerAdapter.unmountAll: null binder."),
                    new LogFormat(
                            SMA_UNMOUNTING_ALL,
                            "VPS. StorageManagerAdapter. Unmounting all volumes."),
                    new LogFormat(
                            SMA_OBTAINING_VOLUME_LIST,
                            "VPS. StorageManagerAdapter. Obtaining volume list."),
                    new LogFormat(
                            SMA_VOLUME_LIST_OBTAINED,
                            "VPS. StorageManagerAdapter. Volume list obtained."),
                    new LogFormat(
                            SMA_UNMOUNTING_VOLUME,
                            "VPS. StorageManagerAdapter. Unmounting volume %s.",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_VOLUME_UNMOUNTED,
                            "VPS. StorageManagerAdapter. Volume %s unmounted.",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_UNMOUNTING_REMOTE_EXCEPTION,
                            "VPS. StorageManagerAdapter.unmountAll: Remote exception: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            SMA_MOUNTING_NULL_BINDER,
                            "VPS. StorageManagerAdapter.mountUnmounted: null binder."),
                    new LogFormat(
                            SMA_MOUNTING_UNMOUNTED,
                            "VPS. StorageManagerAdapter. Mounting previously unmounted volumes."),
                    new LogFormat(
                            RECEIVED_HANDOFF,
                            "VPS. VpdNativeReceiver. handOffResponsibility call from JNI"),
                    new LogFormat(
                            RECEIVED_HANDOFF_STARTUP_ANIMATION,
                            "VPS. VpdNativeReceiver. handOffResponsibilityStartupAnimationDisplayed call from JNI"),
                    new LogFormat(
                            RECEIVED_HANDOFF_STARTUP_ANIMATION_DI,
                            "VPS. VpdNativeReceiver. handOffResponsibilityStartupAnimationAndDiDisplayed call from JNI"),
                    new LogFormat(
                            SMA_MOUNTING_VOLUME,
                            "VPS. StorageManagerAdapter. Mounting volume %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_VOLUME_MOUNTED,
                            "VPS. StorageManagerAdapter. Volume %s mounted.",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_MOUNTING_REMOTE_EXCEPTION,
                            "VPS. StorageManagerAdapter.mountUnmounted: RemoteException: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            SMA_UNMOUNTING_RUNTIME_EXCEPTION,
                            "VPS. StorageManagerAdapter.unmountAll: Exception has occurred when "
                                    + "unmounting '%s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            SMA_MOUNTING_RUNTIME_EXCEPTION,
                            "VPS. StorageManagerAdapter.mountUnmounted: Exception has occurred when "
                                    + "mounting '%s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            RUNNER_VCONF_READ_FALLBACK,
                            "VPS. VehicleConfigReaderWithFallback. VPS uses fallback config file"),
                    new LogFormat(
                            ISM_NOTIFY_TO_EMPTY_LISTENER,
                            "VPS. ism.IsmSubscription.handOff. VPS uses empty listener no notify ISM"),
                    new LogFormat(
                            ISM_EXCEPTION_ISMSUBSCRIPTION_SUBSCRIBEISM,
                            "VPS. IsmSubscription. Exception was occured while notifing ISM from IsmSubscription.subscribeIsm: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF,
                            "VPS. IsmSubscription. Exception was occured while notifing ISM from IsmSubscription.handOff: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_DISPLAYED,
                            "VPS. IsmSubscription. Exception was occured while notifing ISM from IsmSubscription.handOff: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_DI_DISPLAYED,
                            "VPS. IsmSubscription. Exception was occured while notifing ISM from IsmSubscription.handOff: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_EXCEPTION_ISMSUBSCRIPTION_HANDOFF_STARTUP_AND_DI_DISPLAYED,
                            "VPS. IsmSubscription. Exception was occured while notifing ISM from IsmSubscription.handOff: %s",
                            asList(new DisplayTypeString(1024, "Exception"))),
                    new LogFormat(
                            ISM_SEND_HANDOFF,
                            "VPS. IsmSubscription. Send handOffResponsibility to ISM"),
                    new LogFormat(
                            ISM_SEND_HANDOFF_STARTUP_ANIMATION,
                            "VPS. IsmSubscription. Send handOffResponsibilityStartupAnimationDisplayed to ISM"),
                    new LogFormat(
                            ISM_SEND_HANDOFF_STARTUP_ANIMATION_DI,
                            "VPS. IsmSubscription. Send handOffResponsibilityStartupAnimationAndDiDisplayed to ISM"),
                    new LogFormat(
                            ISM_RECEIVE_NULL_LISTENER,
                            "VPS. IsmSubscription. subscribeIsm: Received empty BootDaemonStatusListener"),
                    new LogFormat(
                            CNC_ON_COMPLETE_NOT_SUBSCRIBED,
                            "VPS. CompleteNotificationCounter. Error, no prior subscription (in/from): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_SUBSCRIBE_DECLINED,
                            "VPS. CompleteNotificationCounter. Request for subscribe declined due to repeated request: (in/from): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_SUBSCRIBE_ACCEPTED,
                            "VPS. CompleteNotificationCounter. Subscription counter: %d. Request for subscribe accepted (in/from): %s",
                            asList(
                                    new DisplayTypeDecInt("Counter"),
                                    new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_UNSUBSCRIBE_ACCEPTED,
                            "VPS. CompleteNotificationCounter. Subscription counter: %d. Request for unsubscribe accepted (in/from): %s",
                            asList(
                                    new DisplayTypeDecInt("Counter"),
                                    new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_UNSUBSCRIBE_DECLINED,
                            "VPS. CompleteNotificationCounter. Request for unsubscribe declined due to not subscribed (in/from): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_RESET,
                            "VPS. CompleteNotificationCounter. Reset counter: %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_NOT_COMPLETE,
                            "VPS. CompleteNotificationCounter. No complete notification (in/from): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_COMPLETE_ALREADY_UNSUBSCRIBED,
                            "VPS. CompleteNotificationCounter. Request for complete declined due to already unsubscribed(in/from): %s",
                            asList(new DisplayTypeString(1024, "Parameter"))),
                    new LogFormat(
                            CNC_ON_COMPLETE_FROM,
                            "VPS. CompleteNotificationCounter. Subscription counter: %d. Complete counter: %d. Complete (in/from): %s",
                            asList(
                                    new DisplayTypeDecInt("Counter1"),
                                    new DisplayTypeDecInt("Counter2"),
                                    new DisplayTypeString(1024, "Parameter"))));
}
