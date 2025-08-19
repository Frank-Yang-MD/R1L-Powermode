/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCPUCOMMCONTROL_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCPUCOMMCONTROL_H

#include <gmock/gmock.h>

#include "ICpuComControl.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockCpuCommControl : public ICpuCommControl {
public:
    MOCK_METHOD0(start, bool());
    MOCK_METHOD0(stop, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCPUCOMMCONTROL_H
