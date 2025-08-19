/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORSTARTCPUCOM_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORSTARTCPUCOM_H_

#include <memory>
#include <string>

#include "IAndroidInit.h"
#include "ICpuComControl.h"
#include "VpdStateBase.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 *  Wait for CpuComDaemon 'Start complete' notification
 */
class WaitForStartCpuCom : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             IAndroidInit& pAndroidInit,
                             ICpuCommControl& cpuComControl);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    void onEnter() override;
    void onStartCompleteCpuCom() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitForStartCpuCom");
    }

private:
    WaitForStartCpuCom(IVpdStateMachine* sm,
                       IAndroidInit& pAndroidInit,
                       ICpuCommControl& cpuComControl);
    virtual ~WaitForStartCpuCom() override;

    IAndroidInit& mAndroidInit;
    ICpuCommControl& mCpuComControl;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORSTARTCPUCOM_H_
