/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VConfProvider.h"
#include "MockVehicleConfigReader.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Invoke;
using ::testing::Return;

class VCOnfProvTest : public ::testing::Test {
public:
    VCOnfProvTest()
        : v_conf_prov{mock_vconf_reader}
        , init_string_default{"vendor.vpstart: externalinputkeydaemon"}
        , init_string_simulated{"vpstart: externalinputkeydaemon2"}
    {
    }

    MockVehicleConfigReader mock_vconf_reader;
    VehicleConfigProvider v_conf_prov;

    std::string init_string_default;
    std::string init_string_simulated;
};

TEST_F(VCOnfProvTest, GetInitStringDefault)
{
    EXPECT_CALL(mock_vconf_reader, getString(_, _)).WillOnce(Return(1));
    EXPECT_STREQ(v_conf_prov.getInitString().c_str(), init_string_default.c_str());
}

TEST_F(VCOnfProvTest, GetInitStringHappy)
{
    EXPECT_CALL(mock_vconf_reader, getString(_, _))
        .WillOnce(DoAll(Invoke([=](const std::string& key, std::string& value) {
                            (void)key;
                            value = init_string_simulated;
                        }),
                        Return(0)));
    EXPECT_STREQ(v_conf_prov.getInitString().c_str(), init_string_simulated.c_str());
}

TEST_F(VCOnfProvTest, getConfig_returns_TimeoutConfig_with_default_settings)
{
    const TimeoutConfig& timeoutConfig = v_conf_prov.getConfig();

    EXPECT_EQ(timeoutConfig.VPowerAppStopJavaTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerAppStopLogdogTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerAppStopNativeTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerAppResumeEfwTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerLogSaveTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerAppResumeEfwTimeout, 0);
    EXPECT_EQ(timeoutConfig.VPowerAppResumeAppTimeout, 0);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
