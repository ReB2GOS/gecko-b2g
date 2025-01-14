/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#define LOG_TAG "WifiNative"

#include "WifiNative.h"
#include <cutils/properties.h>
#include <string.h>
#include "js/CharacterEncoding.h"

using namespace mozilla::dom;

static const int32_t CONNECTION_RETRY_INTERVAL_US = 100000;
static const int32_t CONNECTION_RETRY_TIMES = 50;

/* static */
WifiHal* WifiNative::sWifiHal = nullptr;
WificondControl* WifiNative::sWificondControl = nullptr;
SoftapManager* WifiNative::sSoftapManager = nullptr;
SupplicantStaManager* WifiNative::sSupplicantStaManager = nullptr;
WifiEventCallback* WifiNative::sCallback = nullptr;

WifiNative::WifiNative()
    : mScanEventService(nullptr),
      mPnoScanEventService(nullptr),
      mSoftapEventService(nullptr) {
  sWifiHal = WifiHal::Get();
  sWificondControl = WificondControl::Get();
  sSoftapManager = SoftapManager::Get();
  sSupplicantStaManager = SupplicantStaManager::Get();
}

bool WifiNative::ExecuteCommand(CommandOptions& aOptions, nsWifiResult* aResult,
                                const nsCString& aInterface) {
  // Always correlate the opaque ids.
  aResult->mId = aOptions.mId;

  if (aOptions.mCmd == nsIWifiCommand::INITIALIZE) {
    aResult->mStatus = InitHal();
  } else if (aOptions.mCmd == nsIWifiCommand::GET_MODULE_VERSION) {
    aResult->mStatus =
        GetDriverModuleInfo(aResult->mDriverVersion, aResult->mFirmwareVersion);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_SUPPORTED_FEATURES) {
    aResult->mStatus = GetSupportedFeatures(aResult->mSupportedFeatures);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_LOW_LATENCY_MODE) {
    aResult->mStatus = SetLowLatencyMode(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_CONCURRENCY_PRIORITY) {
    aResult->mStatus = SetConcurrencyPriority(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::START_WIFI) {
    aResult->mStatus = StartWifi(aResult->mStaInterface);
  } else if (aOptions.mCmd == nsIWifiCommand::STOP_WIFI) {
    aResult->mStatus = StopWifi();
  } else if (aOptions.mCmd == nsIWifiCommand::GET_MAC_ADDRESS) {
    aResult->mStatus = GetMacAddress(aResult->mMacAddress);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_STA_IFACE) {
    aResult->mStatus = GetClientInterfaceName(aResult->mStaInterface);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_DEBUG_LEVEL) {
    aResult->mStatus = GetDebugLevel(aResult->mDebugLevel);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_DEBUG_LEVEL) {
    aResult->mStatus = SetDebugLevel(&aOptions.mDebugLevel);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_POWER_SAVE) {
    aResult->mStatus = SetPowerSave(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_SUSPEND_MODE) {
    aResult->mStatus = SetSuspendMode(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_EXTERNAL_SIM) {
    aResult->mStatus = SetExternalSim(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_AUTO_RECONNECT) {
    aResult->mStatus = SetAutoReconnect(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_COUNTRY_CODE) {
    aResult->mStatus = SetCountryCode(aOptions.mCountryCode);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_BT_COEXIST_MODE) {
    aResult->mStatus = SetBtCoexistenceMode(aOptions.mBtCoexistenceMode);
  } else if (aOptions.mCmd == nsIWifiCommand::SET_BT_COEXIST_SCAN_MODE) {
    aResult->mStatus = SetBtCoexistenceScanMode(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_LINK_LAYER_STATS) {
    wifiNameSpaceV1_0::StaLinkLayerStats stats;
    aResult->mStatus = GetLinkLayerStats(stats);

    RefPtr<nsLinkLayerStats> linkLayerStats = new nsLinkLayerStats(
        stats.iface.beaconRx, stats.iface.avgRssiMgmt, stats.timeStampInMs);

    RefPtr<nsLinkLayerPacketStats> wmeBePktStats = new nsLinkLayerPacketStats(
        stats.iface.wmeBePktStats.rxMpdu, stats.iface.wmeBePktStats.txMpdu,
        stats.iface.wmeBePktStats.lostMpdu, stats.iface.wmeBePktStats.retries);

    RefPtr<nsLinkLayerPacketStats> wmeBkPktStats = new nsLinkLayerPacketStats(
        stats.iface.wmeBkPktStats.rxMpdu, stats.iface.wmeBkPktStats.txMpdu,
        stats.iface.wmeBkPktStats.lostMpdu, stats.iface.wmeBkPktStats.retries);

    RefPtr<nsLinkLayerPacketStats> wmeViPktStats = new nsLinkLayerPacketStats(
        stats.iface.wmeViPktStats.rxMpdu, stats.iface.wmeViPktStats.txMpdu,
        stats.iface.wmeViPktStats.lostMpdu, stats.iface.wmeViPktStats.retries);

    RefPtr<nsLinkLayerPacketStats> wmeVoPktStats = new nsLinkLayerPacketStats(
        stats.iface.wmeVoPktStats.rxMpdu, stats.iface.wmeVoPktStats.txMpdu,
        stats.iface.wmeVoPktStats.lostMpdu, stats.iface.wmeVoPktStats.retries);

    size_t numRadio = stats.radios.size();
    nsTArray<RefPtr<nsLinkLayerRadioStats>> radios(numRadio);

    for (auto& radio : stats.radios) {
      size_t numTxTime = radio.txTimeInMsPerLevel.size();
      nsTArray<uint32_t> txTimeInMsPerLevel(numTxTime);

      for (auto& txTime : radio.txTimeInMsPerLevel) {
        txTimeInMsPerLevel.AppendElement(txTime);
      }
      RefPtr<nsLinkLayerRadioStats> radioStats = new nsLinkLayerRadioStats(
          radio.onTimeInMs, radio.txTimeInMs, radio.rxTimeInMs,
          radio.onTimeInMsForScan, txTimeInMsPerLevel);
      radios.AppendElement(radioStats);
    }

    linkLayerStats->updatePacketStats(wmeBePktStats, wmeBkPktStats,
                                      wmeViPktStats, wmeVoPktStats);
    linkLayerStats->updateRadioStats(radios);
    aResult->updateLinkLayerStats(linkLayerStats);
  } else if (aOptions.mCmd == nsIWifiCommand::SIGNAL_POLL) {
    std::vector<int32_t> pollResult;
    aResult->mStatus = SignalPoll(pollResult);

    size_t num = pollResult.size();
    if (num > 0) {
      nsTArray<int32_t> pollArray(num);
      for (int32_t& element : pollResult) {
        pollArray.AppendElement(element);
      }
      aResult->updateSignalPoll(pollArray);
    }
  } else if (aOptions.mCmd == nsIWifiCommand::SET_FIRMWARE_ROAMING) {
    aResult->mStatus = SetFirmwareRoaming(aOptions.mEnabled);
  } else if (aOptions.mCmd == nsIWifiCommand::CONFIG_FIRMWARE_ROAMING) {
    aResult->mStatus = ConfigureFirmwareRoaming(&aOptions.mRoamingConfig);
  } else if (aOptions.mCmd == nsIWifiCommand::START_SINGLE_SCAN) {
    aResult->mStatus = StartSingleScan(&aOptions.mScanSettings);
  } else if (aOptions.mCmd == nsIWifiCommand::STOP_SINGLE_SCAN) {
    aResult->mStatus = StopSingleScan();
  } else if (aOptions.mCmd == nsIWifiCommand::START_PNO_SCAN) {
    aResult->mStatus = StartPnoScan(&aOptions.mPnoScanSettings);
  } else if (aOptions.mCmd == nsIWifiCommand::STOP_PNO_SCAN) {
    aResult->mStatus = StopPnoScan();
  } else if (aOptions.mCmd == nsIWifiCommand::GET_SCAN_RESULTS) {
    std::vector<Wificond::NativeScanResult> nativeScanResults;

    if (aOptions.mScanType == nsIScanSettings::USE_SINGLE_SCAN) {
      aResult->mStatus = GetScanResults(nativeScanResults);
    } else if (aOptions.mScanType == nsIScanSettings::USE_PNO_SCAN) {
      aResult->mStatus = GetPnoScanResults(nativeScanResults);
    } else {
      WIFI_LOGE(LOG_TAG, "Invalid scan type: %d", aOptions.mScanType);
    }

    if (nativeScanResults.empty()) {
      WIFI_LOGD(LOG_TAG, "No scan result available");
      return false;
    }
    size_t num = nativeScanResults.size();
    nsTArray<RefPtr<nsScanResult>> scanResults(num);

    for (auto result : nativeScanResults) {
      std::string ssid_str(result.ssid.begin(), result.ssid.end());
      std::string bssid_str = ConvertMacToString(result.bssid);
      nsString ssid(NS_ConvertUTF8toUTF16(ssid_str.c_str()));
      nsString bssid(NS_ConvertUTF8toUTF16(bssid_str.c_str()));

      size_t ie_size = result.info_element.size();
      nsTArray<uint8_t> info_element(ie_size);
      for (auto& element : result.info_element) {
        info_element.AppendElement(element);
      }
      RefPtr<nsScanResult> scanResult = new nsScanResult(
          ssid, bssid, info_element, result.frequency, result.tsf,
          result.capability, result.signal_mbm, result.associated);
      scanResults.AppendElement(scanResult);
    }
    aResult->updateScanResults(scanResults);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_CHANNELS_FOR_BAND) {
    std::vector<int32_t> channels;
    aResult->mStatus = GetChannelsForBand(aOptions.mBandMask, channels);
    size_t num = channels.size();
    if (num > 0) {
      nsTArray<int32_t> channel_array(num);
      for (int32_t& ch : channels) {
        channel_array.AppendElement(ch);
      }
      aResult->updateChannels(channel_array);
    }
  } else if (aOptions.mCmd == nsIWifiCommand::CONNECT) {
    aResult->mStatus = Connect(&aOptions.mConfig);
  } else if (aOptions.mCmd == nsIWifiCommand::RECONNECT) {
    aResult->mStatus = Reconnect();
  } else if (aOptions.mCmd == nsIWifiCommand::REASSOCIATE) {
    aResult->mStatus = Reassociate();
  } else if (aOptions.mCmd == nsIWifiCommand::DISCONNECT) {
    aResult->mStatus = Disconnect();
  } else if (aOptions.mCmd == nsIWifiCommand::ENABLE_NETWORK) {
    aResult->mStatus = EnableNetwork();
  } else if (aOptions.mCmd == nsIWifiCommand::DISABLE_NETWORK) {
    aResult->mStatus = DisableNetwork();
  } else if (aOptions.mCmd == nsIWifiCommand::REMOVE_NETWORKS) {
    aResult->mStatus = RemoveNetworks();
  } else if (aOptions.mCmd == nsIWifiCommand::START_ROAMING) {
    aResult->mStatus = StartRoaming(&aOptions.mConfig);
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_IDENTITY_RESPONSE) {
    aResult->mStatus = SendEapSimIdentityResponse(&aOptions.mIdentityResp);
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_GSM_AUTH_RESPONSE) {
    aResult->mStatus = SendEapSimGsmAuthResponse(aOptions.mGsmAuthResp);
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_GSM_AUTH_FAILURE) {
    aResult->mStatus = SendEapSimGsmAuthFailure();
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_UMTS_AUTH_RESPONSE) {
    aResult->mStatus = SendEapSimUmtsAuthResponse(&aOptions.mUmtsAuthResp);
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_UMTS_AUTS_RESPONSE) {
    aResult->mStatus = SendEapSimUmtsAutsResponse(&aOptions.mUmtsAutsResp);
  } else if (aOptions.mCmd == nsIWifiCommand::SEND_UMTS_AUTH_FAILURE) {
    aResult->mStatus = SendEapSimUmtsAuthFailure();
  } else if (aOptions.mCmd == nsIWifiCommand::START_SOFTAP) {
    aResult->mStatus =
        StartSoftAp(&aOptions.mSoftapConfig, aResult->mApInterface);
  } else if (aOptions.mCmd == nsIWifiCommand::STOP_SOFTAP) {
    aResult->mStatus = StopSoftAp();
  } else if (aOptions.mCmd == nsIWifiCommand::GET_AP_IFACE) {
    aResult->mStatus = GetSoftApInterfaceName(aResult->mApInterface);
  } else if (aOptions.mCmd == nsIWifiCommand::GET_SOFTAP_STATION_NUMBER) {
    aResult->mStatus = GetSoftapStations(aResult->mNumStations);
  } else {
    WIFI_LOGE(LOG_TAG, "ExecuteCommand: Unknown command %d", aOptions.mCmd);
    return false;
  }
  WIFI_LOGD(LOG_TAG, "command result: id=%d, status=%d", aResult->mId,
            aResult->mStatus);

  return true;
}

void WifiNative::RegisterEventCallback(WifiEventCallback* aCallback) {
  sCallback = aCallback;
  if (sSupplicantStaManager) {
    sSupplicantStaManager->RegisterEventCallback(sCallback);
  }
}

void WifiNative::UnregisterEventCallback() {
  if (sSupplicantStaManager) {
    sSupplicantStaManager->UnregisterEventCallback();
  }
  sCallback = nullptr;
}

Result_t WifiNative::InitHal() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  // make sure wifi hal is ready
  result = sWifiHal->InitHalInterface();
  if (result != nsIWifiResult::SUCCESS) {
    return result;
  }

  result = sWificondControl->InitWificondInterface();
  if (result != nsIWifiResult::SUCCESS) {
    return result;
  }

  // init supplicant hal
  if (!sSupplicantStaManager->IsInterfaceInitializing()) {
    result = sSupplicantStaManager->InitInterface();
    if (result != nsIWifiResult::SUCCESS) {
      return result;
    }
  }
  return nsIWifiResult::SUCCESS;
}

Result_t WifiNative::DeinitHal() { return nsIWifiResult::SUCCESS; }

Result_t WifiNative::GetSupportedFeatures(uint32_t& aSupportedFeatures) {
  return sWifiHal->GetSupportedFeatures(aSupportedFeatures);
}

Result_t WifiNative::GetDriverModuleInfo(nsAString& aDriverVersion,
                                         nsAString& aFirmwareVersion) {
  return sWifiHal->GetDriverModuleInfo(aDriverVersion, aFirmwareVersion);
}

Result_t WifiNative::SetLowLatencyMode(bool aEnable) {
  return sWifiHal->SetLowLatencyMode(aEnable);
}

Result_t WifiNative::SetConcurrencyPriority(bool aEnable) {
  return sSupplicantStaManager->SetConcurrencyPriority(aEnable);
}

/**
 * To enable wifi and start supplicant
 *
 * @param aIfaceName - output wlan module interface name
 *
 * 1. load wifi driver module, configure chip.
 * 2. setup client mode interface.
 * 3. start supplicant.
 */
Result_t WifiNative::StartWifi(nsAString& aIfaceName) {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  result = sWifiHal->StartWifiModule();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to start wifi");
    return result;
  }

  WIFI_LOGD(LOG_TAG, "module loaded, try to configure...");
  result = sWifiHal->ConfigChipAndCreateIface(wifiNameSpaceV1_0::IfaceType::STA,
                                              mStaInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to create sta interface");
    return result;
  } else {
    sWifiHal->EnableLinkLayerStats();
  }

  // here create scan and pno scan event service,
  // which implement scan callback from wificond
  mScanEventService = ScanEventService::CreateService(mStaInterfaceName);
  if (mScanEventService == nullptr) {
    WIFI_LOGE(LOG_TAG, "Failed to create scan event service");
    return nsIWifiResult::ERROR_COMMAND_FAILED;
  }
  mScanEventService->RegisterEventCallback(sCallback);

  mPnoScanEventService = PnoScanEventService::CreateService(mStaInterfaceName);
  if (mPnoScanEventService == nullptr) {
    WIFI_LOGE(LOG_TAG, "Failed to create pno scan event service");
    return nsIWifiResult::ERROR_COMMAND_FAILED;
  }
  mPnoScanEventService->RegisterEventCallback(sCallback);

  result = StartSupplicant();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to initialize supplicant");
    return result;
  }

  // supplicant initialized, register death handler
  SupplicantDeathHandler* deathHandler = new SupplicantDeathHandler();
  sSupplicantStaManager->RegisterDeathHandler(deathHandler);

  result = sWificondControl->SetupClientIface(
      mStaInterfaceName,
      android::interface_cast<android::net::wifi::IScanEvent>(
          mScanEventService),
      android::interface_cast<android::net::wifi::IPnoScanEvent>(
          mPnoScanEventService));

  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to setup iface in wificond");
    sWificondControl->TearDownClientInterface(mStaInterfaceName);
    return result;
  }

  result = sSupplicantStaManager->SetupStaInterface(mStaInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to setup iface in supplicant");
    return result;
  }

  nsString iface(NS_ConvertUTF8toUTF16(mStaInterfaceName.c_str()));
  aIfaceName.Assign(iface);
  return CHECK_SUCCESS(aIfaceName.Length() > 0);
}

/**
 * To disable wifi
 *
 * 1. clean supplicant hidl client and stop supplicant
 * 2. clean client interfaces in wificond
 * 3. clean wifi hidl client and unload wlan module
 */
Result_t WifiNative::StopWifi() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  result = StopSupplicant();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to stop supplicant");
    return result;
  }

  if (mScanEventService) {
    mScanEventService->UnregisterEventCallback();
  }
  if (mPnoScanEventService) {
    mPnoScanEventService->UnregisterEventCallback();
  }

  // teardown wificond interfaces.
  result = sWificondControl->TearDownClientInterface(mStaInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to teardown wificond interfaces");
    return result;
  }

  // unregister supplicant death Handler
  sSupplicantStaManager->UnregisterDeathHandler();

  result = sWifiHal->TearDownInterface(wifiNameSpaceV1_0::IfaceType::STA);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to stop wifi");
    return result;
  }
  return nsIWifiResult::SUCCESS;
}

