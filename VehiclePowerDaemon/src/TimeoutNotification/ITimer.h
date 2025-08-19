/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ITimer {
public:
    // LCOV_EXCL_START
    virtual ~ITimer() = default;
    // LCOV_EXCL_STOP

    virtual void start() = 0;
    virtual bool isExpired() const = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMER_H_
