/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIBOOTDAEMONCALLBACK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIBOOTDAEMONCALLBACK_H_

#include <gmock/gmock.h>

#include "IBootDaemonCallback.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class MockIBootDaemonCallback : public IBootDaemonCallback {
public:
    MOCK_METHOD1(onHMIEvent, void(common::EHmiEvent event));
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIBOOTDAEMONCALLBACK_H_
