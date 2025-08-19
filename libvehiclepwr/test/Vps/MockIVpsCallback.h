/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPSCALLBACK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPSCALLBACK_H_

#include <gmock/gmock.h>

#include "IVpsCallback.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class MockIVpsCallback : public IVpsCallback {
public:
    MOCK_METHOD0(onFwResume, void());
    MOCK_METHOD0(onFwRestart, void());
    MOCK_METHOD0(onFwStop, void());
    MOCK_METHOD0(onTimeOutError, void());
    MOCK_METHOD0(requestMount, void());
    MOCK_METHOD0(requestUnmount, void());
    MOCK_METHOD0(handOffResponsibility, void());
    MOCK_METHOD0(handOffResponsibilityStartupAnimationDisplayed, void());
    MOCK_METHOD0(handOffResponsibilityDiDisplayed, void());
    MOCK_METHOD0(handOffResponsibilityStartupAnimationAndDiDisplayed, void());
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPSCALLBACK_H_
