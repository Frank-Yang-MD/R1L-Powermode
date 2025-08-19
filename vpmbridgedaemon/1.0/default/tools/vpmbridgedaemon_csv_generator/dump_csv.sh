#!/bin/bash

if [ -z "$ANDROID_BUILD_TOP" ];
then
    echo "Please, setup enviroment!";
    exit 1;
fi

FILE=${ANDROID_HOST_OUT}/bin/vpmbridgedaemon_csv_generator
if [ -f "$FILE" ]; then
    $FILE > ${ANDROID_BUILD_TOP}/vendor/melco/efw/Config/LogdogFormat/VPMBD.csv
    echo "SUCCESS. VPMDB.csv created."
else
    echo "$FILE does not exist."
    echo "Provide mm for vpmbridgedaemon_csv_generator"
fi
