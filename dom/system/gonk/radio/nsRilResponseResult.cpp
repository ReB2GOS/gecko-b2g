/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsRilResponseResult.h"

/* Logging related */
#undef LOG_TAG
#define LOG_TAG "nsRilResponseResult"

#undef INFO
#undef ERROR
#undef DEBUG
#define INFO(args...) \
  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##args)
#define ERROR(args...) \
  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##args)
#define DEBUG(args...) \
  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, ##args)


#define RILRESPONSERESULT_CID                        \
  {                                                  \
    0xe058deee, 0xe10a, 0x4165, {                    \
      0x80, 0x98, 0xfc, 0x86, 0x50, 0x13, 0xda, 0x1d \
    }                                                \
  }

/*============================================================================
 *======================Implementation of Class nsRilResponseResult =====================
 *============================================================================*/
NS_IMPL_ISUPPORTS(nsRilResponseResult, nsIRilResponseResult)

/**
 * Constructor for a nsRilResponseResult
 * For those has no parameter notify.
 */
nsRilResponseResult::nsRilResponseResult(const nsAString &aRilMessageType, int32_t aRilMessageToken, int32_t aErrorMsg) :
    nsRilResult(aRilMessageType, aRilMessageToken, aErrorMsg) {
  INFO("init nsRilResponseResult");
}
/**
 * For DeviceIdentity
 */
void nsRilResponseResult::updateDeviceIdentity(const nsAString &aImei, const nsAString &aImeisv,
  const nsAString &aEsn, const nsAString &aMeid) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateDeviceIdentity");
  mImei = aImei;
  mImeisv = aImeisv;
  mEsn = aEsn;
  mMeid = aMeid;
}

/**
 * For VoiceRadioTechnology
 */
void nsRilResponseResult::updateVoiceRadioTechnology(int32_t aRadioTech) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateVoiceRadioTechnology");
  mRadioTech = aRadioTech;
}

/**
 * For BasebandVersionResponse
 */
void nsRilResponseResult::updateBasebandVersion(const nsAString &aBasebandVersion) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateBasebandVersion");
  mBasebandVersion = aBasebandVersion;
}

/**
 * For IccCardStatus
 */
void nsRilResponseResult::updateIccCardStatus(nsCardStatus* aCardStatus) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateIccCardStatus");
  mCardStatus = aCardStatus;
}

/**
 * For VoiceRegStatus
 */
void nsRilResponseResult::updateVoiceRegStatus(nsVoiceRegState* aVoiceRegState) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateVoiceRegStatus");
  mVoiceRegState = aVoiceRegState;
}

/**
 * For DataRegStatus
 */
void nsRilResponseResult::updateDataRegStatus(nsDataRegState* aDataRegState) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateDataRegStatus");
  mDataRegState = aDataRegState;
}

/**
 * For OperatorInfo
 */
void nsRilResponseResult::updateOperator(nsOperatorInfo* aOperatorInfo) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateOperatorInfo");
  mOperatorInfo = aOperatorInfo;
}

/**
 * For NetworkSelectionMode
 */
void nsRilResponseResult::updateNetworkSelectionMode(bool aNwModeManual) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateNetworkSelectionMode");
  mNwModeManual = aNwModeManual;
}

/**
 * For SignalStrength
 */
void nsRilResponseResult::updateSignalStrength(nsSignalStrength* aSignalStrength) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateSignalStrength");
  mSignalStrength = aSignalStrength;
}

/**
 * For GetSmscAddress
 */
void nsRilResponseResult::updateSmscAddress(const nsAString &aSmsc) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateSmscAddress");
  mSmsc = aSmsc;
}

/**
 * For getCurrentCallsResponse
 */
void nsRilResponseResult::updateCurrentCalls(nsTArray<RefPtr<nsCall>> & aCalls) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateCurrentCalls");
  mCalls = aCalls.Clone();
}

/**
 * For getLastCallsFailCause
 */
void nsRilResponseResult::updateFailCause(int32_t aCauseCode, const nsAString &aVendorCause) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateFailCause");
  mCauseCode = aCauseCode;
  mVendorCause = aVendorCause;
}

/**
 * For getPreferredNetworkType
 */
void nsRilResponseResult::updatePreferredNetworkType(int32_t aType) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateType");
  mType = aType;
}

/**
 * For getAvailableNetwork
 */
