/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIAPPRESUMECOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKIAPPRESUMECOMPLETENOTIFICATION_H_

#include "IAppResumeCompleteNotification.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * MockIAppResumeCompleteNotification
 */
class MockIAppResumeCompleteNotification : public IAppResumeCompleteNotification {
public:
    MOCK_METHOD0(onAppResumeComplete, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKAPPWRESUMECOMPLETENOTIFICATION_H_
