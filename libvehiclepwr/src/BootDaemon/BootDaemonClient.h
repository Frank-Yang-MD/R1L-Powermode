/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDAEMONCLIENT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDAEMONCLIENT_H

#include "IBootDaemonCallback.h"
#include "IBootDaemonClient.h"
#include "IClient.h"
#include "NativeClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class BootDaemonClient final
    : public NativeClient
    , public IBootDaemonClient {
public:
    /**
     * BootDaemonClient ctor.
     * @param eventSender This object is used to send events to Vehicle Power Daemon.
     */
    explicit BootDaemonClient(std::unique_ptr<IClient> eventSender);

    ~BootDaemonClient() override;

    void onConnect() override;
    void onDisconnect() override;

    void subscribeBootdHmiClient(IBootDaemonCallback* callback) override;
    void unsubscribe() override;

    virtual void handOffResponsibility() override;
    virtual void handOffResponsibilityStartupAnimationDisplayed() override;
    virtual void handOffResponsibilityStartupAnimationAndDiDisplayed() override;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDAEMONCLIENT_H
