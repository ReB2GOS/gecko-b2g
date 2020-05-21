/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsRilIndicationResult.h"

/* Logging related */
#undef LOG_TAG
#define LOG_TAG "nsRilIndicationResult"

#undef INFO
#undef ERROR
#undef DEBUG
#define INFO(args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##args)
#define ERROR(args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##args)
#define DEBUG(args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, ##args)

#define RILINDICATIONRESULT_CID                      \
  {                                                  \
    0x02c03b38, 0xbaf2, 0x4df3, {                    \
      0x88, 0xc8, 0x3a, 0xc7, 0xd2, 0x63, 0x7b, 0x8d \
    }                                                \
  }

/*============================================================================
 *======================Implementation of Class nsRilIndicationResult
 *=====================
 *============================================================================*/
NS_IMPL_ISUPPORTS(nsRilIndicationResult, nsIRilIndicationResult)

/**
 * Constructor for a nsRilIndicationResult
 * For those has no parameter notify.
 */
nsRilIndicationResult::nsRilIndicationResult(const nsAString& aRilMessageType)
    : nsRilResult(aRilMessageType) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "init nsRilIndicationResult");
}

/**
 * Constructor for a nsRilIndicationResult
 * For radioStateChanged
 */
void nsRilIndicationResult::updateRadioStateChanged(int32_t aRadioState) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateRadioStateChanged");
  mRadioState = aRadioState;
}

/**
 * Constructor for a nsRilIndicationResult
 * For newSmsOnSim
 */
void nsRilIndicationResult::updateNewSmsOnSim(int32_t aRecordNumber) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateNewSmsOnSim");
  mRecordNumber = aRecordNumber;
}

/**
 * Constructor for a nsRilIndicationResult
 * For onUssd
 */
void nsRilIndicationResult::updateOnUssd(int32_t aTypeCode,
                                         const nsAString& aMessage) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateOnUssd");
  mTypeCode = aTypeCode;
  mMessage = aMessage;
}

/**
 * Constructor for a nsRilIndicationResult
 * For nitzTimeReceived
 */
void nsRilIndicationResult::updateNitzTimeReceived(const nsAString& aDateString,
                                                   int64_t aReceiveTimeInMS) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateNitzTimeReceived");
  mDateString = aDateString;
  mReceiveTimeInMS = aReceiveTimeInMS;
}

/**
 * Constructor for a nsRilIndicationResult
 * For currentSignalStrength
 */
void nsRilIndicationResult::updateCurrentSignalStrength(
    nsSignalStrength* aSignalStrength) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateCurrentSignalStrength");
  mSignalStrength = aSignalStrength;
}

void nsRilIndicationResult::updateDataCallListChanged(
    nsTArray<RefPtr<nsSetupDataCallResult>>& aDatacalls) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateDataCallListChanged");
  mDatacalls = aDatacalls.Clone();
}

/**
 * Constructor for a nsRilIndicationResult
 * For suppSvcNotify
 */
void nsRilIndicationResult::updateSuppSvcNotify(
    nsSuppSvcNotification* aSuppSvc) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateSuppSvcNotify");
  mSuppSvc = aSuppSvc;
}

/**
 * Constructor for a nsRilIndicationResult
 * For stkProactiveCommand
 */
void nsRilIndicationResult::updateStkProactiveCommand(const nsAString& aCmd) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateStkProactiveCommand");
  mCmd = aCmd;
}

/**
 * Constructor for a nsRilIndicationResult
 * For stkEventNotify
 */
void nsRilIndicationResult::updateStkEventNotify(const nsAString& aCmd) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateStkEventNotify");
  mCmd = aCmd;
}

/**
 * Constructor for a nsRilIndicationResult
 * For stkCallSetup
 */
void nsRilIndicationResult::updateStkCallSetup(int32_t aTimeout) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateStkCallSetup");
  mTimeout = aTimeout;
}

/**
 * Constructor for a nsRilIndicationResult
 * For simRefresh
 */
void nsRilIndicationResult::updateSimRefresh(
    nsSimRefreshResult* aRefreshResult) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateSimRefresh");
  mRefreshResult = aRefreshResult;
}

