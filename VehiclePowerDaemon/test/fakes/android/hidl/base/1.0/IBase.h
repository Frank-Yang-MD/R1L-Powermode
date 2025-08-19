/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef ANDROID_HIDL_BASE_V1_0_IBASE_H_
#define ANDROID_HIDL_BASE_V1_0_IBASE_H_

#include <hidl/HidlSupport.h>
#include <hidl/Status.h>
#include <utils/RefBase.h>

namespace android {
namespace hidl {
namespace base {
namespace V1_0 {

/**
 * @addtogroup TestFakes
 */
/**
 * @brief Base class for entities generated from HAL interfaces.
 */
struct IBase : virtual public android::RefBase {
    virtual android::hardware::Return<bool> linkToDeath(
        const android::sp<android::hardware::hidl_death_recipient>& deathRecipient,
        uint64_t)
    {
        mDeathRecipient = deathRecipient;
        return true;
    }

    virtual android::hardware::Return<bool> unlinkToDeath(
        const android::sp<android::hardware::hidl_death_recipient>&)
    {
        mDeathRecipient = nullptr;
        return true;
    }

    void fakeServiceDied()
    {
        if (mDeathRecipient.get()) {
            mDeathRecipient->serviceDied(0, this);
        }
    }

    void fakeServiceDiedAndCleared()
    {
        if (mDeathRecipient.get()) {
            mDeathRecipient->serviceDied(0, nullptr);
        }
    }

private:
    android::sp<android::hardware::hidl_death_recipient> mDeathRecipient;
};

}  // namespace V1_0
}  // namespace base
}  // namespace hidl
}  // namespace android

#endif  // ANDROID_HIDL_BASE_V1_0_IBASE_H_
