/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIG_READER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIG_READER_H_

#include "IConfigReader.h"
#include "IVehicleConfigReader.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ConfigReader final : public IConfigReader {
public:
    explicit ConfigReader(std::unique_ptr<vehicleconfig::IVehicleConfigReader> reader);

    int getByte(const std::string& key, uint8_t& value) const override;
    int getInt(const std::string& key, int& value) const override;
    int getString(const std::string& key, std::string& value) const override;

private:
    std::unique_ptr<vehicleconfig::IVehicleConfigReader> mReader;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIG_READER_H_
