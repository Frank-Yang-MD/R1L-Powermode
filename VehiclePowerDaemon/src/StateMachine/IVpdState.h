/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATE_H_

#include "IAppRestartCompleteNotification.h"
#include "IAppResumeCompleteNotification.h"
#include "IDisconnectNotification.h"
#include "IFwRestartCompleteNotification.h"
#include "IFwResumeCompleteNotification.h"
#include "IPowerStateChangeListener.h"
#include "IResumeProcessingStartCompleteSent.h"
#include "IShutdownProcessingStartSent.h"
#include "IStartCompleteCpuCom.h"
#include "IStartCompleteVps.h"
#include "IStopCompleteLog.h"
#include "IStopCompleteNative.h"
#include "IStopCompleteVps.h"
#include "ISubscriptionNotification.h"
#include "ITimeoutNotification.h"
#include "IVpsMount.h"
#include "IWakeUpNotification.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVpdState
    : public IStartCompleteCpuCom
    , public IStartCompleteVps
    , public IPowerStateChangeListener
    , public IStopCompleteNative
    , public IStopCompleteLog
    , public ITimeoutNotification
    , public IStopCompleteVps
    , public IVpsMount
    , public ISubscriptionNotification
    , public IFwResumeCompleteNotification
    , public IFwRestartCompleteNotification
    , public IAppResumeCompleteNotification
    , public IAppRestartCompleteNotification
    , public IShutdownProcessingStartSent
    , public IResumeProcessingStartCompleteSent
    , public IWakeUpNotification
    , public IDisconnectNotification {
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATE_H_
