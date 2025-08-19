/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeRunnerHolder.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

NativeRunnerHolder::NativeRunnerHolder(IVehicleConfigProvider& configProvider,
                                       const IConfigReader& configReader,
                                       IAndroidInit& androidInit)
    : mConfigProvider(configProvider)
    , mAndroidInit(androidInit)
    , mConfigScriptCmdCreator(mAndroidInit, configReader)
    , mNativeServiceRunner(mConfigProvider, mConfigScriptCmdCreator)
{
}

void NativeRunnerHolder::run() { mNativeServiceRunner.run(); }

bool NativeRunnerHolder::isDone() { return mNativeServiceRunner.isDone(); }

int NativeRunnerHolder::propertySet(const std::string& name, const std::string& value) const
{
    return mAndroidInit.propertySet(name, value);
}
int NativeRunnerHolder::propertyGet(const std::string& name, std::string& value) const
{
    return mAndroidInit.propertyGet(name, value);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
