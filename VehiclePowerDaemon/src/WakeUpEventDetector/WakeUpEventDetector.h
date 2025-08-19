/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WKUPKE_DT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WKUPKE_DT_H_

#include "IWakeUpEventDetector.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace common {
class IExecutor;
class IODevice;
}  // namespace common
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiver;

/**
 * Implements waiting for wake up event.
 */
class WakeUpEventDetector final : public IWakeUpEventDetector {  // LCOV_EXCL_LINE
    // exclude all possible destructors, that cannot be covered by unit-test
public:
    WakeUpEventDetector(com::mitsubishielectric::ahu::common::IExecutor&,
                        com::mitsubishielectric::ahu::common::IODevice&,
                        IEventReceiver&);

    ~WakeUpEventDetector();

    /**
     * Open the sysfs node to wait for wake up event and clear its buffer.
     * Then start polling for wakeup event in a separate thread. Once the
     * event is detected, it is notified to EventReceiver.
     */
    virtual void start() override;

    /**
     * The following method does not actually interrupts poll(), it is
     * used only to break polling loop in unit tests or integration tests.
     */
    virtual void stop() override;

private:
    /**
     * Open device that notifies about wake up event. Returns true is open
     * is successful, false otherwise.
     */
    bool openDevice();

    /**
     * Poll the fd of the opened sysfs node. Once the wake up event received,
     * WakeUp event will be put in event receiver.
     */
    void waitForWakeUp();

    com::mitsubishielectric::ahu::common::IExecutor& mExecutor;
    com::mitsubishielectric::ahu::common::IODevice& mIODevice;
    IEventReceiver& mEventReceiver;

    bool mStarted = false;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WKUPKE_DT_H_
