/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDSTATEMACHINECONTEXT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDSTATEMACHINECONTEXT_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * IVpdStateMachineContext
 */
class IVpdStateMachineContext {
public:
    // LCOV_EXCL_START
    virtual ~IVpdStateMachineContext() = default;
    // LCOV_EXCL_STOP

    virtual void setVpsDisconnected(const bool) = 0;

    virtual bool isVpsDisconnected() = 0;

    virtual void setVpsStartCompleted(bool) = 0;

    virtual bool isVpsStartCompleted() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDSTATEMACHINECONTEXT_H_
