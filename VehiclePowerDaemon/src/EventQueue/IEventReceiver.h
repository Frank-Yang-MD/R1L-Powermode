/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVER_H_

#include <EVpdEvent.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Writer interface for Queue
 */
class IEventReceiver {
public:
    // LCOV_EXCL_START
    virtual ~IEventReceiver() = default;
    // LCOV_EXCL_STOP

    virtual void onEventReceive(int id, common::EVpdEvent event) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVER_H_
