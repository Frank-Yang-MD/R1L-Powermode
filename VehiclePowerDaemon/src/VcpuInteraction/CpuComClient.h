/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUCOMCLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUCOMCLIENT_H_

#include <functional>
#include <memory>

#include "CommandParser.h"
#include "CpuCom.h"
#include "EVpdEvent.h"
#include "IEventReceiver.h"
#include "IVcpuInteraction.h"
#include "ResumeProcessingStartCompleteStatusReceiver.h"
#include "ShutdownProcessingStartStatusReceiver.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class CpuComClient : public IVcpuInteraction {
public:
    CpuComClient(IEventReceiver& eventReceiver,
                 std::function<std::unique_ptr<cpucom::v2::ICpuCom>()> cpuComCreator);
    virtual ~CpuComClient();

    virtual bool start() override;
    virtual void stop() override;

    virtual void sendAdtrRequest() override;
    virtual void sendStartComplete() override;
    virtual void sendShutdownCancelAccept() override;
    virtual void sendShutdownProcessingStart() override;
    virtual void sendShutdownCancelComplete() override;
    virtual void sendWdtrRequest() override;
    void sendSelfRebootTrue() override;
    virtual void sendMainStartComplete() override;

private:
    std::function<std::unique_ptr<cpucom::v2::ICpuCom>()> mCpuComCreator;
    std::unique_ptr<cpucom::v2::ICpuCom> mCpuCom;
    CommandParser mCommandParser;
    ShutdownProcessingStartStatusReceiver mShutdownProcessingStartStatusReceiver;
    ResumeProcessingStartCompleteStatusReceiver mResumeProcessingStartCompleteStatusReceiver;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUCOMCLIENT_H_
