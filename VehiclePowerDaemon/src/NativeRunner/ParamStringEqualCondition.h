/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_PARAMSTRINGEQUALCONDITION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_PARAMSTRINGEQUALCONDITION_H_

#include "ICondition.h"

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IConfigReader;

/**
 * @brief The ParamStringEqualCondition class is responsible for
 * create conditional to start binary file. For property variable name
 * using string type.
 */
class ParamStringEqualCondition : public ICondition {
public:
    ParamStringEqualCondition(const std::string& paramName,
                              const std::string& paramValue,
                              const IConfigReader& configReader);

    /**
     * @brief isSatisfied return true if value from VehicleConfig
     * is equals given paramValue in constructor.
     */
    bool isSatisfied() override;

private:
    const std::string& mParamName;
    const std::string& mParamValue;
    const IConfigReader& mConfigReader;

    bool mIsSatisfied;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_PARAMSTRINGEQUALCONDITION_H_
