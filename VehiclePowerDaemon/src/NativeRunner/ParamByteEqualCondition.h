/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_PARAMBYTEEQUALCONDITION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_PARAMBYTEEQUALCONDITION_H_

#include "ICondition.h"
#include "IConfigReader.h"

#include <cstdint>
#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ParamByteEqualCondition : public ICondition {
public:
    ParamByteEqualCondition(const std::string&, uint8_t, const IConfigReader&);

    bool isSatisfied() override;

private:
    const std::string& mParamName;
    const uint8_t mParamValue;
    const IConfigReader& mConfigReader;
    bool mIsSatisfied;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_PARAMBYTEEQUALCONDITION_H_