void nsRilResponseResult::updateAvailableNetworks(nsTArray<RefPtr<nsOperatorInfo>> & aAvailableNetworks) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateAvailableNetworks");
  mAvailableNetworks = aAvailableNetworks.Clone();
}

/**
 * For setupDataCAll
 */
void nsRilResponseResult::updateDataCallResponse(nsSetupDataCallResult* aDcResponse) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateDataCallResponse");
  mDcResponse = aDcResponse;
}

/**
 * For getDataCallList
 */
void nsRilResponseResult::updateDcList(nsTArray<RefPtr<nsSetupDataCallResult>> & aDcLists) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateDcList");
  mDcLists = aDcLists.Clone();
}

/**
 * For getCellInfoList
 */
void nsRilResponseResult::updateCellInfoList(nsTArray<RefPtr<nsRilCellInfo>> & aCellInfoLists) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateCellInfoList");
  mCellInfoLists = aCellInfoLists.Clone();
}

/**
 * For getIMSI
 */
 void nsRilResponseResult::updateIMSI(const nsAString &aIMSI) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateIMSI");
  mIMSI = aIMSI;
}

/**
 * For IccIOForApp
 */
void nsRilResponseResult::updateIccIoResult(nsIccIoResult* aIccIoResult) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateIccIoResult");
  mIccIoResult = aIccIoResult;
}

/**
 * For getClir */
 void nsRilResponseResult::updateClir(int32_t aN, int32_t aM) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateClir");
  mCLIR_N = aN;
  mCLIR_M = aM;
}

/**
 * For getCallForwardStatus
 */

void nsRilResponseResult::updateCallForwardStatusList(nsTArray<RefPtr<nsCallForwardInfo>> & aCallForwardInfoLists) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateCallForwardStatusList");
  mCallForwardInfoLists = aCallForwardInfoLists.Clone();
}

/**
 * For getCallWaiting */
 void nsRilResponseResult::updateCallWaiting(bool aEnable, int32_t aServiceClass) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateCallWaiting");
  mCWEnable = aEnable;
  mCWServiceClass = aServiceClass;
}

/**
 * For getClip */
 void nsRilResponseResult::updateClip(int32_t aProvisioned) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateClip");
  mProvisioned = aProvisioned;
}

/**
 * For getNeighboringCellIds
 */

void nsRilResponseResult::updateNeighboringCells(nsTArray<RefPtr<nsNeighboringCell>> & aNeighboringCell) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateNeighboringCells");
  mNeighboringCell = aNeighboringCell.Clone();
}

/**
 * For queryTtyMode */
 void nsRilResponseResult::updateTtyMode(int32_t aTtyMode) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateTtyMode");
  mTtyMode = aTtyMode;
}

/**
 * For getMute */
 void nsRilResponseResult::updateMute(bool aMuteEnabled) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateMute");
  mMuteEnabled = aMuteEnabled;
}

/**
 * For Icc pin/pul */
 void nsRilResponseResult::updateRemainRetries(int32_t aRemainingRetries) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResponseResult", "updateRemainRetries");
  mRemainingRetries = aRemainingRetries;
}

/**
 *
 */
nsRilResponseResult::~nsRilResponseResult()
{
}

