/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LOGDOGCLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LOGDOGCLIENT_H_

#include "NativeClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class LogdogClient : public NativeClient {
public:
    /**
     * LogdogClient ctor.
     * @param client Pointer to an object which implements IClient interface.
     */
    LogdogClient(std::unique_ptr<IClient> pClient);
    virtual ~LogdogClient();

    void stopProcessingComplete() override;
    void subscribe(INativeCallback* callback) override;
    void unsubscribe() override;
    void onConnect() override;
    void onDisconnect() override;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LOGDOGCLIENT_H_
