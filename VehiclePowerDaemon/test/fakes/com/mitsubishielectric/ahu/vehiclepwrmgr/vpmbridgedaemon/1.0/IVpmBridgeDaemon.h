/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef HIDL_GENERATED_COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMBRIDGEDAEMON_V1_0_IVPMBRIDGEDAEMON_H
#define HIDL_GENERATED_COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMBRIDGEDAEMON_V1_0_IVPMBRIDGEDAEMON_H

#include <android/hidl/base/1.0/IBase.h>

#include <hidl/HidlSupport.h>
#include <hidl/Status.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {
namespace V1_0 {

struct IVpmBridgeDaemon : public ::android::hidl::base::V1_0::IBase {
    virtual ::android::hardware::Return<void> shutdown() = 0;
    virtual ::android::hardware::Return<void> suspend() = 0;
    virtual ::android::hardware::Return<void> reboot() = 0;
    virtual ::android::hardware::Return<void> rebootRecovery() = 0;
    virtual ::android::hardware::Return<void> wakeUp() = 0;
    static ::android::sp<IVpmBridgeDaemon> getService(const std::string& serviceName = "default",
                                                      bool getStub = false);
};

}  // namespace V1_0
}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // HIDL_GENERATED_COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMBRIDGEDAEMON_V1_0_IVPMBRIDGEDAEMON_H
