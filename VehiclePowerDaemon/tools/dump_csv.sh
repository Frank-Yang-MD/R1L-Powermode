#!/bin/bash

if [ -z "$ANDROID_BUILD_TOP" ];
then
    echo "Please, setup enviroment!";
    exit 1;
fi

FILE=${ANDROID_BUILD_TOP}/out/host/linux-x86/bin/vehiclepowerdaemon_csv_generator
if [ -f "$FILE" ]; then
    $FILE > ${ANDROID_BUILD_TOP}/vendor/melco/efw/Config/LogdogFormat/VPD.csv
    echo "SUCCESS. VPD.csv created."
else 
    echo "$FILE does not exist."
    echo "Provide mm for VehiclePowerDaemon"
fi
