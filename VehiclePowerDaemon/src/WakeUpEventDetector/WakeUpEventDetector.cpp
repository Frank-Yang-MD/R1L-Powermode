/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUpEventDetector.h"
#include "IEventReceiver.h"
#include "VehiclePowerDaemonLog.h"

#include <IExecutor.h>
#include <IODevice.h>

/* allocate 1 byte for value to be read */
#define BUFFER_SIZE 1

using com::mitsubishielectric::ahu::common::IExecutor;
using com::mitsubishielectric::ahu::common::IODevice;

static const char WAKE_UP_EVENT_EXPECTED_VALUE = '0';
static constexpr int OPEN_RETRY_COUNT = 10;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

WakeUpEventDetector::WakeUpEventDetector(IExecutor& executor,
                                         IODevice& ioDevice,
                                         IEventReceiver& eventReceiver)
    : mExecutor{executor}
    , mIODevice{ioDevice}
    , mEventReceiver{eventReceiver}
{
}

WakeUpEventDetector::~WakeUpEventDetector() { stop(); }

void WakeUpEventDetector::start()
{
    if (openDevice()) {
        mStarted = true;
        mExecutor.execute(std::bind(&WakeUpEventDetector::waitForWakeUp, this));
    }
}

void WakeUpEventDetector::stop() { mStarted = false; }

bool WakeUpEventDetector::openDevice()
{
    bool result = false;
    if (mIODevice.open(IODevice::OpenMode::ReadOnly)) {
        result = true;
        /*
         * According to kernel/fs/kernfs/file.c:822, blocking poll or select call
         * on sysfs node can be done only after read operation. Therefore, we read
         * the kernfs attribute file first (there is no need to check what we have
         * read) and only after that we can use poll for the fd.
         */
        uint8_t buffer[BUFFER_SIZE] = {0};
        mIODevice.read(buffer, IODevice::kTimeoutImmediate);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::WakeUpEventDetector_cannotOpenNode);
    }

    return result;
}

void WakeUpEventDetector::waitForWakeUp()
{
    uint8_t buffer[BUFFER_SIZE] = {0};
    int retryCount = 0;

    while (mStarted) {
        mIODevice.seek(0, IODevice::Whence::Beginning);
        const auto result =
            mIODevice.read(buffer, IODevice::kTimeoutInfinite, IODevice::Events::PollPri);

        if (IODevice::Result::Success == result) {
            const char value = buffer[0];

            MLOGI(LogFuncId::VPD, LogId::WakeUpEventDetector_valueFromNode, std::string{value});

            if (WAKE_UP_EVENT_EXPECTED_VALUE == value) {
                MLOGI(LogFuncId::VPD, LogId::WakeUpEventDetector_wakeUpEventDetected);
                mEventReceiver.onEventReceive(0, common::EVpdEvent::WakeUp);
            }
        }
        else {
            MLOGW(LogFuncId::VPD, LogId::WakeUpEventDetector_pollError, retryCount);

            if (retryCount++ < OPEN_RETRY_COUNT) {
                mIODevice.close();
                openDevice();
            }
            else {
                MLOGW(LogFuncId::VPD, LogId::WakeUpEventDetector_pollError_retryExceeded);
                mStarted = false;
                break;
            }
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
