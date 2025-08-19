/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef HIDL_GENERATED_MITSUBISHIELECTRIC_AHU_LOGDOGD_V1_0_ILOGDOGDAEMONVPDIF_H
#define HIDL_GENERATED_MITSUBISHIELECTRIC_AHU_LOGDOGD_V1_0_ILOGDOGDAEMONVPDIF_H

#include <android/hidl/base/1.0/IBase.h>

#include <hidl/HidlSupport.h>
#include <hidl/Status.h>

namespace mitsubishielectric {
namespace ahu {
namespace logdogd {
namespace V1_0 {

struct ILogdogDaemonVpdIf : public ::android::hidl::base::V1_0::IBase {
    virtual ::android::hardware::Return<int32_t> getLogSavingState() = 0;
    static ::android::sp<ILogdogDaemonVpdIf> getService(const std::string& serviceName = "default",
                                                        bool getStub = false);
};

}  // namespace V1_0
}  // namespace logdogd
}  // namespace ahu
}  // namespace mitsubishielectric

#endif  // HIDL_GENERATED_MITSUBISHIELECTRIC_AHU_LOGDOGD_V1_0_ILOGDOGDAEMONVPDIF_H
