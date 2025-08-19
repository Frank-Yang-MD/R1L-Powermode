/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSM_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSM_H_

#include <gmock/gmock.h>

#include "IVpdStateMachine.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockSM : public IVpdStateMachine {
public:
    MOCK_METHOD1(transitionTo, void(VpdStateBase*));

    MOCK_METHOD0(context, IVpdStateMachineContext&());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSM_H_
