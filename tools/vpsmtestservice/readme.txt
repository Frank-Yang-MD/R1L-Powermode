How to use VpmsTestService:

1) Use the following patch to include VpmsTestService in a build:
https://mea-dev.net/gerrit/c/vendor/melco/efw/vehiclepwrmgr/+/189675

2) Add VpmsTestService to startup sequence.
Use corresponding patch:
https://mea-dev.net/gerrit/c/vendor/melco/efw/config/vehicleconfig/+/190214

Or follow the steps below:
2.1 Copy "NORMAL_EFW_SERVICES_START_CONFIG.str" file from HU using following command:
adb pull /mnt/vendor/vehicledefcfg/VPM/NORMAL_EFW_SERVICES_START_CONFIG.str <target_directory>
where <target_directory> is the path to the directory on your PC

2.2 Update "NORMAL_EFW_SERVICES_START_CONFIG.str" file by adding VpmsTestService to the list of
services and as the first element of "start_sequence" block:
{
...

  "VpmsTestService": {
    "pkg": "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpmtestservice",
    "cls": "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpmtestservice.VpmsTestService"
  },
  "start_sequence": [
    {
      "command": "start",
      "service": "VpmsTestService"
    },

...
}

2.3 Push the updated "NORMAL_EFW_SERVICES_START_CONFIG.str" file to the HU using following commands:
adb root
adb shell mount -o rw,remount /mnt/vendor/vehicledefcfg
adb push <target_directory>/NORMAL_EFW_SERVICES_START_CONFIG.str /mnt/vendor/vehicledefcfg/VPM
adb reboot

3) To call VehiclePowerManager API, send broadcast from command line:
adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION <method_to_be_called> \
[--es VPM_TEST_ACTION_PARAMETER <method_argument>]

Examples:
adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION getServiceState \
--es VPM_TEST_ACTION_PARAMETER com.mitsubishielectric.ahu.efw.logdogservice.LogdogService
adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION isAllEfwServicesStarted

The list of VehiclePowerManager API methods, which can be tested with VpmsTestService is as follows:
-isServiceStarted(String className)
-EServiceState getServiceState(String className)
-isAllAppServicesStarted()
-isAllEfwServicesStarted()
-normalAppServicesStarted()
-isNormalEfwServicesStartComplete()
-isNormalAppServicesStartComplete()
-isLateEfwServicesStartComplete()
-isLateAppServicesStartComplete()

where className is a string value, which should be passed in place of <method_argument>

4) To get the last power state transition event, send broadcast from command line:
adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION getVpsState

It will call both VehiclePowerServiceManager#getNotifiedEvent() and VehiclePowerDaemon#GetVPDState()

5) As an alternative, executor.sh script can be used, which provides convenient way of testing
the above methods

6) VpmsTestService implements polling of VehiclePowerServiceManager until all services startup
completion and logs the result returned by the following VehiclePowerServiceManager API methods:
- isNormalEfwServicesStartComplete();
- isNormalAppServicesStartComplete();
- isLateEfwServicesStartComplete();
- isLateAppServicesStartComplete();
- normalAppServicesStarted();
- isAllEfwServicesStarted();
- isAllAppServicesStarted();
