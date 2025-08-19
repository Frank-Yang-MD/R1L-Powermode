#!/vendor/bin/sh

CONFIG_KILL_TASK_SIGNAL=9;
CONFIG_KILL_TASK_TIMEOUT=10;

# Use the command under /vendor as priority.
export PATH=/vendor/bin:/vendor/xbin:${PATH}

# Check if init.pre_suspend.sh has been run before run init.post_wakeup.sh
# to guarantee persist.sys.suspend.* properties contain correct values.
# This check is also important due to init.post_wakeup.sh is run on boot
# complete as failover for the case when system crash after
# init.pre_suspend.sh call or before init.post_wakeup.sh call.

sys_suspend_requested=$(getprop persist.sys.suspend.requested)

if [ "x$sys_suspend_requested" != "x1" ]; then
    # We need to set value of "sys.wakeup.request" property to "0" even
    # if execution was not successfull because this property is also
    # used by VehiclePowerDaemon as indicator that init.pre_suspend.sh
    # script is not currently running.
    setprop vendor.sys.wakeup.request 0
    echo "<3>init.post_wakeup.sh: Suspend hasn't been requested, no reason to execute this script, exiting" > /dev/kmsg;
    exit 1
fi

echo -n $0 > /sys/power/wake_lock
echo "$(basename $0): resume of init entry $(date '+%Y-%m-%d %H:%M:%S.%N %Z')" > /dev/kmsg;

PIDS=();

# Recovery USB devices.
# Configuration is executed later because hub device is detected slowly.
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  CONFIG_USB1="a600000.ssusb"
  CONFIG_USB2="a800000.hsusb"

  CONFIG_USB1_DEVICE_PATH="/sys/bus/platform/devices/${CONFIG_USB1}"
  CONFIG_USB2_DEVICE_PATH="/sys/bus/platform/devices/${CONFIG_USB2}"

  echo 1 > ${CONFIG_USB1_DEVICE_PATH}/ext_power_switch_enable

  # Unlock mode changes
  echo "" > ${CONFIG_USB1_DEVICE_PATH}/mode_lock
  echo "" > ${CONFIG_USB2_DEVICE_PATH}/mode_lock

  echo host > ${CONFIG_USB1_DEVICE_PATH}/mode
  echo host > ${CONFIG_USB2_DEVICE_PATH}/mode

  echo "<6>init.post_wakeup.sh: USB bus restored" > /dev/kmsg

  prop_sys_usb_port_adb_roleswitch="$(getprop vendor.usb.port.adb.roleswitch)"
  if [[ "auto" == ${prop_sys_usb_port_adb_roleswitch} ]]; then

    prop_sys_usb_controller=$(getprop vendor.usb.controller);
    while [[ ! -e "/sys/class/udc/${prop_sys_usb_controller}" ]]; do
      sleep 0.05;
    done;

    prop_init_svc_adbd=$(getprop vendor.init.svc.adbd);
    if [[ "running" == ${prop_init_svc_adbd} ]]; then
        echo peripheral > $CONFIG_USB1_DEVICE_PATH/mode;
    fi

  fi

'; } &
PIDS+="$! ";

# Load LKMs which is unloaded at suspend
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  insmod /vendor/lib/modules/qca_cld3_qca6174.ko;
  if [ $? -eq 0 ]; then
	echo "<6>init.post_wakeup.sh: Inserting wlan module succeed" > /dev/kmsg;
  else
        echo "<3>init.post_wakeup.sh: Inserting wlan module failed" > /dev/kmsg;
  fi
'; } &
PIDS+="$! ";

# Enable ACSD touch panel has been disabled during suspend
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  for I in /sys/class/input/input*; do
    if [[ -d $I && -f $I/name && -f $I/device/enable ]]; then
      if [[ "$(cat $I/name)" != *"DCSD"* ]]; then
        echo 1 > $I/device/enable
        echo "<6>init.post_wakeup.sh: ACSD touch panel is enabled" > /dev/kmsg
      fi
    fi
  done
'; } &
PIDS+="$! ";

# Wait for completion of background tasks
for I in $PIDS; do
	wait $I;
	if [ $? -ne 0 ]; then
		echo "<3>init.post_wakeup.sh: One of the processes return non-zero exit status" > /dev/kmsg;
	fi
done

echo "$(basename $0): resume of init exit $(date '+%Y-%m-%d %H:%M:%S.%N %Z')" > /dev/kmsg;

setprop vendor.sys.wakeup.request 0
setprop persist.sys.suspend.requested 0

echo -n $0 > /sys/power/wake_unlock
