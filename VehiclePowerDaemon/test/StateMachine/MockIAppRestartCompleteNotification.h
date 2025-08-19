/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIAPPRESTARTCOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIAPPRESTARTCOMPLETENOTIFICATION_H_

#include "IAppRestartCompleteNotification.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * MockIAppRestartCompleteNotification
 */
class MockIAppRestartCompleteNotification : public IAppRestartCompleteNotification {
public:
    MOCK_METHOD0(onAppRestartComplete, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIAPPRESTARTCOMPLETENOTIFICATION_H_