/**
 * Steps to setup supplicant.
 *
 * 1. initialize supplicant hidl client.
 * 2. start supplicant daemon through wificond or ctl.stat.
 * 3. wait for hidl client registration ready.
 */
Result_t WifiNative::StartSupplicant() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  // start supplicant hal
  if (!sSupplicantStaManager->IsInterfaceReady()) {
    result = sSupplicantStaManager->InitInterface();
    if (result != nsIWifiResult::SUCCESS) {
      WIFI_LOGE(LOG_TAG, "Failed to initialize supplicant hal");
      return result;
    }
  }

  // start supplicant from wificond.
  result = sWificondControl->StartSupplicant();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to start supplicant daemon");
    return result;
  }

  bool connected = false;
  int32_t connectTries = 0;
  while (!connected && connectTries++ < CONNECTION_RETRY_TIMES) {
    // Check if the initialization is complete.
    if (sSupplicantStaManager->IsInterfaceReady()) {
      connected = true;
      break;
    }
    usleep(CONNECTION_RETRY_INTERVAL_US);
  }
  return CHECK_SUCCESS(connected);
}

Result_t WifiNative::StopSupplicant() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  // teardown supplicant hal interfaces
  result = sSupplicantStaManager->DeinitInterface();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to teardown iface in supplicant");
    return result;
  }

  result = sWificondControl->StopSupplicant();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to stop supplicant");
    return result;
  }
  return nsIWifiResult::SUCCESS;
}

Result_t WifiNative::GetMacAddress(nsAString& aMacAddress) {
  return sSupplicantStaManager->GetMacAddress(aMacAddress);
}

Result_t WifiNative::GetClientInterfaceName(nsAString& aIfaceName) {
  nsString iface(NS_ConvertUTF8toUTF16(mStaInterfaceName.c_str()));
  aIfaceName.Assign(iface);
  return CHECK_SUCCESS(aIfaceName.Length() > 0);
}

Result_t WifiNative::GetSoftApInterfaceName(nsAString& aIfaceName) {
  nsString iface(NS_ConvertUTF8toUTF16(mApInterfaceName.c_str()));
  aIfaceName.Assign(iface);
  return CHECK_SUCCESS(aIfaceName.Length() > 0);
}

Result_t WifiNative::GetDebugLevel(uint32_t& aLevel) {
  return sSupplicantStaManager->GetSupplicantDebugLevel(aLevel);
}

Result_t WifiNative::SetDebugLevel(SupplicantDebugLevelOptions* aLevel) {
  return sSupplicantStaManager->SetSupplicantDebugLevel(aLevel);
}

Result_t WifiNative::SetPowerSave(bool aEnable) {
  return sSupplicantStaManager->SetPowerSave(aEnable);
}

Result_t WifiNative::SetSuspendMode(bool aEnable) {
  return sSupplicantStaManager->SetSuspendMode(aEnable);
}

