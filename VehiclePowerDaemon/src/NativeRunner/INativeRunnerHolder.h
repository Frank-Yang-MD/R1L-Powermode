/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNERHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNERHOLDER_H_

#include "IAndroidInit.h"
#include "INativeRunner.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INativeRunnerHolder
    : public INativeRunner
    , public IAndroidInit {
public:
    // LCOV_EXCL_START
    virtual ~INativeRunnerHolder() = default;
    // LCOV_EXCL_STOP
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNERHOLDER_H_
