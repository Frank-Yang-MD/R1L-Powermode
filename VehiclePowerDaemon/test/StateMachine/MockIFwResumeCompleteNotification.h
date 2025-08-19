/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIFWRESUMECOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIFWRESUMECOMPLETENOTIFICATION_H_

#include "IFwResumeCompleteNotification.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * MockIFwResumeCompleteNotification
 */
class MockIFwResumeCompleteNotification : public IFwResumeCompleteNotification {
public:
    MOCK_METHOD0(onFwResumeComplete, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIFWRESUMECOMPLETENOTIFICATION_H_
