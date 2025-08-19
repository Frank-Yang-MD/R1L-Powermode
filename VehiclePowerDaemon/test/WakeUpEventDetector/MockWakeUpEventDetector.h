/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKWAKEUPEVENTDETECTOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKWAKEUPEVENTDETECTOR_H_

#include <IWakeUpEventDetector.h>

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockWakeUpEventDetector : public IWakeUpEventDetector {
public:
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKWAKEUPEVENTDETECTOR_H_
