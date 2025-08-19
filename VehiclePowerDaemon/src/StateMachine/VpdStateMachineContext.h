/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDSTATEMACHINECONTEXT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDSTATEMACHINECONTEXT_H_

#include "IVpdStateMachineContext.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpdStateMachineContext : public IVpdStateMachineContext {
public:
    VpdStateMachineContext() = default;

    void setVpsDisconnected(const bool) override;

    bool isVpsDisconnected() override;

    void setVpsStartCompleted(bool) override;

    bool isVpsStartCompleted() override;

private:
    bool mIsVpsDisconnected = true;
    bool mIsVpsStartCompleted = false;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDSTATEMACHINECONTEXT_H_
