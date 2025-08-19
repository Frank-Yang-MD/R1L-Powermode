/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIG_READER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIG_READER_H_

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IConfigReader {
public:
    // LCOV_EXCL_START
    virtual ~IConfigReader() = default;
    // LCOV_EXCL_STOP

    virtual int getByte(const std::string& key, uint8_t& value) const = 0;
    virtual int getInt(const std::string& key, int& value) const = 0;
    virtual int getString(const std::string& key, std::string& value) const = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIG_READER_H_
