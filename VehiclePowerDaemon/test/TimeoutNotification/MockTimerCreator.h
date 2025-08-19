/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKTIMERCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKTIMERCREATOR_H_

#include <ITimerCreator.h>

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockTimerCreator : public ITimerCreator {
public:
    MOCK_METHOD1(createTimer, ITimer*(const std::chrono::microseconds&));
    MOCK_METHOD0(getAbnormalHandler, IVcpuResetTimer*());
    MOCK_METHOD0(getWatchDogHandler, IVcpuResetTimer*());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKTIMERCREATOR_H_
