/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report
// because it is just holder for other class instances.

#include "ClientHolder.h"

#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

static const std::vector<common::EVpdNotifications> NATIVE_CLIENT_NOTITIFICATIONS = {
    common::EVpdNotifications::AppResume,  common::EVpdNotifications::AppStartColdboot,
    common::EVpdNotifications::AppStartNa, common::EVpdNotifications::AppStop,
    common::EVpdNotifications::AppRestart,
};

static const std::vector<common::EVpdNotifications> VPS_NOTITIFICATIONS = {
    common::EVpdNotifications::FwStop,
    common::EVpdNotifications::FwResume,
    common::EVpdNotifications::FwRestart,
    common::EVpdNotifications::TimeOutError,
    common::EVpdNotifications::RequestUnmount,
    common::EVpdNotifications::RequestMount,
    common::EVpdNotifications::HandOffResponsibility,
    common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed,
    common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed,
};

static const std::vector<common::EVpdNotifications> BOOTD_NOTITIFICATIONS = {
    common::EVpdNotifications::HmiEventVpsReady,
    common::EVpdNotifications::EndEarlyHmi,
    common::EVpdNotifications::ForcedCompletionEarlyServices,
};

ClientHolder::ClientHolder()
    : mVpdMessageClient{mNotificationReceiver}
{
    configureHandlers();
}

void ClientHolder::configureHandler(const std::vector<common::EVpdNotifications>& notifications,
                                    IVpdNotificationHandler* handler)
{
    for (const auto& notification : notifications) {
        mNotificationReceiver.addHandler(notification, handler);
    }
}

void ClientHolder::configureHandlers()
{
    configureHandler(NATIVE_CLIENT_NOTITIFICATIONS, &mNativeClientNotificationHandler);
    configureHandler(VPS_NOTITIFICATIONS, &mVpsNotificationHandler);
    configureHandler(BOOTD_NOTITIFICATIONS, &mBootdNotificationHandler);
}

void ClientHolder::sendVpdEvent(common::EVpdEvent inEVpdEvent)
{
    mVpdMessageClient.sendVpdEvent(inEVpdEvent);
}

common::EVpdState ClientHolder::getVpdState()
{
    return mNativeClientNotificationHandler.getVpdState();
}

void ClientHolder::setInternalCallback(IInternalCallback* inInternalCallback)
{
    mNotificationReceiver.setInternalCallback(inInternalCallback);
}

void ClientHolder::setNativeCallback(INativeCallback* inNativeCallback)
{
    mNativeClientNotificationHandler.setNativeCallback(inNativeCallback);
    mNotificationReceiver.processReceivedNotifications();
}

void ClientHolder::setVpsCallback(IVpsCallback* inVpsCallback)
{
    mVpsNotificationHandler.setVpsCallback(inVpsCallback);
    mNotificationReceiver.processReceivedNotifications();
}

void ClientHolder::setBootDaemonCallback(IBootDaemonCallback* inBootdCallback)
{
    mBootdNotificationHandler.setBootDaemonCallback(inBootdCallback);
    mNotificationReceiver.processReceivedNotifications();
}

void ClientHolder::startConnection() { mVpdMessageClient.start(); }

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_START
