/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONACTIONS_H_

#include "ILogDaemonActions.h"

#include <mitsubishielectric/ahu/logdogd/1.0/ILogdogDaemonVpdIf.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace android;
using namespace ::mitsubishielectric::ahu::logdogd::V1_0;

class LogDaemonActions
    : public ILogDaemonActions
    , public hardware::hidl_death_recipient {
public:
    LogDaemonActions();
    virtual ~LogDaemonActions() override;

    virtual bool isLogSavingComplete() override;

    virtual void serviceDied(uint64_t cookie, const wp<hidl::base::V1_0::IBase>& who) override;

private:
    sp<ILogdogDaemonVpdIf> mLogDog;
    sp<hardware::hidl_death_recipient> mSpToThis;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONACTIONS_H_
