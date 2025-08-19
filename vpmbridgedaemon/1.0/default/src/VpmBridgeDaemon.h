/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_V1_0_VPMHAL_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_V1_0_VPMHAL_H

#include <com/mitsubishielectric/ahu/vehiclepwrmgr/vpmbridgedaemon/1.0/IVpmBridgeDaemon.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include "AndroidPmAdapter.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {
namespace V1_0 {
namespace implementation {

using ::android::sp;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;

class VpmBridgeDaemon final : public IVpmBridgeDaemon {
public:
    VpmBridgeDaemon(AndroidPmAdapter& androidPmAdapter);

    Return<void> shutdown() override;
    Return<void> suspend() override;
    Return<void> reboot() override;
    Return<void> rebootRecovery() override;
    Return<void> wakeUp() override;

private:
    AndroidPmAdapter& mAndroidPmAdapter;
};

}  // namespace implementation
}  // namespace V1_0
}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_V1_0_VPMHAL_H
