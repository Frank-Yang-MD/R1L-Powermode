/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMANDRESULT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMANDRESULT_H_

#include "IAndroidInit.h"
#include "ICommandResult.h"

#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCommandResult : public ICommandResult {
public:
    StartNativeCommandResult(const std::vector<std::string>&, const IAndroidInit&);

    bool isDone() override;

private:
    std::vector<std::string> mServices;
    const IAndroidInit& mAndroidInit;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMANDRESULT_H_
