/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VehicleConfigProvider.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

namespace {
/**
 * String constants for VehicleConfig queries
 */
const std::string VP_APP_STOP_JAVA{"VPOWER_APPSTOP_JAVA_TIMEOUT"};
const std::string VP_APP_STOP_LOGDOG{"VPOWER_APPSTOP_LOGDOG_TIMEOUT"};
const std::string VP_APP_STOP_NATIVE{"VPOWER_APPSTOP_NATIVE_TIMEOUT"};
const std::string VP_APP_RESUME_EFW_TIMEOUT{"VPOWER_APPRESUME_EFW_TIMEOUT"};
const std::string VP_APP_RESUME_APP_TIMEOUT{"VPOWER_APPRESUME_APP_TIMEOUT"};
const std::string VP_LOGSAVE{"VPOWER_LOGSAVE_TIMEOUT"};
const std::string NATIVE_INIT_CONFIG{"VPM.NATIVE_SERVICES_INIT_CONFIG"};

/**
 * Default value(s) for config timeouts(ms)
 */
const int32_t VCONF_DEF_TIMEOUT = 100;

const std::string VCONF_DEF_NATIVE_INIT_CONFIG{"vendor.vpstart: externalinputkeydaemon"};
}  // namespace

/**
 * Configuration data query and shared_ptr init.
 * If any invocation of 'get' functions fails default values are set.
 */
VehicleConfigProvider::VehicleConfigProvider(const IConfigReader& rp)
    : mReader{rp}
    , mTimeoutConfig{VCONF_DEF_TIMEOUT, VCONF_DEF_TIMEOUT, VCONF_DEF_TIMEOUT,
                     VCONF_DEF_TIMEOUT, VCONF_DEF_TIMEOUT, VCONF_DEF_TIMEOUT}
{
    int javaTimeoutMs = 0;
    int logdogTimeoutMs = 0;
    int nativeTimeoutMs = 0;
    int appResumeEfwTimeoutMs = 0;
    int appResumeAppTimeoutMs = 0;
    int logSaveTimeoutMs = 0;

    if (mReader.getInt(VP_APP_STOP_JAVA, javaTimeoutMs)) {
        MLOGW(LogFuncId::VPD, LogId::VehicleConfigProvider_ctor_Cant_read_javaTimeout_parameter);
        return;
    }
    if (mReader.getInt(VP_APP_STOP_LOGDOG, logdogTimeoutMs)) {
        MLOGW(LogFuncId::VPD, LogId::VehicleConfigProvider_ctor_Cant_read_logdogTimeout_parameter);
        return;
    }
    if (mReader.getInt(VP_APP_STOP_NATIVE, nativeTimeoutMs)) {
        MLOGW(LogFuncId::VPD, LogId::VehicleConfigProvider_ctor_Cant_read_nativeTimeout_parameter);
        return;
    }
    if (mReader.getInt(VP_APP_RESUME_EFW_TIMEOUT, appResumeEfwTimeoutMs)) {
        MLOGW(LogFuncId::VPD,
              LogId::VehicleConfigProvider_ctor_Cant_read_appResumeTimeout_parameter);
        return;
    }
    if (mReader.getInt(VP_APP_RESUME_APP_TIMEOUT, appResumeAppTimeoutMs)) {
        MLOGW(LogFuncId::VPD,
              LogId::VehicleConfigProvider_ctor_Cant_read_appResumeAppTimeout_parameter);
        return;
    }
    if (mReader.getInt(VP_LOGSAVE, logSaveTimeoutMs)) {
        MLOGW(LogFuncId::VPD, LogId::VehicleConfigProvider_ctor_Cant_read_logSaveTimeout_parameter);
        return;
    }

    // multiply by 1000 to convert milliseconds to microseconds
    mTimeoutConfig.VPowerAppStopJavaTimeout = javaTimeoutMs * 1000;
    mTimeoutConfig.VPowerAppStopLogdogTimeout = logdogTimeoutMs * 1000;
    mTimeoutConfig.VPowerAppStopNativeTimeout = nativeTimeoutMs * 1000;
    mTimeoutConfig.VPowerAppResumeEfwTimeout = appResumeEfwTimeoutMs * 1000;
    mTimeoutConfig.VPowerAppResumeAppTimeout = appResumeAppTimeoutMs * 1000;
    mTimeoutConfig.VPowerLogSaveTimeout = logSaveTimeoutMs * 1000;
}

std::string VehicleConfigProvider::getInitString()
{
    std::string nativeInitConfig;

    if (mReader.getString(NATIVE_INIT_CONFIG, nativeInitConfig)) {
        MLOGW(LogFuncId::VPD,
              LogId::VehicleConfigProvider_getInitString_Cant_read_nativeInitConfig_parameter);
        return VCONF_DEF_NATIVE_INIT_CONFIG;
    }

    return nativeInitConfig;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
