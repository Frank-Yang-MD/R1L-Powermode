/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTHOLDER_H_

#include "IClient.h"

#include "BootdNotificationHandler.h"
#include "NativeClientNotificationHandler.h"
#include "VpsNotificationHandler.h"

#include "NotificationReceiver.h"
#include "VpdMessageClient.h"

#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class ClientHolder final : public IClient {
public:
    ClientHolder();

    void sendVpdEvent(common::EVpdEvent inEVpdEvent) override;
    common::EVpdState getVpdState() override;
    void setBootDaemonCallback(IBootDaemonCallback* inBootdCallback) override;
    void setInternalCallback(IInternalCallback*) override;
    void setNativeCallback(INativeCallback* inNativeCallback) override;
    void setVpsCallback(IVpsCallback* inVpsCallback) override;
    void startConnection() override;

private:
    void configureHandler(const std::vector<common::EVpdNotifications>&, IVpdNotificationHandler*);
    void configureHandlers();

    VpdMessageClient mVpdMessageClient;
    NotificationReceiver mNotificationReceiver;

    BootdNotificationHandler mBootdNotificationHandler;
    NativeClientNotificationHandler mNativeClientNotificationHandler;
    VpsNotificationHandler mVpsNotificationHandler;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTHOLDER_H_
