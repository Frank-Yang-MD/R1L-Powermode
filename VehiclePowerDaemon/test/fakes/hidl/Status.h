/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef ANDROID_HARDWARE_STATUS_H_
#define ANDROID_HARDWARE_STATUS_H_

#include <string>

namespace android {
namespace hardware {

/**
 * @file Describes entities, defined in hidl/Status.h.
 * @note Used for testing purposes only.
 */

/**
 * @addtogroup TestFakes
 */
/** @{ */

/**
 * Base return class.
 */
struct return_status {
    bool isOk() const { return mIsOk; }
    bool isDeadObject() const { return mIsDead; }
    std::string description() const { return "description"; }

    void fakeSetIsOk(bool val) { mIsOk = val; }
    void fakeSetIsDeadObject(bool val) { mIsDead = val; }

private:
    bool mIsOk{true};
    bool mIsDead{false};
};

/**
 * Essence to provide return value through hwbinder RPC call.
 * @tparam T    Return value type
 */
template <typename T>
struct Return : public return_status {
    Return(T v)
        : mVal(v)
    {
    }

    operator T() const { return mVal; }

    Return(const Return&) = default;
    Return(Return&&) = default;
    Return& operator=(const Return&) = default;
    Return& operator=(Return&&) = default;

private:
    T mVal{};
};

/**
 * Full specialization @ref Return for methods without return value.
 */
template <>
struct Return<void> : public return_status {
};

/** @} */

}  // namespace hardware
}  // namespace android

#endif  // ANDROID_HARDWARE_STATUS_H_
