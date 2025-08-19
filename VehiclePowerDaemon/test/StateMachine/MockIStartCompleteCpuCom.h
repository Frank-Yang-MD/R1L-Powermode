/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_ISTARTCOMPLETECPUCOMMOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_ISTARTCOMPLETECPUCOMMOCK_H_

#include "IStartCompleteCpuCom.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockIStartCompleteCpuCom : public IStartCompleteCpuCom {
public:
    MOCK_METHOD(void, onStartCompleteCpuCom, ());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_ISTARTCOMPLETECPUCOMMOCK_H_