Result_t WifiNative::SetExternalSim(bool aEnable) {
  return sSupplicantStaManager->SetExternalSim(aEnable);
}

Result_t WifiNative::SetAutoReconnect(bool aEnable) {
  return sSupplicantStaManager->SetAutoReconnect(aEnable);
}

Result_t WifiNative::SetBtCoexistenceMode(uint8_t aMode) {
  return sSupplicantStaManager->SetBtCoexistenceMode(aMode);
}

Result_t WifiNative::SetBtCoexistenceScanMode(bool aEnable) {
  return sSupplicantStaManager->SetBtCoexistenceScanMode(aEnable);
}

Result_t WifiNative::SignalPoll(std::vector<int32_t>& aPollResult) {
  return sWificondControl->SignalPoll(aPollResult);
}

Result_t WifiNative::GetLinkLayerStats(
    wifiNameSpaceV1_0::StaLinkLayerStats& aStats) {
  return sWifiHal->GetLinkLayerStats(aStats);
}

Result_t WifiNative::SetCountryCode(const nsAString& aCountryCode) {
  std::string countryCode = NS_ConvertUTF16toUTF8(aCountryCode).get();
  return sSupplicantStaManager->SetCountryCode(countryCode);
}

Result_t WifiNative::SetFirmwareRoaming(bool aEnable) {
  return sWifiHal->SetFirmwareRoaming(aEnable);
}

