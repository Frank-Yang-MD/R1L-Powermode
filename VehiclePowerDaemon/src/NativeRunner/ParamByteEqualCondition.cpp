#include "ParamByteEqualCondition.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ParamByteEqualCondition::ParamByteEqualCondition(const std::string& paramName,
                                                 uint8_t paramValue,
                                                 const IConfigReader& configReader)
    : mParamName{paramName}
    , mParamValue{paramValue}
    , mConfigReader{configReader}
{
    uint8_t configValue{};
    if (mConfigReader.getByte(mParamName, configValue)) {
        MLOGW(LogFuncId::VPD,
              LogId::ParamByteEqualCondition_ctor_Cant_read_Byte_parameter_from_config);
        mIsSatisfied = false;
    }
    else {
        mIsSatisfied = mParamValue == configValue;
    }

    if (!mIsSatisfied) {
        MLOGW(LogFuncId::VPD,
              LogId::ParamByteEqualCondition_ctor_Condition_is_not_satisfied_expected_actual,
              mParamValue, configValue);
    }
}

bool ParamByteEqualCondition::isSatisfied() { return mIsSatisfied; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
