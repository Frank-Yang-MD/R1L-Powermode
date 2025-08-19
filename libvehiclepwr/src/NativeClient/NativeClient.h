/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENT_H_

#include "EVpdState.h"
#include "IClient.h"
#include "IConnectionControl.h"
#include "IInternalCallback.h"
#include "INativeClient.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class NativeClient
    : public virtual INativeClient
    , public IInternalCallback
    , public IConnectionControl {
public:
    /**
     * NativeClient ctor.
     * @param client Pointer to an object which implements IClient interface.
     */
    NativeClient(std::unique_ptr<IClient> client);
    ~NativeClient() override;

    common::EVpdState GetVPDState() override;
    void onConnect() override;
    void onDisconnect() override;
    void stopProcessingComplete() override;
    void subscribe(INativeCallback* callback) override;
    void unsubscribe() override;
    void startConnection() override;

protected:
    std::unique_ptr<IClient> mClient;  //!< object that implements IClient interface.
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENT_H_
