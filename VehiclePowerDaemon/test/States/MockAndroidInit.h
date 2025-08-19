/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDINIT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDINIT_H

#include <gmock/gmock.h>

#include "IAndroidInit.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockAndroidInit : public IAndroidInit {
public:
    MOCK_CONST_METHOD2(propertySet, int(const std::string&, const std::string&));
    MOCK_CONST_METHOD2(propertyGet, int(const std::string&, std::string&));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDINIT_H
