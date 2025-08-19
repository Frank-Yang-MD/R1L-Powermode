/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATEMACHINE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATEMACHINE_H_

#include "IVpdStateMachineContext.h"

#include <chrono>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpdStateBase;

class IVpdStateMachine {
public:
    // LCOV_EXCL_START
    virtual ~IVpdStateMachine() = default;
    // LCOV_EXCL_STOP

    virtual void transitionTo(VpdStateBase*) = 0;

    virtual IVpdStateMachineContext& context() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDSTATEMACHINE_H_
