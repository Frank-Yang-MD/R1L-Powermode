/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDCLOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDCLOCK_H_

#include "IClock.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpdClock : public IClock {
public:
    virtual std::chrono::time_point<std::chrono::steady_clock> clock() const override;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDCLOCK_H_
