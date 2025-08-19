/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CPUCOMCLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CPUCOMCLIENT_H_

#include "IClient.h"
#include "IConnectionControl.h"
#include "ICpuComClient.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class CpuComClient
    : public ICpuComClient
    , public IConnectionControl {
public:
    /**
     * CpuComClient ctor.
     * @param eventSender This objec is used to send events to Vehicle Power Daemon.
     */
    CpuComClient(std::unique_ptr<IClient> eventSender);

    void CpuComStarted() override;
    void startConnection() override;

private:
    std::unique_ptr<IClient> mEventSender;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CPUCOMCLIENT_H_
