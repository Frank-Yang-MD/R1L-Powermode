/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNATIVERUNNER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNATIVERUNNER_H

#include <gmock/gmock.h>

#include "INativeRunner.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockNativeRunner : public INativeRunner {
public:
    MOCK_METHOD0(run, void());
    MOCK_METHOD0(isDone, bool());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNATIVERUNNER_H
