/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <KernelModulesControl.h>

#include <MockAndroidInit.h>

using namespace testing;

const std::string MODULES_LOAD_PROP_NAME = "vendor.sys.wakeup.request";
const std::string MODULES_LOADED_PROP_VALUE = "0";

const std::string MODULES_UNLOAD_PROP_NAME = "vendor.sys.suspend.request";
const std::string MODULES_UNLOADED_PROP_VALUE = "0";

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class KernelModulesControlTest : public Test {
public:
    KernelModulesControlTest()
        : kernelModulesControl{mMockAndroidInit}
    {
    }

    KernelModulesControl kernelModulesControl;

    MockAndroidInit mMockAndroidInit;
};

TEST_F(KernelModulesControlTest, loadModules)
{
    EXPECT_CALL(mMockAndroidInit, propertySet(MODULES_LOAD_PROP_NAME, "0")).Times(1);
    EXPECT_CALL(mMockAndroidInit, propertySet(MODULES_LOAD_PROP_NAME, "1")).Times(1);

    kernelModulesControl.loadModules();
}

TEST_F(KernelModulesControlTest, unloadModules)
{
    EXPECT_CALL(mMockAndroidInit, propertySet(MODULES_UNLOAD_PROP_NAME, "0")).Times(1);
    EXPECT_CALL(mMockAndroidInit, propertySet(MODULES_UNLOAD_PROP_NAME, "1")).Times(1);

    kernelModulesControl.unloadModules();
}

TEST_F(KernelModulesControlTest, isModulesLoadedPositive)
{
    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_LOAD_PROP_NAME, _))
        .WillOnce(DoAll(SetArgReferee<1>(MODULES_LOADED_PROP_VALUE),
                        Return(MODULES_LOADED_PROP_VALUE.length())));

    EXPECT_TRUE(kernelModulesControl.isModulesLoaded());
}

TEST_F(KernelModulesControlTest, isModulesLoadedNegative)
{
    const std::string modulesStillUnloaded = "1";

    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_LOAD_PROP_NAME, _))
        .WillOnce(
            DoAll(SetArgReferee<1>(modulesStillUnloaded), Return(modulesStillUnloaded.length())));

    EXPECT_FALSE(kernelModulesControl.isModulesLoaded());
}

TEST_F(KernelModulesControlTest, isModulesUnloadedPositive)
{
    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_UNLOAD_PROP_NAME, _))
        .WillOnce(DoAll(SetArgReferee<1>(MODULES_UNLOADED_PROP_VALUE),
                        Return(MODULES_UNLOADED_PROP_VALUE.length())));

    EXPECT_TRUE(kernelModulesControl.isModulesUnloaded());
}

TEST_F(KernelModulesControlTest, isModulesUnloadedNegative)
{
    const std::string modulesStillLoaded = "1";

    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_UNLOAD_PROP_NAME, _))
        .WillOnce(DoAll(SetArgReferee<1>(modulesStillLoaded), Return(modulesStillLoaded.length())));

    EXPECT_FALSE(kernelModulesControl.isModulesUnloaded());
}

TEST_F(KernelModulesControlTest, skipModulesUnloadedCheckPositive)
{
    const std::string modulesStillLoaded = "";

    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_UNLOAD_PROP_NAME, _))
        .WillOnce(DoAll(SetArgReferee<1>(modulesStillLoaded), Return(modulesStillLoaded.length())));

    EXPECT_TRUE(kernelModulesControl.skipModulesUnloadedCheck());
}

TEST_F(KernelModulesControlTest, skipModulesUnloadedCheckNegative)
{
    EXPECT_CALL(mMockAndroidInit, propertyGet(MODULES_UNLOAD_PROP_NAME, _))
        .WillOnce(DoAll(SetArgReferee<1>(MODULES_UNLOADED_PROP_VALUE),
                        Return(MODULES_UNLOADED_PROP_VALUE.length())));

    EXPECT_FALSE(kernelModulesControl.skipModulesUnloadedCheck());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
