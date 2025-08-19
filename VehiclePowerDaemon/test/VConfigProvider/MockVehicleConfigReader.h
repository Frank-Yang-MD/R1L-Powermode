/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGREADER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGREADER_H_

#include "IConfigReader.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVehicleConfigReader : public IConfigReader {
public:
    MOCK_CONST_METHOD2(getByte, int(const std::string&, uint8_t&));
    MOCK_CONST_METHOD2(getInt, int(const std::string&, int32_t&));
    MOCK_CONST_METHOD2(getString, int(const std::string&, std::string&));
};

class MockVehicleConfigReaderOverride : public IConfigReader {
public:
    int getByte(const std::string&, uint8_t& value) const override
    {
        value = 1;
        return 0;
    }

    MOCK_CONST_METHOD2(getInt, int(const std::string&, int32_t&));
    MOCK_CONST_METHOD2(getString, int(const std::string&, std::string&));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGREADER_H_
