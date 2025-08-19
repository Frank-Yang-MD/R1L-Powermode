/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "CpuComClient.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

static const CpuCommand ADTR_REQUEST{0x71, 0x04};
static const CpuCommand START_COMPLETE_NOTIFICATION{0x71, 0x81};
static const CpuCommand SHUTDOWN_CANCEL_ACCEPT_RESPONSE{0x71, 0x85};
static const CpuCommand SHUTDOWN_START_NOTIFICATION{0x71, 0x83};
static const CpuCommand SHUTDOWN_CANCEL_COMPLETE{0x71, 0x0D};
static const CpuCommand WDRT_REQUEST{0x71, 0x06};
const CpuCommand MAIN_STATUS_CHANGE_NOTIFICATION{0x85, 0x8F};
static const CpuCommand MAIN_START_COMPLETE{0x71, 0x87};

CpuComClient::CpuComClient(IEventReceiver& eventReceiver,
                           std::function<std::unique_ptr<cpucom::v2::ICpuCom>()> cpuComCreator)
    : mCpuComCreator(cpuComCreator)
    , mCommandParser(eventReceiver)
    , mShutdownProcessingStartStatusReceiver{eventReceiver}
    , mResumeProcessingStartCompleteStatusReceiver{eventReceiver}
{
}

bool CpuComClient::start()
{
    using namespace std::placeholders;

    MLOGI(LogFuncId::VPD, LogId::CpuComClient_start);

    if (!mCpuCom) {
        if (mCpuComCreator) {
            mCpuCom = mCpuComCreator();
        }
        if (!mCpuCom) {
            MLOGW(LogFuncId::VPD, LogId::CpuComClient_start_CpuCom_is_not_initialized);
            return false;
        }
    }

    mCpuCom->initialize(std::bind(&CommandParser::onError, &mCommandParser, _1, _2), [] {});

    mCpuCom->subscribe(CommandParser::SHUTDOWN_REQUEST,
                       std::bind(&CommandParser::onReceiveCommand, &mCommandParser, _1, _2));
    mCpuCom->subscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST,
                       std::bind(&CommandParser::onReceiveCommand, &mCommandParser, _1, _2));

    return mCpuCom->connect();
}

void CpuComClient::stop()
{
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_stop);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_stop_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->unsubscribe(CommandParser::SHUTDOWN_REQUEST);
    mCpuCom->unsubscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST);

    mCpuCom->disconnect();
}

void CpuComClient::sendAdtrRequest()
{
    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendAdtrRequest_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(ADTR_REQUEST, {});
}

void CpuComClient::sendStartComplete()
{
    using namespace std::placeholders;
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_sendStartComplete);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendStartComplete_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(START_COMPLETE_NOTIFICATION, {},
                  std::bind(&ResumeProcessingStartCompleteStatusReceiver::onDeliveryStatus,
                            &mResumeProcessingStartCompleteStatusReceiver, _1));
}

void CpuComClient::sendShutdownCancelAccept()
{
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_sendShutdownCancelAccept);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD,
              LogId::CpuComClient_sendShutdownCancelAccept_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(SHUTDOWN_CANCEL_ACCEPT_RESPONSE, {});
}

void CpuComClient::sendShutdownProcessingStart()
{
    using namespace std::placeholders;
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_sendShutdownProcessingStart);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD,
              LogId::CpuComClient_sendShutdownProcessingStart_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(SHUTDOWN_START_NOTIFICATION, {},
                  std::bind(&ShutdownProcessingStartStatusReceiver::onDeliveryStatus,
                            &mShutdownProcessingStartStatusReceiver, _1));
}

void CpuComClient::sendShutdownCancelComplete()
{
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_sendShutdownCancelComplete);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendShutdownCancelComplete_is_not_initialized);
        return;
    }

    mCpuCom->send(SHUTDOWN_CANCEL_COMPLETE, {});
}

void CpuComClient::sendWdtrRequest()
{
    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendWdtrRequest_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(WDRT_REQUEST, {});
}

void CpuComClient::sendSelfRebootTrue()
{
    MLOGE(LogFuncId::VPD, LogId::CpuComClient_sendSelfRebootTrue);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendSelfRebootTrue_CpuCom_is_not_initialized);
        return;
    }

    const uint8_t dataTypeSelfReboot = 10;
    const uint8_t dataValueHigh = 0;
    const uint8_t dataValueLow = 1;

    mCpuCom->send(MAIN_STATUS_CHANGE_NOTIFICATION,
                  {dataTypeSelfReboot, dataValueHigh, dataValueLow});
}

void CpuComClient::sendMainStartComplete()
{
    MLOGI(LogFuncId::VPD, LogId::CpuComClient_sendMainStartComplete);

    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_sendMainStartComplete_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->send(MAIN_START_COMPLETE, {});
}

CpuComClient::~CpuComClient()
{
    if (!mCpuCom) {
        MLOGW(LogFuncId::VPD, LogId::CpuComClient_dtor_CpuCom_is_not_initialized);
        return;
    }

    mCpuCom->unsubscribe(CommandParser::SHUTDOWN_REQUEST);
    mCpuCom->unsubscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST);
}  // LCOV_EXCL_LINE

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
