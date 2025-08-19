/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IKernelModulesControl.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockKernelModulesControl : public IKernelModulesControl {
public:
    MOCK_METHOD(void, loadModules, ());
    MOCK_METHOD(bool, isModulesLoaded, ());
    MOCK_METHOD(void, unloadModules, ());
    MOCK_METHOD(bool, isModulesUnloaded, ());
    MOCK_METHOD(bool, skipModulesUnloadedCheck, ());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
