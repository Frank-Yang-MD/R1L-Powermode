/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSCLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSCLIENT_H_

#include "IClient.h"
#include "INativeCallback.h"
#include "IVpsCallback.h"
#include "IVpsClient.h"
#include "NativeClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class VpsClient final
    : public NativeClient
    , public IVpsClient {
public:
    /**
     * VpsClient ctor.
     * @param client Object that implements IClient interface.
     */
    VpsClient(std::unique_ptr<IClient> client);
    ~VpsClient() override;

    void startProcessingComplete() override;
    void startProcessingFailed() override;
    void appStopProcessingComplete() override;
    void fwRestartProcessingComplete() override;
    void fwResumeProcessingComplete() override;
    void appRestartProcessingComplete() override;
    void appResumeProcessingComplete() override;
    void fwStopProcessingComplete() override;
    void stopProcessingFailed() override;
    void unmountComplete() override;
    void endEarlyHmi() override;
    void forcedCompletionEarlyServices() override;

    void subscribeVps(IVpsCallback* vpsCallback) override;
    void unsubscribe() override;
    void onConnect() override;
    void onDisconnect() override;

    void vpsReady() override;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSCLIENT_H_
