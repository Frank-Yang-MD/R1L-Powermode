/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ParamStringEqualCondition.h"
#include "IConfigReader.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ParamStringEqualCondition::ParamStringEqualCondition(const std::string& paramName,
                                                     const std::string& paramValue,
                                                     const IConfigReader& configReader)
    : mParamName{paramName}
    , mParamValue{paramValue}
    , mConfigReader{configReader}
    , mIsSatisfied{false}
{
    std::string configValue;

    if (mConfigReader.getString(mParamName, configValue)) {
        MLOGW(LogFuncId::VPD,
              LogId::ParamStringEqualCondition_ctor_Cant_read_String_parameter_from_config);
    }
    else {
        mIsSatisfied = (mParamValue == configValue);
    }

    if (!mIsSatisfied) {
        MLOGW(LogFuncId::VPD,
              LogId::ParamStringEqualCondition_ctor_Condition_is_not_satisfied_expected_actual,
              mParamValue, configValue);
    }
}

bool ParamStringEqualCondition::isSatisfied() { return mIsSatisfied; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
