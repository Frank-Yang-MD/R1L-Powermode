/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTCONTROL_H_

#include "IEventReceiver.h"
#include <ITimeoutControl.h>

#include <atomic>
#include <pthread.h>
#include <thread>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class TimeoutControl : public ITimeoutControl {
public:
    TimeoutControl(IEventReceiver& eventReceiver);

    virtual ~TimeoutControl() override;

    virtual void setupTimeout(std::chrono::microseconds timeout) override;

private:
    static const timespec TIMESPEC_MAX;

    std::atomic<bool> mStarted;
    std::vector<timespec> mTimepoints;

    std::thread mThread;
    pthread_mutex_t mMutex;
    pthread_condattr_t mCondAttr;
    pthread_cond_t mCondVar;

    IEventReceiver& mEventReceiver;

    void mainLoop();

    timespec timepointToTimespec(
        const std::chrono::time_point<std::chrono::steady_clock>& timepoint);
    timespec getMinTimepoint();
    void removeTimepoint(timespec timepont);
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTCONTROL_H_
