/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ConfigReader.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ConfigReader::ConfigReader(std::unique_ptr<vehicleconfig::IVehicleConfigReader> reader)
    : mReader{std::move(reader)}
{
}

int ConfigReader::getByte(const std::string& key, uint8_t& value) const
{
    return mReader ? mReader->getByte(key, value) : -1;
}

int ConfigReader::getInt(const std::string& key, int& value) const
{
    if (!mReader) {
        return -1;
    }
    return mReader->getInt(key, value);
}

int ConfigReader::getString(const std::string& key, std::string& value) const
{
    if (!mReader) {
        return -1;
    }
    return mReader->getString(key, value);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
