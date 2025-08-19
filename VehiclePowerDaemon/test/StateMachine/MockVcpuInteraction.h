/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "IVcpuInteraction.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVcpuInteraction : public IVcpuInteraction {
public:
    MOCK_METHOD0(start, bool());
    MOCK_METHOD0(stop, void());

    MOCK_METHOD0(sendAdtrRequest, void());
    MOCK_METHOD0(sendStartComplete, void());
    MOCK_METHOD0(sendShutdownCancelAccept, void());
    MOCK_METHOD0(sendShutdownProcessingStart, void());
    MOCK_METHOD0(sendWdtrRequest, void());
    MOCK_METHOD0(sendSelfRebootTrue, void());
    MOCK_METHOD0(sendMainStartComplete, void());
    MOCK_METHOD0(sendShutdownCancelComplete, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
