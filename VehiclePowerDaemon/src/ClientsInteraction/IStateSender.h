/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATESENDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATESENDER_H_

#include "EVpdState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStateSender {
public:
    // LCOV_EXCL_START
    virtual ~IStateSender() = default;
    // LCOV_EXCL_STOP
    virtual bool sendState(int clientID, common::EVpdState state) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATESENDER_H_
