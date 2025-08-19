/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "INativeRunnerHolder.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockNativeRunnerHolder : public INativeRunnerHolder {
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD0(isDone, bool());

    MOCK_CONST_METHOD2(propertySet, int(const std::string&, const std::string&));
    MOCK_CONST_METHOD2(propertyGet, int(const std::string&, std::string&));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