Result_t WifiNative::ConfigureFirmwareRoaming(
    RoamingConfigurationOptions* aRoamingConfig) {
  return sWifiHal->ConfigureFirmwareRoaming(aRoamingConfig);
}

Result_t WifiNative::StartSingleScan(ScanSettingsOptions* aScanSettings) {
  return sWificondControl->StartSingleScan(aScanSettings);
}

Result_t WifiNative::StopSingleScan() {
  return sWificondControl->StopSingleScan();
}

Result_t WifiNative::StartPnoScan(PnoScanSettingsOptions* aPnoScanSettings) {
  return sWificondControl->StartPnoScan(aPnoScanSettings);
}

Result_t WifiNative::StopPnoScan() { return sWificondControl->StopPnoScan(); }

Result_t WifiNative::GetScanResults(
    std::vector<Wificond::NativeScanResult>& aScanResults) {
  return sWificondControl->GetScanResults(aScanResults);
}

Result_t WifiNative::GetPnoScanResults(
    std::vector<Wificond::NativeScanResult>& aPnoScanResults) {
  return sWificondControl->GetPnoScanResults(aPnoScanResults);
}

Result_t WifiNative::GetChannelsForBand(uint32_t aBandMask,
                                        std::vector<int32_t>& aChannels) {
  return sWificondControl->GetChannelsForBand(aBandMask, aChannels);
}

/**
 * To make wifi connection with assigned configuration
 *
 * @param aConfig - the network configuration to be set
 */
Result_t WifiNative::Connect(ConfigurationOptions* aConfig) {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  // abort ongoing scan before connect
  sWificondControl->StopSingleScan();

  result = sSupplicantStaManager->ConnectToNetwork(aConfig);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to connect %s",
              NS_ConvertUTF16toUTF8(aConfig->mSsid).get());
    return result;
  }
  return nsIWifiResult::SUCCESS;
}

Result_t WifiNative::Reconnect() { return sSupplicantStaManager->Reconnect(); }

Result_t WifiNative::Reassociate() {
  return sSupplicantStaManager->Reassociate();
}

Result_t WifiNative::Disconnect() {
  return sSupplicantStaManager->Disconnect();
}

Result_t WifiNative::EnableNetwork() {
  return sSupplicantStaManager->EnableNetwork();
}

Result_t WifiNative::DisableNetwork() {
  return sSupplicantStaManager->DisableNetwork();
}

/**
 * To remove all configured networks in supplicant
 */
Result_t WifiNative::RemoveNetworks() {
  return sSupplicantStaManager->RemoveNetworks();
}

Result_t WifiNative::StartRoaming(ConfigurationOptions* aConfig) {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  result = sSupplicantStaManager->RoamToNetwork(aConfig);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Roam to %s failed",
              NS_ConvertUTF16toUTF8(aConfig->mSsid).get());
  }
  return result;
}

Result_t WifiNative::SendEapSimIdentityResponse(
    SimIdentityRespDataOptions* aIdentity) {
  return sSupplicantStaManager->SendEapSimIdentityResponse(aIdentity);
}

Result_t WifiNative::SendEapSimGsmAuthResponse(
    const nsTArray<SimGsmAuthRespDataOptions>& aGsmAuthResp) {
  return sSupplicantStaManager->SendEapSimGsmAuthResponse(aGsmAuthResp);
}

Result_t WifiNative::SendEapSimGsmAuthFailure() {
  return sSupplicantStaManager->SendEapSimGsmAuthFailure();
}

Result_t WifiNative::SendEapSimUmtsAuthResponse(
    SimUmtsAuthRespDataOptions* aUmtsAuthResp) {
  return sSupplicantStaManager->SendEapSimUmtsAuthResponse(aUmtsAuthResp);
}

Result_t WifiNative::SendEapSimUmtsAutsResponse(
    SimUmtsAutsRespDataOptions* aUmtsAutsResp) {
  return sSupplicantStaManager->SendEapSimUmtsAutsResponse(aUmtsAutsResp);
}

Result_t WifiNative::SendEapSimUmtsAuthFailure() {
  return sSupplicantStaManager->SendEapSimUmtsAuthFailure();
}

/**
 * To enable wifi hotspot
 *
 * @param aSoftapConfig - the softap configuration to be set
 * @param aIfaceName - out the interface name for AP mode
 *
 * 1. load driver module and configure chip as AP mode
 * 2. start hostapd hidl service an register callback
 * 3. with lazy hal designed, hostapd daemon should be
 *    started while getService() of IHostapd
 * 4. setup ap in wificond, which will listen to event from driver
 */
Result_t WifiNative::StartSoftAp(SoftapConfigurationOptions* aSoftapConfig,
                                 nsAString& aIfaceName) {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  // Load wifi driver module and configure as ap mode.
  result = sWifiHal->StartWifiModule();
  if (result != nsIWifiResult::SUCCESS) {
    return result;
  }

  result = StartAndConnectHostapd();
  if (result != nsIWifiResult::SUCCESS) {
    return result;
  }

  result = sWifiHal->ConfigChipAndCreateIface(wifiNameSpaceV1_0::IfaceType::AP,
                                              mApInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to create AP interface");
    return result;
  }

  mSoftapEventService = SoftapEventService::CreateService(mApInterfaceName);
  if (mSoftapEventService == nullptr) {
    WIFI_LOGE(LOG_TAG, "Failed to create softap event service");
    return nsIWifiResult::ERROR_COMMAND_FAILED;
  }
  mSoftapEventService->RegisterEventCallback(sCallback);

  result = sWificondControl->SetupApIface(
      mApInterfaceName,
      android::interface_cast<android::net::wifi::IApInterfaceEventCallback>(
          mSoftapEventService));
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to setup softap iface in wificond");
    sWificondControl->TearDownSoftapInterface(mApInterfaceName);
    return result;
  }

  // Up to now, ap interface should be ready to setup country code.
  std::string countryCode =
      NS_ConvertUTF16toUTF8(aSoftapConfig->mCountryCode).get();
  result = sWifiHal->SetSoftapCountryCode(countryCode);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to set country code");
    return result;
  }

  // start softap from hostapd.
  result =
      sSoftapManager->StartSoftap(mApInterfaceName, countryCode, aSoftapConfig);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to start softap");
    return result;
  }

  nsString iface(NS_ConvertUTF8toUTF16(mApInterfaceName.c_str()));
  aIfaceName.Assign(iface);
  return CHECK_SUCCESS(aIfaceName.Length() > 0);
}

/**
 * To disable wifi hotspot
 *
 * 1. clean hostapd hidl client and stop daemon
 * 2. clean ap interfaces in wificond
 * 3. clean wifi hidl client and unload wlan module
 */
Result_t WifiNative::StopSoftAp() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  result = sSoftapManager->StopSoftap(mApInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to stop softap");
    return result;
  }

  if (mSoftapEventService) {
    mSoftapEventService->UnregisterEventCallback();
  }

  result = sWificondControl->TearDownSoftapInterface(mApInterfaceName);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to teardown ap interface in wificond");
    return result;
  }

  result = StopHostapd();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to stop hostapd");
    return result;
  }

  result = sWifiHal->TearDownInterface(wifiNameSpaceV1_0::IfaceType::AP);
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to teardown softap interface");
    return result;
  }
  return nsIWifiResult::SUCCESS;
}

Result_t WifiNative::StartAndConnectHostapd() {
  Result_t result = nsIWifiResult::ERROR_UNKNOWN;

  result = sSoftapManager->InitInterface();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to initialize hostapd interface");
    return result;
  }

  bool connected = false;
  int32_t connectTries = 0;
  while (!connected && connectTries++ < CONNECTION_RETRY_TIMES) {
    // Check if the initialization is complete.
    if (sSoftapManager->IsInterfaceReady()) {
      connected = true;
      break;
    }
    usleep(CONNECTION_RETRY_INTERVAL_US);
  }
  return CHECK_SUCCESS(connected);
}

Result_t WifiNative::StopHostapd() {
  Result_t result = sSoftapManager->DeinitInterface();
  if (result != nsIWifiResult::SUCCESS) {
    WIFI_LOGE(LOG_TAG, "Failed to tear down hostapd interface");
    return result;
  }
  return nsIWifiResult::SUCCESS;
}

void WifiNative::SupplicantDeathHandler::OnDeath() {
  // supplicant died, start to clean up.
  WIFI_LOGE(LOG_TAG, "Supplicant DIED: ##############################");
}

Result_t WifiNative::GetSoftapStations(uint32_t& aNumStations) {
  return sWificondControl->GetSoftapStations(aNumStations);
}
