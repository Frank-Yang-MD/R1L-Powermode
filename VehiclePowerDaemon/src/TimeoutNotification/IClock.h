/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLOCK_H_

#include <chrono>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IClock {
public:
    // LCOV_EXCL_START
    virtual ~IClock() = default;
    // LCOV_EXCL_STOP

    virtual std::chrono::time_point<std::chrono::steady_clock> clock() const = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLOCK_H_
