/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IConfigReader.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockConfigReader : public IConfigReader {
public:
    MOCK_METHOD(int, getByte, (const std::string& key, uint8_t& value), (const));
    MOCK_METHOD(int, getInt, (const std::string& key, int& value), (const));
    MOCK_METHOD(int, getString, (const std::string& key, std::string& value), (const));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
