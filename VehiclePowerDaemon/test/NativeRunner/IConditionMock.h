/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITIONMOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITIONMOCK_H_

#include <gmock/gmock.h>

#include "ICondition.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IConditionMock : public ICondition {
public:
    MOCK_METHOD0(isSatisfied, bool());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITIONMOCK_H_
