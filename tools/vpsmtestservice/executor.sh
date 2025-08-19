#!/bin/bash

declare -a services
declare -a actions

function print_array() {
  arr=("$@")
  length=${#arr[@]}
  for ((i = 0; i < length; i++)); do
    echo "$i. ${arr[$i]}"
  done
}

function print_array_with_back_option() {
  arr=("$@")
  length=${#arr[@]}
  for ((i = 0; i < length; i++)); do
    echo "$i. ${arr[$i]}"
  done
  echo "-1. BACK"
}

function select_action() {
  declare -a actions_=("$@")

  echo "Select number of action:"

  # shellcheck disable=SC2068
  print_array ${actions_[@]}

  read action

  # shellcheck disable=SC2004
  if (($action < 0 || $action >= ${#arr[@]})); then
    # shellcheck disable=SC2151
    select_action "${actions_[@]}"
    echo "Please select correct number of action...."
    # shellcheck disable=SC2152
    return "$"
  else
    return "$action"
  fi
}

function select_service() {
  declare -a services_=("$@")
  echo "Select number of service:"

  # shellcheck disable=SC2068
  print_array_with_back_option ${services_[@]}

  read service

  # shellcheck disable=SC2004
  if ((service < -1 || $service >= ${#arr[@]})); then
    # shellcheck disable=SC2151
    print_array_with_back_option "${services_[@]}"
    echo "Please select correct number of service...."
    # shellcheck disable=SC2152
    return "$"
  else
    return "$service"
  fi
}

function start() {
  select_action "${actions[@]}"
  # shellcheck disable=SC2034
  selected_action=$?
  # shellcheck disable=SC2128
  # shellcheck disable=SC2154
  echo "selected action: ${actions[$selected_action]}"

  selected_service=-1
  if ((selected_action == 0 || selected_action == 1)); then
    # shellcheck disable=SC2151
    select_service "${services[@]}"
    # shellcheck disable=SC2152
    selected_service=$?

    if ((selected_service == -1)); then
      start
      exit 0
    fi
    # shellcheck disable=SC2034
    echo "selected service ${services[selected_service]}"
  fi

  if ((selected_service == -1)); then
    adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION ${actions[$selected_action]}
  else
    adb shell am broadcast -a VPM_TEST_FILTER --es VPM_TEST_ACTION ${actions[$selected_action]} --es VPM_TEST_ACTION_PARAMETER ${services[selected_service]}
  fi
}

actions[0]=isServiceStarted
actions[1]=getServiceState
actions[2]=isAllAppServicesStarted
actions[3]=isAllEfwServicesStarted
actions[4]=normalAppServicesStarted
actions[5]=isNormalEfwServicesStartComplete
actions[6]=isNormalAppServicesStartComplete
actions[7]=isLateEfwServicesStartComplete
actions[8]=isLateAppServicesStartComplete
actions[9]=getVpsState

services[0]=com.mitsubishielectric.ahu.efw.logdogservice.LogdogService
services[1]=com.mitsubishielectric.ahu.efw.audiomanagement.AudioMicService
services[2]=com.mitsubishielectric.ahu.efw.popupservice.PopupService
services[3]=com.mitsubishielectric.ahu.appservice.infotainmentstatemanager.InfotainmentStateService
services[4]=com.mitsubishielectric.ahu.efw.audiomanagement.AudioModeService
services[5]=com.mitsubishielectric.ahu.efw.logdogservice.LogdogLogRequestService
services[6]=com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VehiclePowerService
services[7]=com.mitsubishielectric.ahu.efw.externalinputkeyservice.ExternalInputKeyService
services[8]=com.mitsubishielectric.ahu.efw.viomanagement.VioService
services[9]=com.mitsubishielectric.ahu.efw.extnetservice.ExtNetworkService
services[10]=com.mitsubishielectric.ahu.efw.extendedservicemanager.action.EFW_REBOOT
services[11]=com.mitsubishielectric.ahu.appservice.featuresetservice.FeatureSetService
services[12]=com.mitsubishielectric.ahu.efw.ddcontrol.DdControlService
services[13]=com.mitsubishielectric.ahu.efw.lastscreenservice.lastscreenserverservice.LastScreenServerService
services[14]=com.mitsubishielectric.ahu.efw.logdogservice.LogdogDiagService
services[15]=com.mitsubishielectric.ahu.appservice.tbm.TbmService
services[16]=com.mitsubishielectric.ahu.efw.extendedservicemanager.ExtSrvMngService
services[17]=com.mitsubishielectric.ahu.efw.ecucertservice.ECUCertService
services[18]=com.mitsubishielectric.ahu.appservice.ota.rb.engmode.EngModeServiceManager
services[19]=com.mitsubishielectric.ahu.efw.cpucomservice.CpuComService
services[20]=SmartProjectionService
services[21]=com.mitsubishielectric.ahu.efw.keyinstall.KeyInstallDiagService
services[22]=com.mitsubishielectric.ahu.efw.keyinstall.CertProviderService
services[23]=com.mitsubishielectric.ahu.appservice.devicemanagerservice.DeviceManagerService
services[24]=com.mitsubishielectric.ahu.efw.usbcomplianceservice.UsbComplianceService
services[25]=com.mitsubishielectric.ahu.efw.firewallmgr.FirewallService
services[26]=com.mitsubishielectric.ahu.appservice.featuresetservice
services[27]=com.mitsubishielectric.ahu.efw.keyinstall.KeyMasterWrapperService
services[28]=com.mitsubishielectric.ahu.efw.vehicleconfigservice.VehicleConfigServiceReader
services[29]=com.mitsubishielectric.ahu.efw.common.permission.ACCESS_MELCO_EFW
services[30]=com.mitsubishielectric.ahu.efw.audiomanagement.AudioSignalProcessingServiceImpl
services[31]=com.mitsubishielectric.ahu.efw.vehicleconfigservice.VehicleConfigServiceWriter
services[32]=com.mitsubishielectric.ahu.efw.logdogservice.LogdogFileCollectionService
# shellcheck disable=SC2034
services[33]=com.mitsubishielectric.ahu.efw.cms.ConnectionManagementService

start

exit 0
