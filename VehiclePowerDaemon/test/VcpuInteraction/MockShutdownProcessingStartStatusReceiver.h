/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSHUTDOWNPROCESSINGSTATUSRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSHUTDOWNPROCESSINGSTATUSRECEIVER_H_

#include "ShutdownProcessingStartStatusReceiver.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockShutdownProcessingStartStatusReceiver {
public:
    MOCK_METHOD1(onDeliveryStatus, void(bool));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSHUTDOWNPROCESSINGSTATUSRECEIVER_H_
