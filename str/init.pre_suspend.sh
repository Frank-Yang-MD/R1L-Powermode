#!/vendor/bin/sh

CONFIG_KILL_TASK_SIGNAL=9;
CONFIG_KILL_TASK_TIMEOUT=10;

# Use the command under /vendor as priority.
export PATH=/vendor/bin:/vendor/xbin:${PATH}

echo -n qcom.suspend > /sys/power/wake_lock
echo "$(basename $0): suspend of init entry $(date '+%Y-%m-%d %H:%M:%S.%N %Z')" > /dev/kmsg;

# Enable logging of interrupts that could have caused resume
echo 1 > /sys/module/msm_show_resume_irq/parameters/debug_mask

PIDS=();

# Disable ACSD touch panel to prevent touch interrupts during suspend
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  for I in /sys/class/input/input*; do
    if [[ -d $I && -f $I/name && -f $I/device/enable ]]; then
      if [[ "$(cat $I/name)" != *"DCSD"* ]]; then
        echo 0 > $I/device/enable
        echo "<6>init.pre_suspend.sh: ACSD touch panel is disabled" > /dev/kmsg
      fi
    fi
  done
'; } &
PIDS+="$! ";

# Disconnect all USB devices because power-supply will be lost during suspend.
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  CONFIG_USB1="a600000.ssusb";
  CONFIG_USB2="a800000.hsusb";
  CONFIG_USB1_DEVICE_PATH="/sys/bus/platform/devices/${CONFIG_USB1}"
  CONFIG_USB2_DEVICE_PATH="/sys/bus/platform/devices/${CONFIG_USB2}"

  # Disable USB bus and lock mode changes
  echo none > ${CONFIG_USB1_DEVICE_PATH}/mode_lock
  echo none > ${CONFIG_USB2_DEVICE_PATH}/mode_lock

  while [[ -n "$(ls -A /sys/bus/usb/devices/)" ]]; do
      sleep 0.05;
  done;

  echo 0 > ${CONFIG_USB1_DEVICE_PATH}/ext_power_switch_enable

  echo "<6>init.pre_suspend.sh: USB bus disabled" > /dev/kmsg
'; } &
PIDS+="$! ";

# Unload LKMs which disturb suspend process.
{ timeout -s ${CONFIG_KILL_TASK_SIGNAL} ${CONFIG_KILL_TASK_TIMEOUT} sh -c '
  rmmod wlan;
  if [ $? -eq 0 ]; then
	  echo "<6>init.pre_suspend.sh: Removing wlan module succeed" > /dev/kmsg;
  else
	  echo "<3>init.pre_suspend.sh: Removing wlan module failed" > /dev/kmsg;
  fi

'; } &
PIDS+="$! ";

# Wait for completion of background tasks
for I in $PIDS; do
	wait $I;
	if [ $? -ne 0 ]; then
		echo "<3>init.pre_suspend.sh: One of the processes return non-zero exit status" > /dev/kmsg;
	fi
done

echo "$(basename $0): suspend of init exit $(date '+%Y-%m-%d %H:%M:%S.%N %Z')" > /dev/kmsg;

echo 1 > /proc/sys/vm/compact_memory

setprop persist.sys.suspend.requested 1
setprop vendor.sys.suspend.request 0

echo -n qcom.suspend > /sys/power/wake_unlock
