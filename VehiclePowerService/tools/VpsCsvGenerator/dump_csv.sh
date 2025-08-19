#!/bin/sh

LOG_FILES="../../src/java/com/mitsubishielectric/ahu/efw/vehiclepwrmgr/VPSLogMsg.java"
MAIN_CLASS_NAME="com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VpsCsvGenerator"

SRC_FILES="$(find "src/" -name "*.java") ${LOG_FILES}"

OUT_DIR="build"

CLASSPATH="${ANDROID_BUILD_TOP}/out/target/common/obj/APPS/VehiclePowerService_intermediates/classes.jar:${ANDROID_BUILD_TOP}/out/target/common/obj/JAVA_LIBRARIES/com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr_intermediates/classes.jar:${ANDROID_BUILD_TOP}/out/target/common/obj/JAVA_LIBRARIES/com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib_intermediates/classes.jar"

OUT_JAR_NAME="${OUT_DIR}/vps_csv_generator.jar"

mkdir -p ${OUT_DIR}

javac -d ${OUT_DIR} -cp ${CLASSPATH} ${SRC_FILES}

jar cf ${OUT_JAR_NAME} -C ${OUT_DIR} .

java -Djava.library.path=/${ANDROID_HOST_OUT}/lib64 -cp ${CLASSPATH}:${OUT_JAR_NAME} ${MAIN_CLASS_NAME}

${ANDROID_BUILD_TOP}/out/host/linux-x86/bin/vps_jni_csv_generator > VPS.csv

echo '' >> VPS.csv

cat VPS_java.csv >> VPS.csv

rm -f VPS_java.csv

mv *.csv ${ANDROID_BUILD_TOP}/vendor/melco/efw/Config/LogdogFormat/

rm -rf ${OUT_DIR}