NS_IMETHODIMP nsRilResponseResult::GetRilMessageType(nsAString& aRilMessageType) {
  aRilMessageType = mRilMessageType;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetRilMessageToken(int32_t *aRilMessageToken) {
  *aRilMessageToken = mRilMessageToken;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetErrorMsg(int32_t *aErrorMsg) {
  *aErrorMsg = mErrorMsg;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetImei(nsAString& aImei) {
  aImei = mImei;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetImeisv(nsAString& aImeisv) {
  aImeisv = mImeisv;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetEsn(nsAString& aEsn) {
  aEsn = mEsn;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetMeid(nsAString& aMeid) {
  aMeid = mMeid;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetRadioTech(int32_t *aRadioTech) {
  *aRadioTech = mRadioTech;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetBasebandVersion(nsAString& aBasebandVersion) {
  aBasebandVersion = mBasebandVersion;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetCardStatus(nsICardStatus **aCardStatus) {
  RefPtr<nsICardStatus> cardStatus(mCardStatus);
  cardStatus.forget(aCardStatus);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetVoiceRegStatus(nsIVoiceRegState **aVoiceRegState) {
  RefPtr<nsIVoiceRegState> voiceRegState(mVoiceRegState);
  voiceRegState.forget(aVoiceRegState);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetDataRegStatus(nsIDataRegState **aDataRegState) {
  RefPtr<nsIDataRegState> dataRegState(mDataRegState);
  dataRegState.forget(aDataRegState);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetOperator(nsIOperatorInfo **aOperatorInfo) {
  RefPtr<nsIOperatorInfo> operatorInfo(mOperatorInfo);
  operatorInfo.forget(aOperatorInfo);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetNwModeManual(bool* aNwModeManual)  {
  *aNwModeManual = mNwModeManual;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetSignalStrength(nsISignalStrength **aSignalStrength) {
  RefPtr<nsISignalStrength> signalStrength(mSignalStrength);
  signalStrength.forget(aSignalStrength);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetSmsc(nsAString& aSmsc) {
  aSmsc = mSmsc;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetCurrentCalls(uint32_t *count, nsICall ***calls) {
  *count = mCalls.Length();
  nsICall **call = (nsICall**)moz_xmalloc(*count * sizeof(nsICall *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(call[i] = mCalls[i]);
  }

  *calls = call;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetCauseCode(int32_t *aCauseCode) {
  *aCauseCode = mCauseCode;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetVendorCause(nsAString& aVendorCause) {
  aVendorCause = mVendorCause;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetType(int32_t *aType) {
  *aType = mType;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetAvailableNetworks(uint32_t *count, nsIOperatorInfo ***networks) {
  *count = mAvailableNetworks.Length();
  nsIOperatorInfo **network = (nsIOperatorInfo**)moz_xmalloc(*count * sizeof(nsIOperatorInfo *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(network[i] = mAvailableNetworks[i]);
  }

  *networks = network;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetDcResponse(nsISetupDataCallResult **aDcResponse) {
  RefPtr<nsISetupDataCallResult> dcResponse(mDcResponse);
  dcResponse.forget(aDcResponse);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetDataCallLists(uint32_t *count, nsISetupDataCallResult ***datacalls) {
  *count = mDcLists.Length();
  nsISetupDataCallResult **datacall = (nsISetupDataCallResult**)moz_xmalloc(*count * sizeof(nsISetupDataCallResult *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(datacall[i] = mDcLists[i]);
  }

  *datacalls = datacall;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetCellInfoList(uint32_t *count, nsIRilCellInfo ***cellInfos) {
  *count = mCellInfoLists.Length();
  nsIRilCellInfo **cellInfo = (nsIRilCellInfo**)moz_xmalloc(*count * sizeof(nsIRilCellInfo *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(cellInfo[i] = mCellInfoLists[i]);
  }

  *cellInfos = cellInfo;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetImsi(nsAString& aIMSI) {
  aIMSI = mIMSI;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetIccIo(nsIIccIoResult **aIccIoResult) {
  RefPtr<nsIIccIoResult> iccIoResult(mIccIoResult);
  iccIoResult.forget(aIccIoResult);

  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetN(int32_t *aN)  {
  *aN = mCLIR_N;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetM(int32_t *aM) {
  *aM = mCLIR_M;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetCallForwardStatus(uint32_t *count, nsICallForwardInfo ***callForwardInfos) {
  *count = mCallForwardInfoLists.Length();
  nsICallForwardInfo **callForwardInfo = (nsICallForwardInfo**)moz_xmalloc(*count * sizeof(nsICallForwardInfo *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(callForwardInfo[i] = mCallForwardInfoLists[i]);
  }

  *callForwardInfos = callForwardInfo;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetEnable(bool* aEnable)  {
  *aEnable = mCWEnable;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetServiceClass(int32_t *aServiceClass) {
  *aServiceClass = mCWServiceClass;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetProvisioned(int32_t *aProvisioned) {
  *aProvisioned = mProvisioned;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetNeighboringCids(uint32_t *count, nsINeighboringCell ***cells) {
  *count = mNeighboringCell.Length();
  nsINeighboringCell **cell = (nsINeighboringCell**)moz_xmalloc(*count * sizeof(nsINeighboringCell *));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(cell[i] = mNeighboringCell[i]);
  }

  *cells = cell;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetTtyMode(int32_t *aTtyMode) {
  *aTtyMode = mTtyMode;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetMuteEnable(bool *aMuteEnable)  {
  *aMuteEnable = mMuteEnabled;
  return NS_OK;
}

NS_IMETHODIMP nsRilResponseResult::GetRemainingRetries(int32_t *aRemainingRetries) {
  *aRemainingRetries = mRemainingRetries;
  return NS_OK;
}


NS_DEFINE_NAMED_CID(RILRESPONSERESULT_CID);
