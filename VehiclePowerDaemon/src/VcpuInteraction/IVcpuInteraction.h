/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUINTERACTION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUINTERACTION_H_

#include "ICpuComControl.h"
#include "IVcpuActions.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Common interface for interaction with VCPU via CpuCom
 */
class IVcpuInteraction
    : public ICpuCommControl
    , public IVcpuActions {
public:
    // LCOV_EXCL_START
    virtual ~IVcpuInteraction() = default;
    // LCOV_EXCL_STOP
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUINTERACTION_H_