/**
 * Constructor for a nsRilIndicationResult
 * For callRing
 */
void nsRilIndicationResult::updateCallRing(bool aIsGsm) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateCallRing");
  mIsGsm = aIsGsm;
}

/**
 * Constructor for a nsRilIndicationResult
 * For newBroadcastSms
 */
void nsRilIndicationResult::updateNewBroadcastSms(nsTArray<int32_t>& aData) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateNewBroadcastSms");
  mData = aData.Clone();
}

/**
 * Constructor for a nsRilIndicationResult
 * For restrictedStateChanged
 */
void nsRilIndicationResult::updateRestrictedStateChanged(
    int32_t aRestrictedState) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateRestrictedStateChanged");
  mRestrictedState = aRestrictedState;
}

/**
 * Constructor for a nsRilIndicationResult
 * For indicateRingbackTone
 */
void nsRilIndicationResult::updateIndicateRingbackTone(bool aPlayRingbackTone) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateIndicateRingbackTone");
  mPlayRingbackTone = aPlayRingbackTone;
}

/**
 * Constructor for a nsRilIndicationResult
 * For voiceRadioTechChanged
 */
void nsRilIndicationResult::updateVoiceRadioTechChanged(int32_t aRadioTech) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateVoiceRadioTechChanged");
  mRadioTech = aRadioTech;
}

void nsRilIndicationResult::updateCellInfoList(
    nsTArray<RefPtr<nsRilCellInfo>>& aRecords) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateCellInfoList");
  mRecords = aRecords.Clone();
}

/**
 * Constructor for a nsRilIndicationResult
 * For subscriptionStatusChanged
 */
void nsRilIndicationResult::updateSubscriptionStatusChanged(bool aActivate) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateSubscriptionStatusChanged");
  mActivate = aActivate;
}

/**
 * Constructor for a nsRilIndicationResult
 * For srvccStateNotify
 */
void nsRilIndicationResult::updateSrvccStateNotify(int32_t aSrvccState) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateSrvccStateNotify");
  mSrvccState = aSrvccState;
}

void nsRilIndicationResult::updateHardwareConfigChanged(
    nsTArray<RefPtr<nsHardwareConfig>>& aConfigs) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateHardwareConfigChanged");
  mConfigs = aConfigs.Clone();
}

/**
 * Constructor for a nsRilIndicationResult
 * For radioCapabilityIndication
 */
void nsRilIndicationResult::updateRadioCapabilityIndication(
    nsRadioCapability* aRc) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateRadioCapabilityIndication");
  mRc = aRc;
}

/**
 * Constructor for a nsRilIndicationResult
 * For stkCallControlAlphaNotify
 */
void nsRilIndicationResult::updateStkCallControlAlphaNotify(
    const nsAString& aAlpha) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateStkCallControlAlphaNotify");
  mAlpha = aAlpha;
}

/**
 * Constructor for a nsRilIndicationResult
 * For lceData
 */
void nsRilIndicationResult::updateLceData(nsILceDataInfo* aLce) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateLceData");
  mLce = aLce;
}

/**
 * Constructor for a nsRilIndicationResult
 * For pcoData
 */
void nsRilIndicationResult::updatePcoData(nsIPcoDataInfo* aPco) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updatePcoDatat");
  mPco = aPco;
}

/**
 * Constructor for a nsRilIndicationResult
 * For modemReset
 */
void nsRilIndicationResult::updateModemReset(const nsAString& aReason) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilIndicationResult",
                      "updateModemReset");
  mReason = aReason;
}

/**
 *
 */
nsRilIndicationResult::~nsRilIndicationResult() {
  // QLOGD("mDatacall = aDatacall; for %x, type = %d", (int)(void *)this,
  // mNetworkType);
}

