/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogDaemonActions.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

LogDaemonActions::LogDaemonActions()
    : mLogDog{nullptr}
    , mSpToThis{this}
{
}

LogDaemonActions::~LogDaemonActions()
{
    if (mLogDog != nullptr) {
        mLogDog->unlinkToDeath(mSpToThis);
    }
    mLogDog.clear();
    mSpToThis.clear();
}

bool LogDaemonActions::isLogSavingComplete()
{
    MLOGI(LogFuncId::VPD, LogId::LogDaemonActions_isLogSavingComplete);

    bool returnResult = false;

    if (mLogDog == nullptr) {
        mLogDog = ILogdogDaemonVpdIf::getService();
        if (mLogDog == nullptr) {
            MLOGW(LogFuncId::VPD, LogId::LogDaemonActions_isLogSavingComplete_service_unavailable);
        }
        else {
            mLogDog->linkToDeath(mSpToThis, 0);
            MLOGI(LogFuncId::VPD, LogId::LogDaemonActions_isLogSavingComplete_mLogDog_instantiated);
        }
    }

    if (mLogDog != nullptr) {
        returnResult = static_cast<bool>(mLogDog->getLogSavingState());
        MLOGI(LogFuncId::VPD,
              LogId::LogDaemonActions_isLogSavingComplete_LogDogs_log_saving_state_is,
              returnResult ? "true" : "false");

        mLogDog->unlinkToDeath(mSpToThis);
        mLogDog.clear();
    }

    return returnResult;
}

void LogDaemonActions::serviceDied(uint64_t /*cookie*/, const wp<hidl::base::V1_0::IBase>& /*who*/)
{
    MLOGI(LogFuncId::VPD, LogId::LogDaemonActions_serviceDied_Logdog_service_died);
    mLogDog.clear();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
