/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_TESTI_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_TESTI_H_

#include "VehicleConfigProvider.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class GoodReader : public IConfigReader {
public:
    GoodReader() {}
    int getInt(const std::string& s, int& val) const
    {
        int ret_val = -1;
        const std::string JAVA = std::string("VPOWER_APPSTOP_JAVA_TIMEOUT");
        const std::string LOGDOG = std::string("VPOWER_APPSTOP_LOGDOG_TIMEOUT");
        const std::string NATIVE = std::string("VPOWER_APPSTOP_NATIVE_TIMEOUT");
        const std::string APP_RESUME = std::string("VPOWER_APPRESUME_EFW_TIMEOUT");
        const std::string LOGSAVE = std::string("VPOWER_LOGSAVE_TIMEOUT");
        if (JAVA.compare(s) == 0) {
            val = 700;
            ret_val = 0;
        }
        else if (LOGDOG.compare(s) == 0) {
            val = 150;
            ret_val = 0;
        }
        else if (NATIVE.compare(s) == 0) {
            val = 300;
            ret_val = 0;
        }
        else if (APP_RESUME.compare(s) == 0) {
            val = 500;
            ret_val = 0;
        }
        else if (LOGSAVE.compare(s) == 0) {
            val = 100;
            ret_val = 0;
        }
        return ret_val;
    }
};
class BadReader : public IConfigReader {
public:
    BadReader() {}
    int getInt(const std::string& s, int& val) const
    {
        const std::string JAVA = std::string("VPOWER_APPSTOP_JAVA_TIMEOUT");
        const std::string LOGDOG = std::string("VPOWER_APPSTOP_LOGDOG_TIMEOUT");
        const std::string NATIVE = std::string("VPOWER_APPSTOP_NATIVE_TIMEOUT");
        const std::string APP_RESUME = std::string("VPOWER_APPRESUME_EFW_TIMEOUT");
        const std::string LOGSAVE = std::string("VPOWER_LOGSAVE_TIMEOUT");
        int ret_val = -1;
        if (JAVA.compare(s) == 0) {
            ret_val = 0;
        }
        else if (LOGDOG.compare(s) == 0) {
            val = -199;
            ret_val = -1;
        }
        else if (NATIVE.compare(s) == 0) {
            val = 300;
            ret_val = 0;
        }
        else if (NATIVE.compare(s) == 0) {
            val = 500;
            ret_val = 0;
        }
        else if (LOGSAVE.compare(s) == 0) {
            val = 100;
            ret_val = 0;
        }
        return ret_val;
    }
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_TESTI_H_
