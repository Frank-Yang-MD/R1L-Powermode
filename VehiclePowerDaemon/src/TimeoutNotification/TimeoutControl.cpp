/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "TimeoutControl.h"
#include "EVpdEvent.h"

#include <algorithm>
#include <limits>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace std::chrono;

const timespec TimeoutControl::TIMESPEC_MAX = {
    .tv_sec = std::numeric_limits<time_t>::max(),
    .tv_nsec = 999999999L,  // valid values are [0, 999999999]
};

TimeoutControl::TimeoutControl(IEventReceiver& eventReceiver)
    : mStarted{true}
    , mEventReceiver{eventReceiver}
{
    mMutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_condattr_init(&mCondAttr);
    pthread_condattr_setclock(&mCondAttr, CLOCK_MONOTONIC);
    pthread_cond_init(&mCondVar, &mCondAttr);
    mThread = std::thread{&TimeoutControl::mainLoop, this};
}

TimeoutControl::~TimeoutControl()
{
    pthread_mutex_lock(&mMutex);
    mStarted = false;

    pthread_mutex_unlock(&mMutex);
    pthread_cond_broadcast(&mCondVar);
    // LCOV_EXCL_START
    // This is excluded from a unit test coverage report because it is hard to test due to
    // multithreading.
    if (mThread.joinable()) {
        mThread.join();
    }
    // LCOV_EXCL_STOP
}

timespec TimeoutControl::timepointToTimespec(const time_point<steady_clock>& timepoint)
{
    timespec ts;
    ts.tv_sec = duration_cast<seconds>(timepoint.time_since_epoch()).count();
    ts.tv_nsec =
        duration_cast<nanoseconds>(timepoint.time_since_epoch()).count() - ts.tv_sec * 1000000000L;
    return ts;
}

void TimeoutControl::setupTimeout(microseconds timeout)
{
    auto ts = timepointToTimespec(steady_clock::now() + timeout);

    pthread_mutex_lock(&mMutex);
    mTimepoints.emplace_back(ts);
    pthread_mutex_unlock(&mMutex);

    pthread_cond_broadcast(&mCondVar);
}

timespec TimeoutControl::getMinTimepoint()
{
    return *std::min_element(begin(mTimepoints), end(mTimepoints),
                             [](const timespec& lhs, const timespec& rhs) {
                                 if (lhs.tv_sec == rhs.tv_sec) {
                                     return lhs.tv_nsec < rhs.tv_nsec;
                                 }
                                 return lhs.tv_sec < rhs.tv_sec;
                             });
}

void TimeoutControl::removeTimepoint(timespec timepoint)
{
    mTimepoints.erase(std::remove_if(begin(mTimepoints), end(mTimepoints),
                                     [&timepoint](const timespec& t) {
                                         if (t.tv_sec == timepoint.tv_sec) {
                                             return t.tv_nsec <= timepoint.tv_nsec;
                                         }
                                         return t.tv_sec <= timepoint.tv_sec;
                                     }),
                      end(mTimepoints));
}

void TimeoutControl::mainLoop()
{
    timespec timepoint;
    while (mStarted) {
        timepoint = TIMESPEC_MAX;

        pthread_mutex_lock(&mMutex);

        if (!mTimepoints.empty()) {
            timepoint = getMinTimepoint();
        }

        int rc = pthread_cond_timedwait(&mCondVar, &mMutex, &timepoint);

        if (rc == ETIMEDOUT) {
            removeTimepoint(timepoint);
            pthread_mutex_unlock(&mMutex);

            mEventReceiver.onEventReceive(0, common::EVpdEvent::Timeout);
        }
        else {
            pthread_mutex_unlock(&mMutex);
        }
    }

    pthread_mutex_lock(&mMutex);
    mTimepoints.clear();
    pthread_mutex_unlock(&mMutex);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