NS_IMETHODIMP nsRilIndicationResult::GetRilMessageType(
    nsAString& aRilMessageType) {
  aRilMessageType = mRilMessageType;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRadioState(int32_t* aRadioState) {
  *aRadioState = mRadioState;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRecordNumber(int32_t* aRecordNumber) {
  *aRecordNumber = mRecordNumber;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetTypeCode(int32_t* aTypeCode) {
  *aTypeCode = mTypeCode;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetMessage(nsAString& aMessage) {
  aMessage = mMessage;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetDateString(nsAString& aDateString) {
  aDateString = mDateString;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetReceiveTimeInMS(
    int64_t* aReceiveTimeInMS) {
  *aReceiveTimeInMS = mReceiveTimeInMS;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetSignalStrength(
    nsISignalStrength** aSignalStrength) {
  RefPtr<nsISignalStrength> signalStrength(mSignalStrength);
  signalStrength.forget(aSignalStrength);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetDataCallLists(
    uint32_t* count, nsISetupDataCallResult*** datacalls) {
  *count = mDatacalls.Length();
  nsISetupDataCallResult** datacall = (nsISetupDataCallResult**)moz_xmalloc(
      *count * sizeof(nsISetupDataCallResult*));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(datacall[i] = mDatacalls[i]);
  }

  *datacalls = datacall;

  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetSuppSvc(
    nsISuppSvcNotification** aSuppSvc) {
  RefPtr<nsISuppSvcNotification> suppSvc(mSuppSvc);
  suppSvc.forget(aSuppSvc);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetCmd(nsAString& aCmd) {
  aCmd = mCmd;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetTimeout(int32_t* aTimeout) {
  *aTimeout = mTimeout;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRefreshResult(
    nsISimRefreshResult** aRefreshResult) {
  RefPtr<nsISimRefreshResult> refreshResult(mRefreshResult);
  refreshResult.forget(aRefreshResult);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetIsGsm(bool* aIsGsm) {
  *aIsGsm = mIsGsm;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetNewBroadcastSms(int32_t** data,
                                                        uint32_t* count) {
  *count = mData.Length();
  *data = (int32_t*)moz_xmalloc((*count) * sizeof(int32_t));
  NS_ENSURE_TRUE(*data, NS_ERROR_OUT_OF_MEMORY);

  for (uint32_t i = 0; i < *count; i++) {
    (*data)[i] = mData[i];
  }
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRestrictedState(
    int32_t* aRestrictedState) {
  *aRestrictedState = mRestrictedState;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetPlayRingbackTone(
    bool* aPlayRingbackTone) {
  *aPlayRingbackTone = mPlayRingbackTone;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRadioTech(int32_t* aRadioTech) {
  *aRadioTech = mRadioTech;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetCellInfo(nsIRilCellInfo*** records,
                                                 uint32_t* count) {
  *count = mRecords.Length();
  nsIRilCellInfo** record =
      (nsIRilCellInfo**)moz_xmalloc(*count * sizeof(nsIRilCellInfo*));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(record[i] = mRecords[i]);
  }

  *records = record;

  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetActivate(bool* aActivate) {
  *aActivate = mActivate;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetSrvccState(int32_t* aSrvccState) {
  *aSrvccState = mSrvccState;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetHardwardConfig(
    nsIHardwareConfig*** configs, uint32_t* count) {
  *count = mConfigs.Length();
  nsIHardwareConfig** config =
      (nsIHardwareConfig**)moz_xmalloc(*count * sizeof(nsIHardwareConfig*));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(config[i] = mConfigs[i]);
  }

  *configs = config;

  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetRc(nsIRadioCapability** aRc) {
  RefPtr<nsIRadioCapability> rc(mRc);
  rc.forget(aRc);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetAlpha(nsAString& aAlpha) {
  aAlpha = mAlpha;
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetLce(nsILceDataInfo** aLce) {
  RefPtr<nsILceDataInfo> lce(mLce);
  lce.forget(aLce);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetPco(nsIPcoDataInfo** aPco) {
  RefPtr<nsIPcoDataInfo> pco(mPco);
  pco.forget(aPco);
  return NS_OK;
}

NS_IMETHODIMP nsRilIndicationResult::GetReason(nsAString& aReason) {
  aReason = mReason;
  return NS_OK;
}

NS_DEFINE_NAMED_CID(RILINDICATIONRESULT_CID);
