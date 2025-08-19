/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IAndroidInit.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockAndroidInit : public IAndroidInit {
public:
    MOCK_METHOD(int, propertyGet, (const std::string&, std::string&), (const));
    MOCK_METHOD(int, propertySet, (const std::string&, const std::string&), (const));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
