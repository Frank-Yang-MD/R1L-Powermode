/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL

#include "Log.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {

using com::mitsubishielectric::ahu::common::MLOGD;
using com::mitsubishielectric::ahu::common::MLOGE;
using com::mitsubishielectric::ahu::common::MLOGF;
using com::mitsubishielectric::ahu::common::MLOGI;
using com::mitsubishielectric::ahu::common::MLOGV;
using com::mitsubishielectric::ahu::common::MLOGW;

namespace LogFuncId {
using common::FunctionID;
const FunctionID VPBD = FunctionID::vehicle_pwr_bd;
}  // namespace LogFuncId

enum LogId {
    Main_Started_successfully = 500,
    Main_Cannot_register_service = 501,

    VpmBridgeDaemon_API_call = 502,

    AndroidPmAdapter_Android_PM_binder_is_null = 503,
    AndroidPmAdapter_Cannot_cast_to_Android_PM_instance = 504,
    AndroidPmAdapter_Android_PM_instance_obtained = 505,
    AndroidPmAdapter_Cannot_obtain_Android_PM_instance = 506,
    AndroidPmAdapter_Request_to_Android_PM = 507,
    AndroidPmAdapter_Request_to_Android_PM_failed = 508,
};

void InitializeLogMessages();
void TerminateLogMessages();

}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL
