/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsRilIndication.h"

/* Logging related */
#undef LOG_TAG
#define LOG_TAG "RilIndication"

#undef INFO
#undef ERROR
#undef DEBUG
#define INFO(args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##args)
#define ERROR(args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##args)
#define DEBUG(args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, ##args)

/**
 *
 */
nsRilIndication::nsRilIndication(nsRilWorker* aRil) {
  INFO("init nsRilIndication");
  mRIL = aRil;
  INFO("init nsRilIndication done");
}

nsRilIndication::~nsRilIndication() {
  INFO("Destructor nsRilIndication");
  mRIL = nullptr;
  MOZ_ASSERT(!mRIL);
}

Return<void> nsRilIndication::radioStateChanged(RadioIndicationType type,
                                                RadioState radioState) {
  INFO("radioStateChanged");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("radiostatechange"));
  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateRadioStateChanged(convertRadioStateToNum(radioState));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::callStateChanged(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("callStateChanged"));
  return Void();
}

Return<void> nsRilIndication::networkStateChanged(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("networkStateChanged"));
  return Void();
}

Return<void> nsRilIndication::newSms(
    RadioIndicationType /*type*/,
    const ::android::hardware::hidl_vec<uint8_t>& /*pdu*/) {
  INFO("Not implement newSms");
  return Void();
}

Return<void> nsRilIndication::newSmsStatusReport(
    RadioIndicationType /*type*/,
    const ::android::hardware::hidl_vec<uint8_t>& /*pdu*/) {
  INFO("Not implement newSmsStatusReport");
  return Void();
}

Return<void> nsRilIndication::newSmsOnSim(RadioIndicationType /*type*/,
                                          int32_t /*recordNumber*/) {
  INFO("Not implement newSmsOnSim");
  return Void();
}

Return<void> nsRilIndication::onUssd(
    RadioIndicationType type, UssdModeType modeType,
    const ::android::hardware::hidl_string& msg) {
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("ussdreceived"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateOnUssd(convertUssdModeType(modeType),
                       NS_ConvertUTF8toUTF16(msg.c_str()));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::nitzTimeReceived(
    RadioIndicationType type, const ::android::hardware::hidl_string& nitzTime,
    uint64_t receivedTime) {
  INFO("nitzTimeReceived");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("nitzTimeReceived"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateNitzTimeReceived(NS_ConvertUTF8toUTF16(nitzTime.c_str()),
                                 receivedTime);
  mRIL->sendRilIndicationResult(result);

  return Void();
}

Return<void> nsRilIndication::currentSignalStrength(
    RadioIndicationType type, const SignalStrength& sig_strength) {
  INFO("currentSignalStrength");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("signalstrengthchange"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  RefPtr<nsSignalStrength> signalStrength =
      result->convertSignalStrength(sig_strength);
  result->updateCurrentSignalStrength(signalStrength);
  mRIL->sendRilIndicationResult(result);

  return Void();
}

Return<void> nsRilIndication::dataCallListChanged(
    RadioIndicationType type,
    const ::android::hardware::hidl_vec<SetupDataCallResult>& dcList) {
  INFO("dataCallListChanged");
  mRIL->processIndication(type);

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(NS_LITERAL_STRING("datacalllistchanged"));
  uint32_t numDataCall = dcList.size();
  INFO("getDataCallListResponse numDataCall= %d", numDataCall);
  nsTArray<RefPtr<nsSetupDataCallResult>> aDcLists(numDataCall);

  for (uint32_t i = 0; i < numDataCall; i++) {
    RefPtr<nsSetupDataCallResult> datcall =
        result->convertDcResponse(dcList[i]);
    aDcLists.AppendElement(datcall);
  }
  result->updateDataCallListChanged(aDcLists);

  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::suppSvcNotify(
    RadioIndicationType type, const SuppSvcNotification& suppSvc) {
  INFO("suppSvcNotification");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("suppSvcNotification"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  RefPtr<nsSuppSvcNotification> notify = new nsSuppSvcNotification(
      suppSvc.isMT, suppSvc.code, suppSvc.index, suppSvc.type,
      NS_ConvertUTF8toUTF16(suppSvc.number.c_str()));
  result->updateSuppSvcNotify(notify);
  mRIL->sendRilIndicationResult(result);

  return Void();
}

Return<void> nsRilIndication::stkSessionEnd(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("stksessionend"));
  return Void();
}

Return<void> nsRilIndication::stkProactiveCommand(
    RadioIndicationType type,
    const ::android::hardware::hidl_string& cmd) {
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("stkProactiveCommand"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateStkProactiveCommand(NS_ConvertUTF8toUTF16(cmd.c_str()));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::stkEventNotify(
    RadioIndicationType type,
    const ::android::hardware::hidl_string& cmd) {
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("stkEventNotify"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateStkEventNotify(NS_ConvertUTF8toUTF16(cmd.c_str()));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::stkCallSetup(RadioIndicationType /*type*/,
                                           int64_t /*timeout*/) {
  INFO("Not implement stkCallSetup");
  return Void();
}

Return<void> nsRilIndication::simSmsStorageFull(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("simSmsStorageFull"));
  return Void();
}

Return<void> nsRilIndication::simRefresh(
    RadioIndicationType type, const SimRefreshResult& refreshResult) {
  INFO("simRefresh");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("simRefresh"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  RefPtr<nsSimRefreshResult> simRefresh = new nsSimRefreshResult(
      convertSimRefreshType(refreshResult.type), refreshResult.efId,
      NS_ConvertUTF8toUTF16(refreshResult.aid.c_str()));
  result->updateSimRefresh(simRefresh);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::callRing(RadioIndicationType type, bool isGsm,
                                       const CdmaSignalInfoRecord& record) {
  mRIL->processIndication(type);

  // TODO impelment CDMA later.

  nsString rilmessageType(NS_LITERAL_STRING("callRing"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::simStatusChanged(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("simStatusChanged"));
  return Void();
}

Return<void> nsRilIndication::cdmaNewSms(RadioIndicationType /*type*/,
                                         const CdmaSmsMessage& /*msg*/) {
  INFO("Not implement cdmaNewSms");
  return Void();
}

Return<void> nsRilIndication::newBroadcastSms(
    RadioIndicationType /*type*/,
    const ::android::hardware::hidl_vec<uint8_t>& /*data*/) {
  INFO("Not implement newBroadcastSms");
  return Void();
}

Return<void> nsRilIndication::cdmaRuimSmsStorageFull(
    RadioIndicationType /*type*/) {
  INFO("Not implement cdmaRuimSmsStorageFull");
  return Void();
}

Return<void> nsRilIndication::restrictedStateChanged(
    RadioIndicationType type, PhoneRestrictedState state) {
  INFO("restrictedStateChanged");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("restrictedStateChanged"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateRestrictedStateChanged(convertPhoneRestrictedState(state));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::enterEmergencyCallbackMode(
    RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("enterEmergencyCbMode"));
  return Void();
}

Return<void> nsRilIndication::cdmaCallWaiting(
    RadioIndicationType /*type*/,
    const CdmaCallWaiting& /*callWaitingRecord*/) {
  INFO("Not implement cdmaCallWaiting");
  return Void();
}

Return<void> nsRilIndication::cdmaOtaProvisionStatus(
    RadioIndicationType /*type*/, CdmaOtaProvisionStatus /*status*/) {
  INFO("Not implement cdmaOtaProvisionStatus");
  return Void();
}

Return<void> nsRilIndication::cdmaInfoRec(
    RadioIndicationType /*type*/, const CdmaInformationRecords& /*records*/) {
  INFO("Not implement cdmaInfoRec");
  return Void();
}

Return<void> nsRilIndication::indicateRingbackTone(RadioIndicationType type,
                                                   bool start) {
  INFO("ringbackTone");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("ringbackTone"));
  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateIndicateRingbackTone(start);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::resendIncallMute(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("resendIncallMute"));
  return Void();
}

Return<void> nsRilIndication::cdmaSubscriptionSourceChanged(
    RadioIndicationType /*type*/, CdmaSubscriptionSource /*cdmaSource*/) {
  INFO("Not implement cdmaSubscriptionSourceChanged");
  return Void();
}

Return<void> nsRilIndication::cdmaPrlChanged(RadioIndicationType /*type*/,
                                             int32_t /*version*/) {
  INFO("Not implement cdmaPrlChanged");
  return Void();
}

Return<void> nsRilIndication::exitEmergencyCallbackMode(
    RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("exitEmergencyCbMode"));
  return Void();
}

Return<void> nsRilIndication::rilConnected(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("rilconnected"));
  return Void();
}

Return<void> nsRilIndication::voiceRadioTechChanged(RadioIndicationType type,
                                                    RadioTechnology rat) {
  INFO("voiceRadioTechChanged");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("voiceRadioTechChanged"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateVoiceRadioTechChanged(nsRilResult::convertRadioTechnology(rat));
  mRIL->sendRilIndicationResult(result);

  return Void();
}

Return<void> nsRilIndication::cellInfoList(
    RadioIndicationType type,
    const ::android::hardware::hidl_vec<CellInfo>& records) {
  INFO("cellInfoList");
  mRIL->processIndication(type);

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(NS_LITERAL_STRING("cellInfoList"));
  uint32_t numCellInfo = records.size();
  INFO("cellInfoList numCellInfo= %d", numCellInfo);
  nsTArray<RefPtr<nsRilCellInfo>> aCellInfoLists(numCellInfo);

  for (uint32_t i = 0; i < numCellInfo; i++) {
    RefPtr<nsRilCellInfo> cellInfo = result->convertRilCellInfo(records[i]);
    aCellInfoLists.AppendElement(cellInfo);
  }
  result->updateCellInfoList(aCellInfoLists);

  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::imsNetworkStateChanged(RadioIndicationType type) {
  defaultResponse(type, NS_LITERAL_STRING("imsNetworkStateChanged"));
  return Void();
}

Return<void> nsRilIndication::subscriptionStatusChanged(
    RadioIndicationType type, bool activate) {
  INFO("subscriptionStatusChanged");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("subscriptionStatusChanged"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateSubscriptionStatusChanged(activate);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::srvccStateNotify(RadioIndicationType type,
                                               SrvccState state) {
  INFO("srvccStateNotify");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("srvccStateNotify"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateSrvccStateNotify(convertSrvccState(state));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::hardwareConfigChanged(
    RadioIndicationType type,
    const ::android::hardware::hidl_vec<HardwareConfig>& configs) {
  INFO("hardwareConfigChanged");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("hardwareConfigChanged"));
  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);

  uint32_t numConfigs = configs.size();
  nsTArray<RefPtr<nsHardwareConfig>> aHWConfigLists(numConfigs);

  for (uint32_t i = 0; i < numConfigs; i++) {
    int32_t type = convertHardwareConfigType(configs[i].type);
    RefPtr<nsHardwareConfig> hwConfig = nullptr;
    RefPtr<nsHardwareConfigModem> hwConfigModem = nullptr;
    RefPtr<nsHardwareConfigSim> hwConfigSim = nullptr;

    if (type == nsIRilIndicationResult::HW_CONFIG_TYPE_MODEM) {
      hwConfigModem = new nsHardwareConfigModem(
          configs[i].modem[0].rilModel, configs[i].modem[0].rat,
          configs[i].modem[0].maxVoice, configs[i].modem[0].maxData,
          configs[i].modem[0].maxStandby);
    } else {
      hwConfigSim = new nsHardwareConfigSim(
          NS_ConvertUTF8toUTF16(configs[i].sim[0].modemUuid.c_str()));
    }

    hwConfig = new nsHardwareConfig(
        type, NS_ConvertUTF8toUTF16(configs[i].uuid.c_str()),
        convertHardwareConfigState(configs[i].state), hwConfigModem,
        hwConfigSim);
    aHWConfigLists.AppendElement(hwConfig);
  }
  result->updateHardwareConfigChanged(aHWConfigLists);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::radioCapabilityIndication(
    RadioIndicationType type, const RadioCapability& rc) {
  INFO("radioCapabilityIndication");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("radioCapabilityIndication"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  RefPtr<nsRadioCapability> capability =
      new nsRadioCapability(rc.session, convertRadioCapabilityPhase(rc.phase),
                            convertRadioAccessFamily(RadioAccessFamily(rc.raf)),
                            NS_ConvertUTF8toUTF16(rc.logicalModemUuid.c_str()),
                            convertRadioCapabilityStatus(rc.status));
  result->updateRadioCapabilityIndication(capability);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::onSupplementaryServiceIndication(
    RadioIndicationType /*type*/, const StkCcUnsolSsResult& /*ss*/) {
  INFO("Not implement onSupplementaryServiceIndication");
  return Void();
}

Return<void> nsRilIndication::stkCallControlAlphaNotify(
    RadioIndicationType /*type*/,
    const ::android::hardware::hidl_string& /*alpha*/) {
  INFO("Not implement stkCallControlAlphaNotify");
  return Void();
}

Return<void> nsRilIndication::lceData(RadioIndicationType type,
                                      const LceDataInfo& lce) {
  INFO("lceData");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("lceData"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  RefPtr<nsLceDataInfo> lceInfo = new nsLceDataInfo(
      lce.lastHopCapacityKbps, lce.confidenceLevel, lce.lceSuspended);
  result->updateLceData(lceInfo);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::pcoData(RadioIndicationType type,
                                      const PcoDataInfo& pco) {
  INFO("pcoData");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("pcoData"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);

  uint32_t numContents = pco.contents.size();
  nsTArray<int32_t> pcoContents(numContents);
  for (uint32_t i = 0; i < numContents; i++) {
    pcoContents.AppendElement((int32_t)pco.contents[i]);
  }

  RefPtr<nsPcoDataInfo> pcoInfo =
      new nsPcoDataInfo(pco.cid, NS_ConvertUTF8toUTF16(pco.bearerProto.c_str()),
                        pco.pcoId, pcoContents);
  result->updatePcoData(pcoInfo);
  mRIL->sendRilIndicationResult(result);
  return Void();
}

Return<void> nsRilIndication::modemReset(
    RadioIndicationType type, const ::android::hardware::hidl_string& reason) {
  INFO("modemReset");
  mRIL->processIndication(type);

  nsString rilmessageType(NS_LITERAL_STRING("modemReset"));

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  result->updateModemReset(NS_ConvertUTF8toUTF16(reason.c_str()));
  mRIL->sendRilIndicationResult(result);
  return Void();
}

// Helper function
void nsRilIndication::defaultResponse(const RadioIndicationType type,
                                      const nsString& rilmessageType) {
  mRIL->processIndication(type);

  RefPtr<nsRilIndicationResult> result =
      new nsRilIndicationResult(rilmessageType);
  mRIL->sendRilIndicationResult(result);
}
int32_t nsRilIndication::convertRadioStateToNum(RadioState state) {
  switch (state) {
    case RadioState::OFF:
      return nsIRilIndicationResult::RADIOSTATE_DISABLED;
    case RadioState::UNAVAILABLE:
      return nsIRilIndicationResult::RADIOSTATE_UNKNOWN;
    case RadioState::ON:
      return nsIRilIndicationResult::RADIOSTATE_ENABLED;
    default:
      return nsIRilIndicationResult::RADIOSTATE_UNKNOWN;
  }
}

int32_t nsRilIndication::convertSimRefreshType(SimRefreshType type) {
  switch (type) {
    case SimRefreshType::SIM_FILE_UPDATE:
      return nsIRilIndicationResult::SIM_REFRESH_FILE_UPDATE;
    case SimRefreshType::SIM_INIT:
      return nsIRilIndicationResult::SIM_REFRESH_INIT;
    case SimRefreshType::SIM_RESET:
      return nsIRilIndicationResult::SIM_REFRESH_RESET;
    default:
      return nsIRilIndicationResult::SIM_REFRESH_UNKNOW;
  }
}

int32_t nsRilIndication::convertPhoneRestrictedState(
    PhoneRestrictedState state) {
  switch (state) {
    case PhoneRestrictedState::NONE:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_NONE;
    case PhoneRestrictedState::CS_EMERGENCY:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_CS_EMERGENCY;
    case PhoneRestrictedState::CS_NORMAL:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_CS_NORMAL;
    case PhoneRestrictedState::CS_ALL:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_CS_ALL;
    case PhoneRestrictedState::PS_ALL:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_PS_ALL;
    default:
      return nsIRilIndicationResult::PHONE_RESTRICTED_STATE_NONE;
  }
}

int32_t nsRilIndication::convertUssdModeType(UssdModeType type) {
  switch (type) {
    case UssdModeType::NOTIFY:
      return nsIRilIndicationResult::USSD_MODE_NOTIFY;
    case UssdModeType::REQUEST:
      return nsIRilIndicationResult::USSD_MODE_REQUEST;
    case UssdModeType::NW_RELEASE:
      return nsIRilIndicationResult::USSD_MODE_NW_RELEASE;
    case UssdModeType::LOCAL_CLIENT:
      return nsIRilIndicationResult::USSD_MODE_LOCAL_CLIENT;
    case UssdModeType::NOT_SUPPORTED:
      return nsIRilIndicationResult::USSD_MODE_NOT_SUPPORTED;
    case UssdModeType::NW_TIMEOUT:
      return nsIRilIndicationResult::USSD_MODE_NW_TIMEOUT;
    default:
      // TODO need confirmed the default value.
      return nsIRilIndicationResult::USSD_MODE_NW_TIMEOUT;
  }
}

int32_t nsRilIndication::convertSrvccState(SrvccState state) {
  switch (state) {
    case SrvccState::HANDOVER_STARTED:
      return nsIRilIndicationResult::SRVCC_STATE_HANDOVER_STARTED;
    case SrvccState::HANDOVER_COMPLETED:
      return nsIRilIndicationResult::SRVCC_STATE_HANDOVER_COMPLETED;
    case SrvccState::HANDOVER_FAILED:
      return nsIRilIndicationResult::SRVCC_STATE_HANDOVER_FAILED;
    case SrvccState::HANDOVER_CANCELED:
      return nsIRilIndicationResult::SRVCC_STATE_HANDOVER_CANCELED;
    default:
      return nsIRilIndicationResult::SRVCC_STATE_HANDOVER_CANCELED;
  }
}

int32_t nsRilIndication::convertHardwareConfigType(HardwareConfigType state) {
  switch (state) {
    case HardwareConfigType::MODEM:
      return nsIRilIndicationResult::HW_CONFIG_TYPE_MODEM;
    case HardwareConfigType::SIM:
      return nsIRilIndicationResult::HW_CONFIG_TYPE_SIM;
    default:
      return nsIRilIndicationResult::HW_CONFIG_TYPE_MODEM;
  }
}

int32_t nsRilIndication::convertHardwareConfigState(HardwareConfigState state) {
  switch (state) {
    case HardwareConfigState::ENABLED:
      return nsIRilIndicationResult::HW_CONFIG_STATE_ENABLED;
    case HardwareConfigState::STANDBY:
      return nsIRilIndicationResult::HW_CONFIG_STATE_STANDBY;
    case HardwareConfigState::DISABLED:
      return nsIRilIndicationResult::HW_CONFIG_STATE_DISABLED;
    default:
      return nsIRilIndicationResult::HW_CONFIG_STATE_DISABLED;
  }
}

int32_t nsRilIndication::convertRadioCapabilityPhase(
    RadioCapabilityPhase value) {
  switch (value) {
    case RadioCapabilityPhase::CONFIGURED:
      return nsIRilIndicationResult::RADIO_CP_CONFIGURED;
    case RadioCapabilityPhase::START:
      return nsIRilIndicationResult::RADIO_CP_START;
    case RadioCapabilityPhase::APPLY:
      return nsIRilIndicationResult::RADIO_CP_APPLY;
    case RadioCapabilityPhase::UNSOL_RSP:
      return nsIRilIndicationResult::RADIO_CP_UNSOL_RSP;
    case RadioCapabilityPhase::FINISH:
      return nsIRilIndicationResult::RADIO_CP_FINISH;
    default:
      return nsIRilIndicationResult::RADIO_CP_FINISH;
  }
}

int32_t nsRilIndication::convertRadioAccessFamily(RadioAccessFamily value) {
  switch (value) {
    case RadioAccessFamily::UNKNOWN:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_UNKNOWN;
    case RadioAccessFamily::GPRS:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_GPRS;
    case RadioAccessFamily::EDGE:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_EDGE;
    case RadioAccessFamily::UMTS:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_UMTS;
    case RadioAccessFamily::IS95A:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_IS95A;
    case RadioAccessFamily::IS95B:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_IS95B;
    case RadioAccessFamily::ONE_X_RTT:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_ONE_X_RTT;
    case RadioAccessFamily::EVDO_0:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_EVDO_0;
    case RadioAccessFamily::EVDO_A:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_EVDO_A;
    case RadioAccessFamily::HSDPA:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_HSDPA;
    case RadioAccessFamily::HSUPA:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_HSUPA;
    case RadioAccessFamily::HSPA:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_HSPA;
    case RadioAccessFamily::EVDO_B:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_EVDO_B;
    case RadioAccessFamily::EHRPD:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_EHRPD;
    case RadioAccessFamily::LTE:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_LTE;
    case RadioAccessFamily::HSPAP:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_HSPAP;
    case RadioAccessFamily::GSM:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_GSM;
    case RadioAccessFamily::TD_SCDMA:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_TD_SCDMA;
    case RadioAccessFamily::LTE_CA:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_LTE_CA;
    default:
      return nsIRilIndicationResult::RADIO_ACCESS_FAMILY_UNKNOWN;
  }
}

int32_t nsRilIndication::convertRadioCapabilityStatus(
    RadioCapabilityStatus state) {
  switch (state) {
    case ::android::hardware::radio::V1_0::RadioCapabilityStatus::NONE:
      return nsIRilIndicationResult::RADIO_CP_STATUS_NONE;
    case ::android::hardware::radio::V1_0::RadioCapabilityStatus::SUCCESS:
      return nsIRilIndicationResult::RADIO_CP_STATUS_SUCCESS;
    case ::android::hardware::radio::V1_0::RadioCapabilityStatus::FAIL:
      return nsIRilIndicationResult::RADIO_CP_STATUS_FAIL;
    default:
      return nsIRilIndicationResult::RADIO_CP_STATUS_NONE;
  }
}
