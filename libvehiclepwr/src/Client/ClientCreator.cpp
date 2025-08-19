/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because it has no logic.

#include "ClientCreator.h"
#include "BootDaemonClient.h"
#include "ClientHolder.h"
#include "CpuComClient.h"
#include "LibVehiclePwrLog.h"
#include "LogdogClient.h"
#include "NativeClient.h"
#include "VpsClient.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

ICpuComClient* ClientCreator::CreateCpuComClient()
{
    std::unique_ptr<CpuComClient> client;
    client = std::make_unique<CpuComClient>(std::make_unique<ClientHolder>());
    client->startConnection();
    return client.release();
}

INativeClient* ClientCreator::CreateNativeClient()
{
    std::unique_ptr<NativeClient> client;
    client = std::make_unique<NativeClient>(std::make_unique<ClientHolder>());
    client->startConnection();
    return client.release();
}

IVpsClient* ClientCreator::CreateVpsClient()
{
    std::unique_ptr<VpsClient> client;
    client = std::make_unique<VpsClient>(std::make_unique<ClientHolder>());
    client->startConnection();
    return client.release();
}

INativeClient* ClientCreator::CreateLogdogClient()
{
    std::unique_ptr<LogdogClient> client;
    client = std::make_unique<LogdogClient>(std::make_unique<ClientHolder>());
    client->startConnection();
    return client.release();
}
IBootDaemonClient* ClientCreator::CreateBootDaemonClient()
{
    std::unique_ptr<BootDaemonClient> client;
    client = std::make_unique<BootDaemonClient>(std::make_unique<ClientHolder>());
    client->startConnection();
    return client.release();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP
