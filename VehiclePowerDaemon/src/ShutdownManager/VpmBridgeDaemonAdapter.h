/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHALADAPTER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHALADAPTER_H_

#include "IAndroidPowerManager.h"

#include <com/mitsubishielectric/ahu/vehiclepwrmgr/vpmbridgedaemon/1.0/IVpmBridgeDaemon.h>

using namespace android;
using com::mitsubishielectric::ahu::vehiclepwrmgr::vpmbridgedaemon::V1_0::IVpmBridgeDaemon;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpmBridgeDaemonAdapter final : public IAndroidPowerManager {
public:
    VpmBridgeDaemonAdapter() = default;
    virtual ~VpmBridgeDaemonAdapter() = default;

    virtual void shutdown() override;
    virtual void suspend() override;
    virtual void reboot() override;
    virtual void rebootRecovery() override;
    virtual void wakeUp() override;

private:
    sp<IVpmBridgeDaemon> mVpmBridgeDaemon;

    bool acquireVpmBridgeDaemon();
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHALADAPTER_H_
