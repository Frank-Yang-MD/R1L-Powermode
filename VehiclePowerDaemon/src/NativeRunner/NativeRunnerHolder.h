/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERHOLDER_H_

#include "AndroidInit.h"
#include "ConfigScriptCmdCreator.h"
#include "IConfigReader.h"
#include "INativeRunnerHolder.h"
#include "NativeConfigProvider.h"
#include "NativeRunnerManagerSequential.h"
#include "VehicleConfigProvider.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class NativeRunnerHolder : public INativeRunnerHolder {
public:
    NativeRunnerHolder(IVehicleConfigProvider&, const IConfigReader&, IAndroidInit&);

    void run() override;
    bool isDone() override;

    int propertySet(const std::string&, const std::string&) const override;
    int propertyGet(const std::string&, std::string&) const override;

private:
    NativeConfigProvider mConfigProvider;
    IAndroidInit& mAndroidInit;
    ConfigScriptCmdCreator mConfigScriptCmdCreator;
    NativeRunnerManagerSequential mNativeServiceRunner;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERHOLDER_H_
