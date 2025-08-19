/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATEHOLDER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATEHOLDER_H

#include "EVpdState.h"
#include "IStateHolder.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockStateHolder : public IStateHolder {
public:
    MOCK_CONST_METHOD0(getVpdState, common::EVpdState());
    MOCK_METHOD1(setVpdState, void(common::EVpdState));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATEHOLDER_H