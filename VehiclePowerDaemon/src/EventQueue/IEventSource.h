/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTSOURCE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTSOURCE_H_

#include "EVpdEvent.h"
#include <utility>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Reader interface for queue
 */
class IEventSource {
public:
    // LCOV_EXCL_START
    virtual ~IEventSource() = default;
    // LCOV_EXCL_STOP

    virtual std::pair<int, common::EVpdEvent> getEvent() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTSOURCE_H_
