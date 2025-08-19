/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef ANDROID_HIDL_SUPPORT_H
#define ANDROID_HIDL_SUPPORT_H

#include <array>
#include <string>
#include <vector>

#include <utils/RefBase.h>

namespace android {
namespace hidl {
namespace base {
namespace V1_0 {
struct IBase;
};  // namespace V1_0
};  // namespace base
};  // namespace hidl

namespace hardware {

// hidl_death_recipient is a callback interface that can be used with
// linkToDeath() / unlinkToDeath()
struct hidl_death_recipient : public virtual RefBase {
    virtual void serviceDied(uint64_t cookie,
                             const android::wp<android::hidl::base::V1_0::IBase>& who) = 0;
};

using hidl_string = std::string;

template <typename T>
using hidl_vec = std::vector<T>;

template <typename T, size_t N>
using hidl_array = std::array<T, N>;

}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HIDL_SUPPORT_H
