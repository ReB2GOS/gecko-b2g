/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "nsRilResult.h"

/* Logging related */
#undef LOG_TAG
#define LOG_TAG "nsRilResult"

#undef INFO
#undef ERROR
#undef DEBUG
#define INFO(args...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, ##args)
#define ERROR(args...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, ##args)
#define DEBUG(args...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, ##args)

#define RILRESULT_CID                                \
  {                                                  \
    0x0a8ace7c, 0xa92a, 0x4c52, {                    \
      0x91, 0x83, 0xf4, 0x39, 0xc7, 0xc1, 0xea, 0xe9 \
    }                                                \
  }

/*============================================================================
 *============ Implementation of Class nsGsmSignalStrength ===================
 *============================================================================*/
/**
 * nsGsmSignalStrength implementation
 */
nsGsmSignalStrength::nsGsmSignalStrength(int32_t aSignalStrength,
                                         int32_t aBitErrorRate,
                                         int32_t aTimingAdvance)
    : mSignalStrength(aSignalStrength),
      mBitErrorRate(aBitErrorRate),
      mTimingAdvance(aTimingAdvance) {
  __android_log_print(ANDROID_LOG_INFO, " nsGsmSignalStrength",
                      "init nsGsmSignalStrength");
}

NS_IMETHODIMP nsGsmSignalStrength::GetSignalStrength(int32_t* aSignalStrength) {
  *aSignalStrength = mSignalStrength;
  return NS_OK;
}
NS_IMETHODIMP nsGsmSignalStrength::GetBitErrorRate(int32_t* aBitErrorRate) {
  *aBitErrorRate = mBitErrorRate;
  return NS_OK;
}
NS_IMETHODIMP nsGsmSignalStrength::GetTimingAdvance(int32_t* aTimingAdvance) {
  *aTimingAdvance = mTimingAdvance;
  return NS_OK;
}
NS_IMPL_ISUPPORTS(nsGsmSignalStrength, nsIGsmSignalStrength)

/*============================================================================
 *============ Implementation of Class nsWcdmaSignalStrength ===================
 *============================================================================*/
/**
 * nsWcdmaSignalStrength implementation
 */
nsWcdmaSignalStrength::nsWcdmaSignalStrength(int32_t aSignalStrength,
                                             int32_t aBitErrorRate)
    : mSignalStrength(aSignalStrength), mBitErrorRate(aBitErrorRate) {
  __android_log_print(ANDROID_LOG_INFO, " nsWcdmaSignalStrength",
                      "init nsWcdmaSignalStrength");
}

NS_IMETHODIMP nsWcdmaSignalStrength::GetSignalStrength(
    int32_t* aSignalStrength) {
  *aSignalStrength = mSignalStrength;
  return NS_OK;
}
NS_IMETHODIMP nsWcdmaSignalStrength::GetBitErrorRate(int32_t* aBitErrorRate) {
  *aBitErrorRate = mBitErrorRate;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsWcdmaSignalStrength, nsIWcdmaSignalStrength)

/*============================================================================
 *============ Implementation of Class nsCdmaSignalStrength ===================
 *============================================================================*/
/**
 * nsCdmaSignalStrength implementation
 */
nsCdmaSignalStrength::nsCdmaSignalStrength(int32_t aDbm, int32_t aEcio)
    : mDbm(aDbm), mEcio(aEcio) {
  __android_log_print(ANDROID_LOG_INFO, " nsCdmaSignalStrength",
                      "init nsCdmaSignalStrength");
}

NS_IMETHODIMP nsCdmaSignalStrength::GetDbm(int32_t* aDbm) {
  *aDbm = mDbm;
  return NS_OK;
}
NS_IMETHODIMP nsCdmaSignalStrength::GetEcio(int32_t* aEcio) {
  *aEcio = mEcio;
  return NS_OK;
}
NS_IMPL_ISUPPORTS(nsCdmaSignalStrength, nsICdmaSignalStrength)

/*============================================================================
 *============ Implementation of Class nsEvdoSignalStrength ===================
 *============================================================================*/
/**
 * nsEvdoSignalStrength implementation
 */
nsEvdoSignalStrength::nsEvdoSignalStrength(int32_t aDbm, int32_t aEcio,
                                           int32_t aSignalNoiseRatio)
    : mDbm(aDbm), mEcio(aEcio), mSignalNoiseRatio(aSignalNoiseRatio) {
  __android_log_print(ANDROID_LOG_INFO, " nsEvdoSignalStrength",
                      "init nsEvdoSignalStrength");
}

NS_IMETHODIMP nsEvdoSignalStrength::GetDbm(int32_t* aDbm) {
  *aDbm = mDbm;
  return NS_OK;
}
NS_IMETHODIMP nsEvdoSignalStrength::GetEcio(int32_t* aEcio) {
  *aEcio = mEcio;
  return NS_OK;
}
NS_IMETHODIMP nsEvdoSignalStrength::GetSignalNoiseRatio(
    int32_t* aSignalNoiseRatio) {
  *aSignalNoiseRatio = mSignalNoiseRatio;
  return NS_OK;
}
NS_IMPL_ISUPPORTS(nsEvdoSignalStrength, nsIEvdoSignalStrength)

/*============================================================================
 *============ Implementation of Class nsLteSignalStrength ===================
 *============================================================================*/
/**
 * nsLteSignalStrength implementation
 */
nsLteSignalStrength::nsLteSignalStrength(int32_t aSignalStrength, int32_t aRsrp,
                                         int32_t aRsrq, int32_t aRssnr,
                                         int32_t aCqi, int32_t aTimingAdvance)
    : mSignalStrength(aSignalStrength),
      mRsrp(aRsrp),
      mRsrq(aRsrq),
      mRssnr(aRssnr),
      mCqi(aCqi),
      mTimingAdvance(aTimingAdvance) {
  __android_log_print(ANDROID_LOG_INFO, " nsLteSignalStrength",
                      "init nsLteSignalStrength");
}

NS_IMETHODIMP nsLteSignalStrength::GetSignalStrength(int32_t* aSignalStrength) {
  *aSignalStrength = mSignalStrength;
  return NS_OK;
}
NS_IMETHODIMP nsLteSignalStrength::GetRsrp(int32_t* aRsrp) {
  *aRsrp = mRsrp;
  return NS_OK;
}
NS_IMETHODIMP nsLteSignalStrength::GetRsrq(int32_t* aRsrq) {
  *aRsrq = mRsrq;
  return NS_OK;
}
NS_IMETHODIMP nsLteSignalStrength::GetRssnr(int32_t* aRssnr) {
  *aRssnr = mRssnr;
  return NS_OK;
}
NS_IMETHODIMP nsLteSignalStrength::GetCqi(int32_t* aCqi) {
  *aCqi = mCqi;
  return NS_OK;
}
NS_IMETHODIMP nsLteSignalStrength::GetTimingAdvance(int32_t* aTimingAdvance) {
  *aTimingAdvance = mTimingAdvance;
  return NS_OK;
}
NS_IMPL_ISUPPORTS(nsLteSignalStrength, nsILteSignalStrength)

/*============================================================================
 *============ Implementation of Class nsTdScdmaSignalStrength
 *===================
 *============================================================================*/
/**
 * nsTdScdmaSignalStrength implementation
 */
nsTdScdmaSignalStrength::nsTdScdmaSignalStrength(int32_t aRscp) : mRscp(aRscp) {
  __android_log_print(ANDROID_LOG_INFO, " nsLteSignalStrength",
                      "init nsLteSignalStrength");
}

NS_IMETHODIMP nsTdScdmaSignalStrength::GetRscp(int32_t* aRscp) {
  *aRscp = mRscp;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsTdScdmaSignalStrength, nsITdScdmaSignalStrength)

/*============================================================================
 *============ Implementation of Class nsSignalStrength ===================
 *============================================================================*/
/**
 * nsSignalStrength implementation
 */
nsSignalStrength::nsSignalStrength(
    nsGsmSignalStrength* aGsmSignalStrength,
    nsCdmaSignalStrength* aCdmaSignalStrength,
    nsEvdoSignalStrength* aEvdoSignalStrength,
    nsLteSignalStrength* aLteSignalStrength,
    nsTdScdmaSignalStrength* aTdScdmaSignalStrength)
    : mGsmSignalStrength(aGsmSignalStrength),
      mCdmaSignalStrength(aCdmaSignalStrength),
      mEvdoSignalStrength(aEvdoSignalStrength),
      mLteSignalStrength(aLteSignalStrength),
      mTdScdmaSignalStrength(aTdScdmaSignalStrength) {
  __android_log_print(ANDROID_LOG_INFO, " nsSignalStrength",
                      "init nsSignalStrength");
}

NS_IMETHODIMP nsSignalStrength::GetGsmSignalStrength(
    nsIGsmSignalStrength** aGsmSignalStrength) {
  RefPtr<nsIGsmSignalStrength> gsmSignalStrength(mGsmSignalStrength);
  gsmSignalStrength.forget(aGsmSignalStrength);
  return NS_OK;
}

NS_IMETHODIMP nsSignalStrength::GetCdmaSignalStrength(
    nsICdmaSignalStrength** aCdmaSignalStrength) {
  RefPtr<nsICdmaSignalStrength> cdmaSignalStrength(mCdmaSignalStrength);
  cdmaSignalStrength.forget(aCdmaSignalStrength);
  return NS_OK;
}

NS_IMETHODIMP nsSignalStrength::GetEvdoSignalStrength(
    nsIEvdoSignalStrength** aEvdoSignalStrength) {
  RefPtr<nsIEvdoSignalStrength> evdoSignalStrength(mEvdoSignalStrength);
  evdoSignalStrength.forget(aEvdoSignalStrength);
  return NS_OK;
}

NS_IMETHODIMP nsSignalStrength::GetLteSignalStrength(
    nsILteSignalStrength** aLteSignalStrength) {
  RefPtr<nsILteSignalStrength> lteSignalStrength(mLteSignalStrength);
  lteSignalStrength.forget(aLteSignalStrength);
  return NS_OK;
}

NS_IMETHODIMP nsSignalStrength::GetTdscdmaSignalStrength(
    nsITdScdmaSignalStrength** aTdscdmaSignalStrength) {
  RefPtr<nsITdScdmaSignalStrength> tdscdmaSignalStrength(
      mTdScdmaSignalStrength);
  tdscdmaSignalStrength.forget(aTdscdmaSignalStrength);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsSignalStrength, nsISignalStrength)

/*============================================================================
 *============ Implementation of Class nsSetupDataCallResult ===================
 *============================================================================*/
/**
 * nsSetupDataCallResult implementation
 */
nsSetupDataCallResult::nsSetupDataCallResult(
    int32_t aFailCause, int32_t aSuggestedRetryTime, int32_t aCid,
    int32_t aActive, const nsAString& aPdpType, const nsAString& aIfname,
    const nsAString& aAddresses, const nsAString& aDnses,
    const nsAString& aGateways, const nsAString& aPcscf, int32_t aMtu)
    : mFailCause(aFailCause),
      mSuggestedRetryTime(aSuggestedRetryTime),
      mCid(aCid),
      mActive(aActive),
      mPdpType(aPdpType),
      mIfname(aIfname),
      mAddresses(aAddresses),
      mDnses(aDnses),
      mGateways(aGateways),
      mPcscf(aPcscf),
      mMtu(aMtu) {
  __android_log_print(ANDROID_LOG_INFO, " nsSetupDataCallResult",
                      "init nsSetupDataCallResult");
}

NS_IMETHODIMP nsSetupDataCallResult::GetFailCause(int32_t* aFailCause) {
  *aFailCause = mFailCause;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetSuggestedRetryTime(
    int32_t* aSuggestedRetryTime) {
  *aSuggestedRetryTime = mSuggestedRetryTime;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetCid(int32_t* aCid) {
  *aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetActive(int32_t* aActive) {
  *aActive = mActive;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetPdpType(nsAString& aPdpType) {
  aPdpType = mPdpType;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetIfname(nsAString& aIfname) {
  aIfname = mIfname;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetAddresses(nsAString& aAddresses) {
  aAddresses = mAddresses;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetDnses(nsAString& aDnses) {
  aDnses = mDnses;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetGateways(nsAString& aGateways) {
  aGateways = mGateways;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetPcscf(nsAString& aPcscf) {
  aPcscf = mPcscf;
  return NS_OK;
}

NS_IMETHODIMP nsSetupDataCallResult::GetMtu(int32_t* aMtu) {
  *aMtu = mMtu;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsSetupDataCallResult, nsISetupDataCallResult)

/*============================================================================
 *============ Implementation of Class nsSuppSvcNotification ===================
 *============================================================================*/
/**
 * nsSuppSvcNotification implementation
 */
nsSuppSvcNotification::nsSuppSvcNotification(bool aNotificationType,
                                             int32_t aCode, int32_t aIndex,
                                             int32_t aType,
                                             const nsAString& aNumber)
    : mNotificationType(aNotificationType),
      mCode(aCode),
      mIndex(aIndex),
      mType(aType),
      mNumber(aNumber) {
  __android_log_print(ANDROID_LOG_INFO, " nsSuppSvcNotification",
                      "init nsSuppSvcNotification");
}

NS_IMETHODIMP nsSuppSvcNotification::GetNotificationType(
    bool* aNotificationType) {
  *aNotificationType = mNotificationType;
  return NS_OK;
}

NS_IMETHODIMP nsSuppSvcNotification::GetCode(int32_t* aCode) {
  *aCode = mCode;
  return NS_OK;
}

NS_IMETHODIMP nsSuppSvcNotification::GetIndex(int32_t* aIndex) {
  *aIndex = mIndex;
  return NS_OK;
}

NS_IMETHODIMP nsSuppSvcNotification::GetType(int32_t* aType) {
  *aType = mType;
  return NS_OK;
}

NS_IMETHODIMP nsSuppSvcNotification::GetNumber(nsAString& aNumber) {
  aNumber = mNumber;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsSuppSvcNotification, nsISuppSvcNotification)

/*============================================================================
 *============ Implementation of Class nsSimRefreshResult ===================
 *============================================================================*/
/**
 * nsSimRefreshResult implementation
 */
nsSimRefreshResult::nsSimRefreshResult(int32_t aType, int32_t aEfId,
                                       const nsAString& aAid)
    : mType(aType), mEfId(aEfId), mAid(aAid) {
  __android_log_print(ANDROID_LOG_INFO, " nsSimRefreshResult",
                      "init nsSimRefreshResult");
}

NS_IMETHODIMP nsSimRefreshResult::GetType(int32_t* aType) {
  *aType = mType;
  return NS_OK;
}

NS_IMETHODIMP nsSimRefreshResult::GetEfId(int32_t* aEfId) {
  *aEfId = mEfId;
  return NS_OK;
}

NS_IMETHODIMP nsSimRefreshResult::GetAid(nsAString& aAid) {
  aAid = mAid;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsSimRefreshResult, nsISimRefreshResult)

/*============================================================================
 *============ Implementation of Class nsCellIdentityGsm ===================
 *============================================================================*/
/**
 * nsCellIdentityGsm implementation
 */
nsCellIdentityGsm::nsCellIdentityGsm(const nsAString& aMcc,
                                     const nsAString& aMnc, int32_t aLac,
                                     int32_t aCid, int32_t aArfcn,
                                     int32_t aBsic)
    : mMcc(aMcc),
      mMnc(aMnc),
      mLac(aLac),
      mCid(aCid),
      mArfcn(aArfcn),
      mBsic(aBsic) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentityGsm",
                      "init nsCellIdentityGsm");
}

NS_IMETHODIMP nsCellIdentityGsm::GetMcc(nsAString& aMcc) {
  aMcc = mMcc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityGsm::GetMnc(nsAString& aMnc) {
  aMnc = mMnc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityGsm::GetLac(int32_t* aLac) {
  *aLac = mLac;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityGsm::GetCid(int32_t* aCid) {
  *aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityGsm::GetArfcn(int32_t* aArfcn) {
  *aArfcn = mArfcn;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityGsm::GetBsic(int32_t* aBsic) {
  *aBsic = mBsic;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentityGsm, nsICellIdentityGsm)

/*============================================================================
 *============ Implementation of Class nsCellIdentityCdma ===================
 *============================================================================*/
/**
 * nsCellIdentityCdma implementation
 */
nsCellIdentityCdma::nsCellIdentityCdma(int32_t aNetworkId, int32_t aSystemId,
                                       int32_t aBaseStationId,
                                       int32_t aLongitude, int32_t aLatitude)
    : mNetworkId(aNetworkId),
      mSystemId(aSystemId),
      mBaseStationId(aBaseStationId),
      mLongitude(aLongitude),
      mLatitude(aLatitude) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentityCdma",
                      "init nsCellIdentityCdma");
}

NS_IMETHODIMP nsCellIdentityCdma::GetNetworkId(int32_t* aNetworkId) {
  *aNetworkId = mNetworkId;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityCdma::GetSystemId(int32_t* aSystemId) {
  *aSystemId = mSystemId;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityCdma::GetBaseStationId(int32_t* aBaseStationId) {
  *aBaseStationId = mBaseStationId;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityCdma::GetLongitude(int32_t* aLongitude) {
  *aLongitude = mLongitude;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityCdma::GetLatitude(int32_t* aLatitude) {
  *aLatitude = mLatitude;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentityCdma, nsICellIdentityCdma)

/*============================================================================
 *============ Implementation of Class nsCellIdentityLte ===================
 *============================================================================*/
/**
 * nsCellIdentityLte implementation
 */
nsCellIdentityLte::nsCellIdentityLte(const nsAString& aMcc,
                                     const nsAString& aMnc, int32_t aCi,
                                     int32_t aPci, int32_t aTac,
                                     int32_t aEarfcn)
    : mMcc(aMcc),
      mMnc(aMnc),
      mCi(aCi),
      mPci(aPci),
      mTac(aTac),
      mEarfcn(aEarfcn) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentityLte",
                      "init nsCellIdentityLte");
}

NS_IMETHODIMP nsCellIdentityLte::GetMcc(nsAString& aMcc) {
  aMcc = mMcc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityLte::GetMnc(nsAString& aMnc) {
  aMnc = mMnc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityLte::GetCi(int32_t* aCi) {
  *aCi = mCi;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityLte::GetPci(int32_t* aPci) {
  *aPci = mPci;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityLte::GetTac(int32_t* aTac) {
  *aTac = mTac;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityLte::GetEarfcn(int32_t* aEarfcn) {
  *aEarfcn = mEarfcn;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentityLte, nsICellIdentityLte)

/*============================================================================
 *============ Implementation of Class nsCellIdentityWcdma ===================
 *============================================================================*/
/**
 * nsCellIdentityWcdma implementation
 */
nsCellIdentityWcdma::nsCellIdentityWcdma(const nsAString& aMcc,
                                         const nsAString& aMnc, int32_t aLac,
                                         int32_t aCid, int32_t aPsc,
                                         int32_t aUarfcn)
    : mMcc(aMcc),
      mMnc(aMnc),
      mLac(aLac),
      mCid(aCid),
      mPsc(aPsc),
      mUarfcn(aUarfcn) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentityWcdma",
                      "init nsCellIdentityWcdma");
}

NS_IMETHODIMP nsCellIdentityWcdma::GetMcc(nsAString& aMcc) {
  aMcc = mMcc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityWcdma::GetMnc(nsAString& aMnc) {
  aMnc = mMnc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityWcdma::GetLac(int32_t* aLac) {
  *aLac = mLac;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityWcdma::GetCid(int32_t* aCid) {
  *aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityWcdma::GetPsc(int32_t* aPsc) {
  *aPsc = mPsc;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityWcdma::GetUarfcn(int32_t* aUarfcn) {
  *aUarfcn = mUarfcn;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentityWcdma, nsICellIdentityWcdma)

/*============================================================================
 *============ Implementation of Class nsCellIdentityTdScdma ===================
 *============================================================================*/
/**
 * nsCellIdentityTdScdma implementation
 */
nsCellIdentityTdScdma::nsCellIdentityTdScdma(const nsAString& aMcc,
                                             const nsAString& aMnc,
                                             int32_t aLac, int32_t aCid,
                                             int32_t aCpid)
    : mMcc(aMcc), mMnc(aMnc), mLac(aLac), mCid(aCid), mCpid(aCpid) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentityTdScdma",
                      "init nsCellIdentityTdScdma");
}

NS_IMETHODIMP nsCellIdentityTdScdma::GetMcc(nsAString& aMcc) {
  aMcc = mMcc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityTdScdma::GetMnc(nsAString& aMnc) {
  aMnc = mMnc;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityTdScdma::GetLac(int32_t* aLac) {
  *aLac = mLac;
  return NS_OK;
}
NS_IMETHODIMP nsCellIdentityTdScdma::GetCid(int32_t* aCid) {
  *aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentityTdScdma::GetCpid(int32_t* aCpid) {
  *aCpid = mCpid;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentityTdScdma, nsICellIdentityTdScdma)

/*============================================================================
 *============ Implementation of Class nsCellIdentity ===================
 *============================================================================*/
/**
 * nsCellIdentity implementation
 */
nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityGsm* aCellIdentityGsm,
                               nsCellIdentityWcdma* aCellIdentityWcdma,
                               nsCellIdentityCdma* aCellIdentityCdma,
                               nsCellIdentityLte* aCellIdentityLte,
                               nsCellIdentityTdScdma* aCellIdentityTdScdma)
    : mCellInfoType(aCellInfoType),
      mCellIdentityGsm(aCellIdentityGsm),
      mCellIdentityWcdma(aCellIdentityWcdma),
      mCellIdentityCdma(aCellIdentityCdma),
      mCellIdentityLte(aCellIdentityLte),
      mCellIdentityTdScdma(aCellIdentityTdScdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity");
}

nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityGsm* aCellIdentityGsm)
    : mCellInfoType(aCellInfoType), mCellIdentityGsm(aCellIdentityGsm) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity GSM");
}

nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityWcdma* aCellIdentityWcdma)
    : mCellInfoType(aCellInfoType), mCellIdentityWcdma(aCellIdentityWcdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity WCDMA");
}

nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityCdma* aCellIdentityCdma)
    : mCellInfoType(aCellInfoType), mCellIdentityCdma(aCellIdentityCdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity CDMA");
}

nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityLte* aCellIdentityLte)
    : mCellInfoType(aCellInfoType), mCellIdentityLte(aCellIdentityLte) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity LTE");
}

nsCellIdentity::nsCellIdentity(int32_t aCellInfoType,
                               nsCellIdentityTdScdma* aCellIdentityTdScdma)
    : mCellInfoType(aCellInfoType), mCellIdentityTdScdma(aCellIdentityTdScdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellIdentity",
                      "init nsCellIdentity TDCDMA");
}

NS_IMETHODIMP nsCellIdentity::GetCellInfoType(int32_t* aCellInfoType) {
  *aCellInfoType = mCellInfoType;
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentity::GetCellIdentityGsm(
    nsICellIdentityGsm** aCellIdentityGsm) {
  RefPtr<nsICellIdentityGsm> cellIdentityGsm(mCellIdentityGsm);
  cellIdentityGsm.forget(aCellIdentityGsm);
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentity::GetCellIdentityWcdma(
    nsICellIdentityWcdma** aCellIdentityWcdma) {
  RefPtr<nsICellIdentityWcdma> cellIdentityWcdma(mCellIdentityWcdma);
  cellIdentityWcdma.forget(aCellIdentityWcdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentity::GetCellIdentityCdma(
    nsICellIdentityCdma** aCellIdentityCdma) {
  RefPtr<nsICellIdentityCdma> cellIdentityCdma(mCellIdentityCdma);
  cellIdentityCdma.forget(aCellIdentityCdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentity::GetCellIdentityLte(
    nsICellIdentityLte** aCellIdentityLte) {
  RefPtr<nsICellIdentityLte> cellIdentityLte(mCellIdentityLte);
  cellIdentityLte.forget(aCellIdentityLte);
  return NS_OK;
}

NS_IMETHODIMP nsCellIdentity::GetCellIdentityTdScdma(
    nsICellIdentityTdScdma** aCellIdentityTdScdma) {
  RefPtr<nsICellIdentityTdScdma> cellIdentityTdScdma(mCellIdentityTdScdma);
  cellIdentityTdScdma.forget(aCellIdentityTdScdma);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellIdentity, nsICellIdentity)

/*============================================================================
 *============ Implementation of Class nsCellInfoGsm ===================
 *============================================================================*/
/**
 * nsCellInfoGsm implementation
 */
nsCellInfoGsm::nsCellInfoGsm(nsCellIdentityGsm* aCellIdentityGsm,
                             nsGsmSignalStrength* aSignalStrengthGsm)
    : mCellIdentityGsm(aCellIdentityGsm),
      mSignalStrengthGsm(aSignalStrengthGsm) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellInfoGsm", "init nsCellInfoGsm");
}

NS_IMETHODIMP nsCellInfoGsm::GetCellIdentityGsm(
    nsICellIdentityGsm** aCellIdentityGsm) {
  RefPtr<nsICellIdentityGsm> cellIdentityGsm(mCellIdentityGsm);
  cellIdentityGsm.forget(aCellIdentityGsm);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoGsm::GetSignalStrengthGsm(
    nsIGsmSignalStrength** aSignalStrengthGsm) {
  RefPtr<nsIGsmSignalStrength> signalStrengthGsm(mSignalStrengthGsm);
  signalStrengthGsm.forget(aSignalStrengthGsm);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellInfoGsm, nsICellInfoGsm)

/*============================================================================
 *============ Implementation of Class nsCellInfoCdma ===================
 *============================================================================*/
/**
 * nsCellInfoCdma implementation
 */
nsCellInfoCdma::nsCellInfoCdma(nsCellIdentityCdma* aCellIdentityCdma,
                               nsCdmaSignalStrength* aSignalStrengthCdma,
                               nsEvdoSignalStrength* aSignalStrengthEvdo)
    : mCellIdentityCdma(aCellIdentityCdma),
      mSignalStrengthCdma(aSignalStrengthCdma),
      mSignalStrengthEvdo(aSignalStrengthEvdo) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellInfoCdma",
                      "init nsCellInfoCdma");
}

NS_IMETHODIMP nsCellInfoCdma::GetCellIdentityCdma(
    nsICellIdentityCdma** aCellIdentityCdma) {
  RefPtr<nsICellIdentityCdma> cellIdentityCdma(mCellIdentityCdma);
  cellIdentityCdma.forget(aCellIdentityCdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoCdma::GetSignalStrengthCdma(
    nsICdmaSignalStrength** aSignalStrengthCdma) {
  RefPtr<nsICdmaSignalStrength> signalStrengthCdma(mSignalStrengthCdma);
  signalStrengthCdma.forget(aSignalStrengthCdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoCdma::GetSignalStrengthEvdo(
    nsIEvdoSignalStrength** aSignalStrengthEvdo) {
  RefPtr<nsIEvdoSignalStrength> signalStrengthEvdo(mSignalStrengthEvdo);
  signalStrengthEvdo.forget(aSignalStrengthEvdo);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellInfoCdma, nsICellInfoCdma)

/*============================================================================
 *============ Implementation of Class nsCellInfoLte ===================
 *============================================================================*/
/**
 * nsCellInfoLte implementation
 */
nsCellInfoLte::nsCellInfoLte(nsCellIdentityLte* aCellIdentityLte,
                             nsLteSignalStrength* aSignalStrengthLte)
    : mCellIdentityLte(aCellIdentityLte),
      mSignalStrengthLte(aSignalStrengthLte) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellInfoLte", "init nsCellInfoLte");
}

NS_IMETHODIMP nsCellInfoLte::GetCellIdentityLte(
    nsICellIdentityLte** aCellIdentityLte) {
  RefPtr<nsICellIdentityLte> cellIdentityLte(mCellIdentityLte);
  cellIdentityLte.forget(aCellIdentityLte);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoLte::GetSignalStrengthLte(
    nsILteSignalStrength** aSignalStrengthLte) {
  RefPtr<nsILteSignalStrength> signalStrengthLte(mSignalStrengthLte);
  signalStrengthLte.forget(aSignalStrengthLte);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellInfoLte, nsICellInfoLte)

/*============================================================================
 *============ Implementation of Class nsCellInfoWcdma ===================
 *============================================================================*/
/**
 * nsCellInfoWcdma implementation
 */
nsCellInfoWcdma::nsCellInfoWcdma(nsCellIdentityWcdma* aCellIdentityWcdma,
                                 nsWcdmaSignalStrength* aSignalStrengthWcdma)
    : mCellIdentityWcdma(aCellIdentityWcdma),
      mSignalStrengthWcdma(aSignalStrengthWcdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellInfoWcdma",
                      "init nsCellInfoWcdma");
}

NS_IMETHODIMP nsCellInfoWcdma::GetCellIdentityWcdma(
    nsICellIdentityWcdma** aCellIdentityWcdma) {
  RefPtr<nsICellIdentityWcdma> cellIdentityWcdma(mCellIdentityWcdma);
  cellIdentityWcdma.forget(aCellIdentityWcdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoWcdma::GetSignalStrengthWcdma(
    nsIWcdmaSignalStrength** aSignalStrengthWcdma) {
  RefPtr<nsIWcdmaSignalStrength> signalStrengthWcdma(mSignalStrengthWcdma);
  signalStrengthWcdma.forget(aSignalStrengthWcdma);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellInfoWcdma, nsICellInfoWcdma)

/*============================================================================
 *============ Implementation of Class nsCellInfoTdScdma ===================
 *============================================================================*/
/**
 * nsCellInfoTdScdma implementation
 */
nsCellInfoTdScdma::nsCellInfoTdScdma(
    nsCellIdentityTdScdma* aCellIdentityTdScdma,
    nsTdScdmaSignalStrength* aSignalStrengthTdScdma)
    : mCellIdentityTdScdma(aCellIdentityTdScdma),
      mSignalStrengthTdScdma(aSignalStrengthTdScdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsCellInfoTdScdma",
                      "init nsCellInfoTdScdma");
}

NS_IMETHODIMP nsCellInfoTdScdma::GetCellIdentityTdScdma(
    nsICellIdentityTdScdma** aCellIdentityTdScdma) {
  RefPtr<nsICellIdentityTdScdma> cellIdentityTdScdma(mCellIdentityTdScdma);
  cellIdentityTdScdma.forget(aCellIdentityTdScdma);
  return NS_OK;
}

NS_IMETHODIMP nsCellInfoTdScdma::GetSignalStrengthTdScdma(
    nsITdScdmaSignalStrength** aSignalStrengthTdScdma) {
  RefPtr<nsITdScdmaSignalStrength> signalStrengthTdScdma(
      mSignalStrengthTdScdma);
  signalStrengthTdScdma.forget(aSignalStrengthTdScdma);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCellInfoTdScdma, nsICellInfoTdScdma)

/*============================================================================
 *============ Implementation of Class nsRilCellInfo ===================
 *============================================================================*/
/**
 * nsRilCellInfo implementation
 */
nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoGsm* aGsm, nsCellInfoCdma* aCdma,
                             nsCellInfoLte* aLte, nsCellInfoWcdma* aWcdma,
                             nsCellInfoTdScdma* aTdScdma)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mGsm(aGsm),
      mCdma(aCdma),
      mLte(aLte),
      mWcdma(aWcdma),
      mTdScdma(aTdScdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo", "init nsRilCellInfo");
}

nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoGsm* aGsm)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mGsm(aGsm) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo",
                      "init nsRilCellInfo GSM");
}

nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoWcdma* aWcdma)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mWcdma(aWcdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo",
                      "init nsRilCellInfo WCDMA");
}

nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoCdma* aCdma)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mCdma(aCdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo",
                      "init nsRilCellInfo CDMA");
}

nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoLte* aLte)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mLte(aLte) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo",
                      "init nsRilCellInfo LTE");
}

nsRilCellInfo::nsRilCellInfo(int32_t aCellInfoType, bool aRegistered,
                             int32_t aTimeStampType, int32_t aTimeStamp,
                             nsCellInfoTdScdma* aTdScdma)
    : mCellInfoType(aCellInfoType),
      mRegistered(aRegistered),
      mTimeStampType(aTimeStampType),
      mTimeStamp(aTimeStamp),
      mTdScdma(aTdScdma) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilCellInfo",
                      "init nsRilCellInfo TDCDMA");
}

NS_IMETHODIMP nsRilCellInfo::GetCellInfoType(int32_t* aCellInfoType) {
  *aCellInfoType = mCellInfoType;
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetRegistered(bool* aRegistered) {
  *aRegistered = mRegistered;
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetTimeStampType(int32_t* aTimeStampType) {
  *aTimeStampType = mTimeStampType;
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetTimeStamp(int32_t* aTimeStamp) {
  *aTimeStamp = mTimeStamp;
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetGsm(nsICellInfoGsm** aGsm) {
  RefPtr<nsICellInfoGsm> cellInfoGsm(mGsm);
  cellInfoGsm.forget(aGsm);
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetCdma(nsICellInfoCdma** aCdma) {
  RefPtr<nsICellInfoCdma> cellInfoCdma(mCdma);
  cellInfoCdma.forget(aCdma);
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetLte(nsICellInfoLte** aLte) {
  RefPtr<nsICellInfoLte> cellInfoLte(mLte);
  cellInfoLte.forget(aLte);
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetWcdma(nsICellInfoWcdma** aWcdma) {
  RefPtr<nsICellInfoWcdma> cellInfoWcdma(mWcdma);
  cellInfoWcdma.forget(aWcdma);
  return NS_OK;
}

NS_IMETHODIMP nsRilCellInfo::GetTdscdma(nsICellInfoTdScdma** aTdScdma) {
  RefPtr<nsICellInfoTdScdma> cellInfoTdScdma(mTdScdma);
  cellInfoTdScdma.forget(aTdScdma);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsRilCellInfo, nsIRilCellInfo)

/*============================================================================
 *============ Implementation of Class nsHardwareConfig ===================
 *============================================================================*/
/**
 * nsHardwareConfig implementation
 */
nsHardwareConfig::nsHardwareConfig(int32_t aType, const nsAString& aUuid,
                                   int32_t aState,
                                   nsIHardwareConfigModem* aModem,
                                   nsIHardwareConfigSim* aSim)
    : mType(aType), mUuid(aUuid), mState(aState), mModem(aModem), mSim(aSim) {
  __android_log_print(ANDROID_LOG_INFO, " nsHardwareConfig",
                      "init nsHardwareConfig");
}

NS_IMETHODIMP nsHardwareConfig::GetType(int32_t* aType) {
  *aType = mType;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfig::GetUuid(nsAString& aUuid) {
  aUuid = mUuid;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfig::GetState(int32_t* aState) {
  *aState = mState;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfig::GetModem(nsIHardwareConfigModem** aModem) {
  RefPtr<nsIHardwareConfigModem> hwConfigModem(mModem);
  hwConfigModem.forget(aModem);
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfig::GetSim(nsIHardwareConfigSim** aSim) {
  RefPtr<nsIHardwareConfigSim> hwConfigSim(mSim);
  hwConfigSim.forget(aSim);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsHardwareConfig, nsIHardwareConfig)

/*============================================================================
 *============ Implementation of Class nsHardwareConfigModem ===================
 *============================================================================*/
/**
 * nsHardwareConfigModem implementation
 */
nsHardwareConfigModem::nsHardwareConfigModem(int32_t aRilModel, int32_t aRat,
                                             int32_t aMaxVoice,
                                             int32_t aMaxData,
                                             int32_t aMaxStandby)
    : mRilModel(aRilModel),
      mRat(aRat),
      mMaxVoice(aMaxVoice),
      mMaxData(aMaxData),
      mMaxStandby(aMaxStandby) {
  __android_log_print(ANDROID_LOG_INFO, " nsHardwareConfigModem",
                      "init nsHardwareConfigModem");
}

NS_IMETHODIMP nsHardwareConfigModem::GetRilModel(int32_t* aRilModel) {
  *aRilModel = mRilModel;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfigModem::GetRat(int32_t* aRat) {
  *aRat = mRat;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfigModem::GetMaxVoice(int32_t* aMaxVoice) {
  *aMaxVoice = mMaxVoice;
  return NS_OK;
}
NS_IMETHODIMP nsHardwareConfigModem::GetMaxData(int32_t* aMaxData) {
  *aMaxData = mMaxData;
  return NS_OK;
}

NS_IMETHODIMP nsHardwareConfigModem::GetMaxStandby(int32_t* aMaxStandby) {
  *aMaxStandby = mMaxStandby;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsHardwareConfigModem, nsIHardwareConfigModem)

/*============================================================================
 *============ Implementation of Class nsHardwareConfigSim ===================
 *============================================================================*/
/**
 * nsHardwareConfigSim implementation
 */
nsHardwareConfigSim::nsHardwareConfigSim(const nsAString& aModemUuid)
    : mModemUuid(aModemUuid) {
  __android_log_print(ANDROID_LOG_INFO, " nsHardwareConfigSim",
                      "init nsHardwareConfigSim");
}

NS_IMETHODIMP nsHardwareConfigSim::GetModemUuid(nsAString& aModemUuid) {
  aModemUuid = mModemUuid;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsHardwareConfigSim, nsIHardwareConfigSim)

/*============================================================================
 *============ Implementation of Class nsRadioCapability ===================
 *============================================================================*/
/**
 * nsRadioCapability implementation
 */
nsRadioCapability::nsRadioCapability(int32_t aSession, int32_t aPhase,
                                     int32_t aRaf,
                                     const nsAString& aLogicalModemUuid,
                                     int32_t aStatus)
    : mSession(aSession),
      mPhase(aPhase),
      mRaf(aRaf),
      mLogicalModemUuid(aLogicalModemUuid),
      mStatus(aStatus) {
  __android_log_print(ANDROID_LOG_INFO, " nsRadioCapability",
                      "init nsRadioCapability");
}

NS_IMETHODIMP nsRadioCapability::GetSession(int32_t* aSession) {
  *aSession = mSession;
  return NS_OK;
}

NS_IMETHODIMP nsRadioCapability::GetPhase(int32_t* aPhase) {
  *aPhase = mPhase;
  return NS_OK;
}

NS_IMETHODIMP nsRadioCapability::GetRaf(int32_t* aRaf) {
  *aRaf = mRaf;
  return NS_OK;
}

NS_IMETHODIMP nsRadioCapability::GetLogicalModemUuid(
    nsAString& aLogicalModemUuid) {
  aLogicalModemUuid = mLogicalModemUuid;
  return NS_OK;
}

NS_IMETHODIMP nsRadioCapability::GetStatus(int32_t* aStatus) {
  *aStatus = mStatus;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsRadioCapability, nsIRadioCapability)

/*============================================================================
 *============ Implementation of Class nsLceStatusInfo ===================
 *============================================================================*/
/**
 * nsLceStatusInfo implementation
 */
nsLceStatusInfo::nsLceStatusInfo(int32_t aLceStatus, int32_t aActualIntervalMs)
    : mLceStatus(aLceStatus), mActualIntervalMs(aActualIntervalMs) {
  __android_log_print(ANDROID_LOG_INFO, " nsLceStatusInfo",
                      "init nsLceStatusInfo");
}

NS_IMETHODIMP nsLceStatusInfo::GetLceStatus(int32_t* aLceStatus) {
  *aLceStatus = mLceStatus;
  return NS_OK;
}

NS_IMETHODIMP nsLceStatusInfo::GetActualIntervalMs(int32_t* aActualIntervalMs) {
  *aActualIntervalMs = mActualIntervalMs;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsLceStatusInfo, nsILceStatusInfo)

/*============================================================================
 *============ Implementation of Class nsLceDataInfo ===================
 *============================================================================*/
/**
 * nsLceDataInfo implementation
 */
nsLceDataInfo::nsLceDataInfo(int32_t aLastHopCapacityKbps,
                             int32_t aConfidenceLevel, bool aLceSuspended)
    : mLastHopCapacityKbps(aLastHopCapacityKbps),
      mConfidenceLevel(aConfidenceLevel),
      mLceSuspended(aLceSuspended) {
  __android_log_print(ANDROID_LOG_INFO, " nsLceDataInfo", "init nsLceDataInfo");
}

NS_IMETHODIMP nsLceDataInfo::GetLastHopCapacityKbps(
    int32_t* aLastHopCapacityKbps) {
  *aLastHopCapacityKbps = mLastHopCapacityKbps;
  return NS_OK;
}

NS_IMETHODIMP nsLceDataInfo::GetConfidenceLevel(int32_t* aConfidenceLevel) {
  *aConfidenceLevel = mConfidenceLevel;
  return NS_OK;
}

NS_IMETHODIMP nsLceDataInfo::GetLceSuspended(bool* aLceSuspended) {
  *aLceSuspended = mLceSuspended;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsLceDataInfo, nsILceDataInfo)

/*============================================================================
 *============ Implementation of Class nsPcoDataInfo ===================
 *============================================================================*/
/**
 * nsPcoDataInfo implementation
 */
nsPcoDataInfo::nsPcoDataInfo(int32_t aCid, const nsAString& aBearerProto,
                             bool aPcoId, nsTArray<int32_t>& aContents)
    : mCid(aCid),
      mBearerProto(aBearerProto),
      mPcoId(aPcoId),
      mContents(aContents.Clone()) {
  __android_log_print(ANDROID_LOG_INFO, " nsPcoDataInfo", "init nsPcoDataInfo");
}

NS_IMETHODIMP nsPcoDataInfo::GetCid(int32_t* aCid) {
  *aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsPcoDataInfo::GetBearerProto(nsAString& aBearerProto) {
  aBearerProto = mBearerProto;
  return NS_OK;
}

NS_IMETHODIMP nsPcoDataInfo::GetPcoId(int32_t* aPcoId) {
  *aPcoId = mPcoId;
  return NS_OK;
}

NS_IMETHODIMP nsPcoDataInfo::GetContents(uint32_t* count, int32_t** contents) {
  *count = mContents.Length();
  *contents = (int32_t*)moz_xmalloc((*count) * sizeof(int32_t));
  NS_ENSURE_TRUE(*contents, NS_ERROR_OUT_OF_MEMORY);

  for (uint32_t i = 0; i < *count; i++) {
    (*contents)[i] = mContents[i];
  }

  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsPcoDataInfo, nsIPcoDataInfo)

/*============================================================================
 *============ Implementation of Class nsAppStatus ===================
 *============================================================================*/
/**
 * nsAppStatus implementation
 */
nsAppStatus::nsAppStatus(int32_t aAppType, int32_t aAppState,
                         int32_t aPersoSubstate, const nsAString& aAidPtr,
                         const nsAString& aAppLabelPtr, int32_t aPin1Replaced,
                         int32_t aPin1, int32_t aPin2)
    : mAppType(aAppType),
      mAppState(aAppState),
      mPersoSubstate(aPersoSubstate),
      mAidPtr(aAidPtr),
      mAppLabelPtr(aAppLabelPtr),
      mPin1Replaced(aPin1Replaced),
      mPin1(aPin1),
      mPin2(aPin2) {
  __android_log_print(ANDROID_LOG_INFO, " nsAppStatus", "init nsAppStatus");
}

NS_IMETHODIMP nsAppStatus::GetAppType(int32_t* aAppType) {
  *aAppType = mAppType;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetAppState(int32_t* aAppState) {
  *aAppState = mAppState;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetPersoSubstate(int32_t* aPersoSubstate) {
  *aPersoSubstate = mPersoSubstate;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetAidPtr(nsAString& aAidPtr) {
  aAidPtr = mAidPtr;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetAppLabelPtr(nsAString& aAppLabelPtr) {
  aAppLabelPtr = mAppLabelPtr;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetPin1Replaced(int32_t* aPin1Replaced) {
  *aPin1Replaced = mPin1Replaced;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetPin1(int32_t* aPin1) {
  *aPin1 = mPin1;
  return NS_OK;
}

NS_IMETHODIMP nsAppStatus::GetPin2(int32_t* aPin2) {
  *aPin2 = mPin2;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsAppStatus, nsIAppStatus)

/*============================================================================
 *============ Implementation of Class nsCardStatus ===================
 *============================================================================*/
/**
 * nsCardStatus implementation
 */
nsCardStatus::nsCardStatus(int32_t aCardState, int32_t aUniversalPinState,
                           int32_t aGsmUmtsSubscriptionAppIndex,
                           int32_t aCdmaSubscriptionAppIndex,
                           int32_t aImsSubscriptionAppIndex,
                           nsTArray<RefPtr<nsAppStatus>>& aApplications)
    : mCardState(aCardState),
      mUniversalPinState(aUniversalPinState),
      mGsmUmtsSubscriptionAppIndex(aGsmUmtsSubscriptionAppIndex),
      mCdmaSubscriptionAppIndex(aCdmaSubscriptionAppIndex),
      mImsSubscriptionAppIndex(aImsSubscriptionAppIndex),
      mApplications(aApplications.Clone()) {
  __android_log_print(ANDROID_LOG_INFO, " nsCardStatus", "init nsCardStatus");
}

NS_IMETHODIMP nsCardStatus::GetCardState(int32_t* aCardState) {
  *aCardState = mCardState;
  return NS_OK;
}

NS_IMETHODIMP nsCardStatus::GetUniversalPinState(int32_t* aUniversalPinState) {
  *aUniversalPinState = mUniversalPinState;
  return NS_OK;
}

NS_IMETHODIMP nsCardStatus::GetGsmUmtsSubscriptionAppIndex(
    int32_t* aGsmUmtsSubscriptionAppIndex) {
  *aGsmUmtsSubscriptionAppIndex = mGsmUmtsSubscriptionAppIndex;
  return NS_OK;
}

NS_IMETHODIMP nsCardStatus::GetCdmaSubscriptionAppIndex(
    int32_t* aCdmaSubscriptionAppIndex) {
  *aCdmaSubscriptionAppIndex = mCdmaSubscriptionAppIndex;
  return NS_OK;
}

NS_IMETHODIMP nsCardStatus::GetImsSubscriptionAppIndex(
    int32_t* aImsSubscriptionAppIndex) {
  *aImsSubscriptionAppIndex = mImsSubscriptionAppIndex;
  return NS_OK;
}

NS_IMETHODIMP nsCardStatus::GetAppStatus(uint32_t* count,
                                         nsIAppStatus*** applications) {
  // Allocate prefix arrays
  *count = mApplications.Length();
  nsIAppStatus** application =
      (nsIAppStatus**)moz_xmalloc(*count * sizeof(nsIAppStatus*));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(application[i] = mApplications[i]);
  }

  *applications = application;

  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCardStatus, nsICardStatus)

/*============================================================================
 *============ Implementation of Class nsVoiceRegState ===================
 *============================================================================*/
/**
 * nsVoiceRegState implementation
 */
nsVoiceRegState::nsVoiceRegState(int32_t aRegState, int32_t aRat,
                                 bool aCssSupported, int32_t aRoamingIndicator,
                                 int32_t aSystemIsInPrl,
                                 int32_t aDefaultRoamingIndicator,
                                 int32_t aReasonForDenial,
                                 nsCellIdentity* aCellIdentity)
    : mRegState(aRegState),
      mRat(aRat),
      mCssSupported(aCssSupported),
      mRoamingIndicator(aRoamingIndicator),
      mSystemIsInPrl(aSystemIsInPrl),
      mDefaultRoamingIndicator(aDefaultRoamingIndicator),
      mReasonForDenial(aReasonForDenial),
      mCellIdentity(aCellIdentity) {
  __android_log_print(ANDROID_LOG_INFO, " nsVoiceRegState",
                      "init nsVoiceRegState");
}

NS_IMETHODIMP nsVoiceRegState::GetRegState(int32_t* aRegState) {
  *aRegState = mRegState;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetRat(int32_t* aRat) {
  *aRat = mRat;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetCssSupported(bool* aCssSupported) {
  *aCssSupported = mCssSupported;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetRoamingIndicator(int32_t* aRoamingIndicator) {
  *aRoamingIndicator = mRoamingIndicator;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetSystemIsInPrl(int32_t* aSystemIsInPrl) {
  *aSystemIsInPrl = mSystemIsInPrl;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetDefaultRoamingIndicator(
    int32_t* aDefaultRoamingIndicator) {
  *aDefaultRoamingIndicator = mDefaultRoamingIndicator;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetReasonForDenial(int32_t* aReasonForDenial) {
  *aReasonForDenial = mReasonForDenial;
  return NS_OK;
}

NS_IMETHODIMP nsVoiceRegState::GetCellIdentity(
    nsICellIdentity** aCellIdentity) {
  RefPtr<nsICellIdentity> cellIdentity(mCellIdentity);
  cellIdentity.forget(aCellIdentity);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsVoiceRegState, nsIVoiceRegState)

/*============================================================================
 *============ Implementation of Class nsDataRegState ===================
 *============================================================================*/
/**
 * nsDataRegState implementation
 */
nsDataRegState::nsDataRegState(int32_t aRegState, int32_t aRat,
                               int32_t aReasonDataDenied, int32_t aMaxDataCalls,
                               nsCellIdentity* aCellIdentity)
    : mRegState(aRegState),
      mRat(aRat),
      mReasonDataDenied(aReasonDataDenied),
      mMaxDataCalls(aMaxDataCalls),
      mCellIdentity(aCellIdentity) {
  __android_log_print(ANDROID_LOG_INFO, " nsDataRegState",
                      "init nsDataRegState");
}

NS_IMETHODIMP nsDataRegState::GetRegState(int32_t* aRegState) {
  *aRegState = mRegState;
  return NS_OK;
}

NS_IMETHODIMP nsDataRegState::GetRat(int32_t* aRat) {
  *aRat = mRat;
  return NS_OK;
}

NS_IMETHODIMP nsDataRegState::GetReasonDataDenied(int32_t* aReasonDataDenied) {
  *aReasonDataDenied = mReasonDataDenied;
  return NS_OK;
}

NS_IMETHODIMP nsDataRegState::GetMaxDataCalls(int32_t* aMaxDataCalls) {
  *aMaxDataCalls = mMaxDataCalls;
  return NS_OK;
}

NS_IMETHODIMP nsDataRegState::GetCellIdentity(nsICellIdentity** aCellIdentity) {
  RefPtr<nsICellIdentity> cellIdentity(mCellIdentity);
  cellIdentity.forget(aCellIdentity);
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsDataRegState, nsIDataRegState)

/*============================================================================
 *============ Implementation of Class nsOperatorInfo ===================
 *============================================================================*/
/**
 * nsOperatorInfo implementation
 */
nsOperatorInfo::nsOperatorInfo(const nsAString& aAlphaLong,
                               const nsAString& aAlphaShort,
                               const nsAString& aOperatorNumeric,
                               int32_t aStatus)
    : mAlphaLong(aAlphaLong),
      mAlphaShort(aAlphaShort),
      mOperatorNumeric(aOperatorNumeric),
      mStatus(aStatus) {
  __android_log_print(ANDROID_LOG_INFO, " nsOperatorInfo",
                      "init nsOperatorInfo");
}

NS_IMETHODIMP nsOperatorInfo::GetAlphaLong(nsAString& aAlphaLong) {
  aAlphaLong = mAlphaLong;
  return NS_OK;
}

NS_IMETHODIMP nsOperatorInfo::GetAlphaShort(nsAString& aAlphaShort) {
  aAlphaShort = mAlphaShort;
  return NS_OK;
}

NS_IMETHODIMP nsOperatorInfo::GetOperatorNumeric(nsAString& aOperatorNumeric) {
  aOperatorNumeric = mOperatorNumeric;
  return NS_OK;
}

NS_IMETHODIMP nsOperatorInfo::GetStatus(int32_t* aStatus) {
  *aStatus = mStatus;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsOperatorInfo, nsIOperatorInfo)

/*============================================================================
 *============ Implementation of Class nsNeighboringCell ===================
 *============================================================================*/
/**
 * nsNeighboringCell implementation
 */
nsNeighboringCell::nsNeighboringCell(const nsAString& aCid, int32_t aRssi)
    : mCid(aCid), mRssi(aRssi) {
  __android_log_print(ANDROID_LOG_INFO, " nsNeighboringCell",
                      "init nsNeighboringCell");
}

NS_IMETHODIMP nsNeighboringCell::GetCid(nsAString& aCid) {
  aCid = mCid;
  return NS_OK;
}

NS_IMETHODIMP nsNeighboringCell::GetRssi(int32_t* aRssi) {
  *aRssi = mRssi;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsNeighboringCell, nsINeighboringCell)

/*============================================================================
 *============ Implementation of Class nsCall ===================
 *============================================================================*/
/**
 * nsCall implementation
 */
nsCall::nsCall(int32_t aState, int32_t aIndex, int32_t aToa, bool aIsMpty,
               bool aIsMT, int32_t aAls, bool aIsVoice, bool aIsVoicePrivacy,
               const nsAString& aNumber, int32_t aNumberPresentation,
               const nsAString& aName, int32_t aNamePresentation,
               nsTArray<RefPtr<nsUusInfo>>& aUusInfo)
    : mState(aState),
      mIndex(aIndex),
      mToa(aToa),
      mIsMpty(aIsMpty),
      mIsMT(aIsMT),
      mAls(aAls),
      mIsVoice(aIsVoice),
      mIsVoicePrivacy(aIsVoicePrivacy),
      mNumber(aNumber),
      mNumberPresentation(aNumberPresentation),
      mName(aName),
      mNamePresentation(aNamePresentation),
      mUusInfo(aUusInfo.Clone()) {
  __android_log_print(ANDROID_LOG_INFO, " nsCall", "init nsCall");
}

NS_IMETHODIMP nsCall::GetState(int32_t* aState) {
  *aState = mState;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetIndex(int32_t* aIndex) {
  *aIndex = mIndex;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetToa(int32_t* aToa) {
  *aToa = mToa;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetIsMpty(bool* aIsMpty) {
  *aIsMpty = mIsMpty;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetIsMT(bool* aIsMT) {
  *aIsMT = mIsMT;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetAls(int32_t* aAls) {
  *aAls = mAls;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetIsVoice(bool* aIsVoice) {
  *aIsVoice = mIsVoice;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetIsVoicePrivacy(bool* aIsVoicePrivacy) {
  *aIsVoicePrivacy = mIsVoicePrivacy;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetNumber(nsAString& aNumber) {
  aNumber = mNumber;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetNumberPresentation(int32_t* aNumberPresentation) {
  *aNumberPresentation = mNumberPresentation;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetName(nsAString& aName) {
  aName = mName;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetNamePresentation(int32_t* aNamePresentation) {
  *aNamePresentation = mNamePresentation;
  return NS_OK;
}

NS_IMETHODIMP nsCall::GetUusInfo(uint32_t* count, nsIUusInfo*** uusInfos) {
  *count = mUusInfo.Length();
  nsIUusInfo** uusinfo =
      (nsIUusInfo**)moz_xmalloc(*count * sizeof(nsIUusInfo*));

  for (uint32_t i = 0; i < *count; i++) {
    NS_ADDREF(uusinfo[i] = mUusInfo[i]);
  }

  *uusInfos = uusinfo;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCall, nsICall)

/*============================================================================
 *============ Implementation of Class nsUusInfo ===================
 *============================================================================*/
/**
 * nsUusInfo implementation
 */
nsUusInfo::nsUusInfo(int32_t aUusType, int32_t aUusDcs,
                     const nsAString& aUusData)
    : mUusType(aUusType), mUusDcs(aUusDcs), mUusData(aUusData) {
  __android_log_print(ANDROID_LOG_INFO, " nsUusInfo", "init nsUusInfo");
}

NS_IMETHODIMP nsUusInfo::GetUusType(int32_t* aUusType) {
  *aUusType = mUusType;
  return NS_OK;
}

NS_IMETHODIMP nsUusInfo::GetUusDcs(int32_t* aUusDcs) {
  *aUusDcs = mUusDcs;
  return NS_OK;
}

NS_IMETHODIMP nsUusInfo::GetUusData(nsAString& aUusData) {
  aUusData = mUusData;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsUusInfo, nsIUusInfo)

/*============================================================================
 *============ Implementation of Class nsIccIoResult ===================
 *============================================================================*/
/**
 * nsIccIoResult implementation
 */
nsIccIoResult::nsIccIoResult(int32_t aSw1, int32_t aSw2,
                             const nsAString& aSimResponse)
    : mSw1(aSw1), mSw2(aSw2), mSimResponse(aSimResponse) {
  __android_log_print(ANDROID_LOG_INFO, " nsIccIoResult", "init nsIccIoResult");
}

NS_IMETHODIMP nsIccIoResult::GetSw1(int32_t* aSw1) {
  *aSw1 = mSw1;
  return NS_OK;
}

NS_IMETHODIMP nsIccIoResult::GetSw2(int32_t* aSw2) {
  *aSw2 = mSw2;
  return NS_OK;
}

NS_IMETHODIMP nsIccIoResult::GetSimResponse(nsAString& aSimResponse) {
  aSimResponse = mSimResponse;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsIccIoResult, nsIIccIoResult)

/*============================================================================
 *============ Implementation of Class nsCallForwardInfo ===================
 *============================================================================*/
/**
 * nsCallForwardInfo implementation
 */
nsCallForwardInfo::nsCallForwardInfo(int32_t aStatus, int32_t aReason,
                                     int32_t aServiceClass, int32_t aToa,
                                     const nsAString& aNumber,
                                     int32_t aTimeSeconds)
    : mStatus(aStatus),
      mReason(aReason),
      mServiceClass(aServiceClass),
      mToa(aToa),
      mNumber(aNumber),
      mTimeSeconds(aTimeSeconds) {
  __android_log_print(ANDROID_LOG_INFO, " nsCallForwardInfo",
                      "init nsCallForwardInfo");
}

NS_IMETHODIMP nsCallForwardInfo::GetStatus(int32_t* aStatus) {
  *aStatus = mStatus;
  return NS_OK;
}

NS_IMETHODIMP nsCallForwardInfo::GetReason(int32_t* aReason) {
  *aReason = mReason;
  return NS_OK;
}

NS_IMETHODIMP nsCallForwardInfo::GetServiceClass(int32_t* aServiceClass) {
  *aServiceClass = mServiceClass;
  return NS_OK;
}

NS_IMETHODIMP nsCallForwardInfo::GetToa(int32_t* aToa) {
  *aToa = mToa;
  return NS_OK;
}

NS_IMETHODIMP nsCallForwardInfo::GetNumber(nsAString& aNumber) {
  aNumber = mNumber;
  return NS_OK;
}

NS_IMETHODIMP nsCallForwardInfo::GetTimeSeconds(int32_t* aTimeSeconds) {
  *aTimeSeconds = mTimeSeconds;
  return NS_OK;
}

NS_IMPL_ISUPPORTS(nsCallForwardInfo, nsICallForwardInfo)
/*============================================================================
 *======================Implementation of Class nsRilResult
 *=====================
 *============================================================================*/
/**
 * Constructor for a nsRilResult
 * For those has no parameter notify.
 */
nsRilResult::nsRilResult(const nsAString& aRilMessageType)
    : mRilMessageType(aRilMessageType) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResult",
                      "init nsRilResult for indication.");
}
nsRilResult::nsRilResult(const nsAString& aRilMessageType,
                         int32_t aRilMessageToken, int32_t aErrorMsg)
    : mRilMessageType(aRilMessageType),
      mRilMessageToken(aRilMessageToken),
      mErrorMsg(aErrorMsg) {
  __android_log_print(ANDROID_LOG_INFO, " nsRilResult",
                      "init nsRilResult for response.");
}

/**
 *
 */
nsRilResult::~nsRilResult() {}

// Helper function

int32_t nsRilResult::convertRadioTechnology(RadioTechnology aRat) {
  switch (aRat) {
    case RadioTechnology::UNKNOWN:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_UNKNOWN;
    case RadioTechnology::GPRS:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_GPRS;
    case RadioTechnology::EDGE:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_EDGE;
    case RadioTechnology::UMTS:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_UMTS;
    case RadioTechnology::IS95A:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_IS95A;
    case RadioTechnology::IS95B:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_IS95B;
    case RadioTechnology::ONE_X_RTT:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_1XRTT;
    case RadioTechnology::EVDO_0:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_EVDO0;
    case RadioTechnology::EVDO_A:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_EVDOA;
    case RadioTechnology::HSDPA:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_HSDPA;
    case RadioTechnology::HSUPA:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_HSUPA;
    case RadioTechnology::HSPA:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_HSPA;
    case RadioTechnology::EVDO_B:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_EVDOB;
    case RadioTechnology::EHRPD:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_EHRPD;
    case RadioTechnology::LTE:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_LTE;
    case RadioTechnology::HSPAP:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_HSPAP;
    case RadioTechnology::GSM:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_GSM;
    case RadioTechnology::TD_SCDMA:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_TD_SCDMA;
    case RadioTechnology::IWLAN:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_IWLAN;
    case RadioTechnology::LTE_CA:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_LTE_CA;
    default:
      return nsIRadioTechnologyState::RADIO_CREG_TECH_UNKNOWN;
  }
}

int32_t nsRilResult::convertDataCallFailCause(DataCallFailCause aCause) {
  switch (aCause) {
    case DataCallFailCause::NONE:
      return nsIDataCallFailCause::DATACALL_FAIL_NONE;
    case DataCallFailCause::OPERATOR_BARRED:
      return nsIDataCallFailCause::DATACALL_FAIL_OPERATOR_BARRED;
    case DataCallFailCause::NAS_SIGNALLING:
      return nsIDataCallFailCause::DATACALL_FAIL_NAS_SIGNALLING;
    case DataCallFailCause::INSUFFICIENT_RESOURCES:
      return nsIDataCallFailCause::DATACALL_FAIL_INSUFFICIENT_RESOURCES;
    case DataCallFailCause::MISSING_UKNOWN_APN:
      return nsIDataCallFailCause::DATACALL_FAIL_MISSING_UKNOWN_APN;
    case DataCallFailCause::UNKNOWN_PDP_ADDRESS_TYPE:
      return nsIDataCallFailCause::DATACALL_FAIL_UNKNOWN_PDP_ADDRESS_TYPE;
    case DataCallFailCause::USER_AUTHENTICATION:
      return nsIDataCallFailCause::DATACALL_FAIL_USER_AUTHENTICATION;
    case DataCallFailCause::ACTIVATION_REJECT_GGSN:
      return nsIDataCallFailCause::DATACALL_FAIL_ACTIVATION_REJECT_GGSN;
    case DataCallFailCause::ACTIVATION_REJECT_UNSPECIFIED:
      return nsIDataCallFailCause::DATACALL_FAIL_ACTIVATION_REJECT_UNSPECIFIED;
    case DataCallFailCause::SERVICE_OPTION_NOT_SUPPORTED:
      return nsIDataCallFailCause::DATACALL_FAIL_SERVICE_OPTION_NOT_SUPPORTED;
    case DataCallFailCause::SERVICE_OPTION_NOT_SUBSCRIBED:
      return nsIDataCallFailCause::DATACALL_FAIL_SERVICE_OPTION_NOT_SUBSCRIBED;
    case DataCallFailCause::SERVICE_OPTION_OUT_OF_ORDER:
      return nsIDataCallFailCause::DATACALL_FAIL_SERVICE_OPTION_OUT_OF_ORDER;
    case DataCallFailCause::NSAPI_IN_USE:
      return nsIDataCallFailCause::DATACALL_FAIL_NSAPI_IN_USE;
    case DataCallFailCause::REGULAR_DEACTIVATION:
      return nsIDataCallFailCause::DATACALL_FAIL_REGULAR_DEACTIVATION;
    case DataCallFailCause::QOS_NOT_ACCEPTED:
      return nsIDataCallFailCause::DATACALL_FAIL_QOS_NOT_ACCEPTED;
    case DataCallFailCause::NETWORK_FAILURE:
      return nsIDataCallFailCause::DATACALL_FAIL_NETWORK_FAILURE;
    case DataCallFailCause::UMTS_REACTIVATION_REQ:
      return nsIDataCallFailCause::DATACALL_FAIL_UMTS_REACTIVATION_REQ;
    case DataCallFailCause::FEATURE_NOT_SUPP:
      return nsIDataCallFailCause::DATACALL_FAIL_FEATURE_NOT_SUPP;
    case DataCallFailCause::TFT_SEMANTIC_ERROR:
      return nsIDataCallFailCause::DATACALL_FAIL_TFT_SEMANTIC_ERROR;
    case DataCallFailCause::TFT_SYTAX_ERROR:
      return nsIDataCallFailCause::DATACALL_FAIL_TFT_SYTAX_ERROR;
    case DataCallFailCause::UNKNOWN_PDP_CONTEXT:
      return nsIDataCallFailCause::DATACALL_FAIL_UNKNOWN_PDP_CONTEXT;
    case DataCallFailCause::FILTER_SEMANTIC_ERROR:
      return nsIDataCallFailCause::DATACALL_FAIL_FILTER_SEMANTIC_ERROR;
    case DataCallFailCause::FILTER_SYTAX_ERROR:
      return nsIDataCallFailCause::DATACALL_FAIL_FILTER_SYTAX_ERROR;
    case DataCallFailCause::PDP_WITHOUT_ACTIVE_TFT:
      return nsIDataCallFailCause::DATACALL_FAIL_PDP_WITHOUT_ACTIVE_TFT;
    case DataCallFailCause::ONLY_IPV4_ALLOWED:
      return nsIDataCallFailCause::DATACALL_FAIL_ONLY_IPV4_ALLOWED;
    case DataCallFailCause::ONLY_IPV6_ALLOWED:
      return nsIDataCallFailCause::DATACALL_FAIL_ONLY_IPV6_ALLOWED;
    case DataCallFailCause::ONLY_SINGLE_BEARER_ALLOWED:
      return nsIDataCallFailCause::DATACALL_FAIL_ONLY_SINGLE_BEARER_ALLOWED;
    case DataCallFailCause::ESM_INFO_NOT_RECEIVED:
      return nsIDataCallFailCause::DATACALL_FAIL_ESM_INFO_NOT_RECEIVED;
    case DataCallFailCause::PDN_CONN_DOES_NOT_EXIST:
      return nsIDataCallFailCause::DATACALL_FAIL_PDN_CONN_DOES_NOT_EXIST;
    case DataCallFailCause::MULTI_CONN_TO_SAME_PDN_NOT_ALLOWED:
      return nsIDataCallFailCause::
          DATACALL_FAIL_MULTI_CONN_TO_SAME_PDN_NOT_ALLOWED;
    case DataCallFailCause::MAX_ACTIVE_PDP_CONTEXT_REACHED:
      return nsIDataCallFailCause::DATACALL_FAIL_MAX_ACTIVE_PDP_CONTEXT_REACHED;
    case DataCallFailCause::UNSUPPORTED_APN_IN_CURRENT_PLMN:
      return nsIDataCallFailCause::
          DATACALL_FAIL_UNSUPPORTED_APN_IN_CURRENT_PLMN;
    case DataCallFailCause::INVALID_TRANSACTION_ID:
      return nsIDataCallFailCause::DATACALL_FAIL_INVALID_TRANSACTION_ID;
    case DataCallFailCause::MESSAGE_INCORRECT_SEMANTIC:
      return nsIDataCallFailCause::DATACALL_FAIL_MESSAGE_INCORRECT_SEMANTIC;
    case DataCallFailCause::INVALID_MANDATORY_INFO:
      return nsIDataCallFailCause::DATACALL_FAIL_INVALID_MANDATORY_INFO;
    case DataCallFailCause::MESSAGE_TYPE_UNSUPPORTED:
      return nsIDataCallFailCause::DATACALL_FAIL_MESSAGE_TYPE_UNSUPPORTED;
    case DataCallFailCause::MSG_TYPE_NONCOMPATIBLE_STATE:
      return nsIDataCallFailCause::DATACALL_FAIL_MSG_TYPE_NONCOMPATIBLE_STATE;
    case DataCallFailCause::UNKNOWN_INFO_ELEMENT:
      return nsIDataCallFailCause::DATACALL_FAIL_UNKNOWN_INFO_ELEMENT;
    case DataCallFailCause::CONDITIONAL_IE_ERROR:
      return nsIDataCallFailCause::DATACALL_FAIL_CONDITIONAL_IE_ERROR;
    case DataCallFailCause::MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE:
      return nsIDataCallFailCause::
          DATACALL_FAIL_MSG_AND_PROTOCOL_STATE_UNCOMPATIBLE;
    case DataCallFailCause::PROTOCOL_ERRORS:
      return nsIDataCallFailCause::DATACALL_FAIL_PROTOCOL_ERRORS;
    case DataCallFailCause::APN_TYPE_CONFLICT:
      return nsIDataCallFailCause::DATACALL_FAIL_APN_TYPE_CONFLICT;
    case DataCallFailCause::INVALID_PCSCF_ADDR:
      return nsIDataCallFailCause::DATACALL_FAIL_INVALID_PCSCF_ADDR;
    case DataCallFailCause::INTERNAL_CALL_PREEMPT_BY_HIGH_PRIO_APN:
      return nsIDataCallFailCause::
          DATACALL_FAIL_INTERNAL_CALL_PREEMPT_BY_HIGH_PRIO_APN;
    case DataCallFailCause::EMM_ACCESS_BARRED:
      return nsIDataCallFailCause::DATACALL_FAIL_EMM_ACCESS_BARRED;
    case DataCallFailCause::EMERGENCY_IFACE_ONLY:
      return nsIDataCallFailCause::DATACALL_FAIL_EMERGENCY_IFACE_ONLY;
    case DataCallFailCause::IFACE_MISMATCH:
      return nsIDataCallFailCause::DATACALL_FAIL_IFACE_MISMATCH;
    case DataCallFailCause::COMPANION_IFACE_IN_USE:
      return nsIDataCallFailCause::DATACALL_FAIL_COMPANION_IFACE_IN_USE;
    case DataCallFailCause::IP_ADDRESS_MISMATCH:
      return nsIDataCallFailCause::DATACALL_FAIL_IP_ADDRESS_MISMATCH;
    case DataCallFailCause::IFACE_AND_POL_FAMILY_MISMATCH:
      return nsIDataCallFailCause::DATACALL_FAIL_IFACE_AND_POL_FAMILY_MISMATCH;
    case DataCallFailCause::EMM_ACCESS_BARRED_INFINITE_RETRY:
      return nsIDataCallFailCause::
          DATACALL_FAIL_EMM_ACCESS_BARRED_INFINITE_RETRY;
    case DataCallFailCause::AUTH_FAILURE_ON_EMERGENCY_CALL:
      return nsIDataCallFailCause::DATACALL_FAIL_AUTH_FAILURE_ON_EMERGENCY_CALL;
    case DataCallFailCause::VOICE_REGISTRATION_FAIL:
      return nsIDataCallFailCause::DATACALL_FAIL_VOICE_REGISTRATION_FAIL;
    case DataCallFailCause::DATA_REGISTRATION_FAIL:
      return nsIDataCallFailCause::DATACALL_FAIL_DATA_REGISTRATION_FAIL;
    case DataCallFailCause::SIGNAL_LOST:
      return nsIDataCallFailCause::DATACALL_FAIL_SIGNAL_LOST;
    case DataCallFailCause::PREF_RADIO_TECH_CHANGED:
      return nsIDataCallFailCause::DATACALL_FAIL_PREF_RADIO_TECH_CHANGED;
    case DataCallFailCause::RADIO_POWER_OFF:
      return nsIDataCallFailCause::DATACALL_FAIL_RADIO_POWER_OFF;
    case DataCallFailCause::TETHERED_CALL_ACTIVE:
      return nsIDataCallFailCause::DATACALL_FAIL_TETHERED_CALL_ACTIVE;
    case DataCallFailCause::ERROR_UNSPECIFIED:
      return nsIDataCallFailCause::DATACALL_FAIL_ERROR_UNSPECIFIED;
    default:
      return nsIDataCallFailCause::DATACALL_FAIL_ERROR_UNSPECIFIED;
  }
}

RefPtr<nsCellIdentity> nsRilResult::convertCellIdentity(
    const CellIdentity& aCellIdentity) {
  int32_t cellInfoType = convertCellInfoType(aCellIdentity.cellInfoType);
  if (aCellIdentity.cellInfoType == CellInfoType::GSM) {
    uint32_t numCellIdentityGsm = aCellIdentity.cellIdentityGsm.size();
    if (numCellIdentityGsm > 0) {
      RefPtr<nsCellIdentityGsm> cellIdentityGsm =
          convertCellIdentityGsm(aCellIdentity.cellIdentityGsm[0]);
      RefPtr<nsCellIdentity> cellIdentity =
          new nsCellIdentity(cellInfoType, cellIdentityGsm);
      return cellIdentity;
    }
  } else if (aCellIdentity.cellInfoType == CellInfoType::LTE) {
    uint32_t numCellIdentityLte = aCellIdentity.cellIdentityLte.size();
    if (numCellIdentityLte > 0) {
      RefPtr<nsCellIdentityLte> cellIdentityLte =
          convertCellIdentityLte(aCellIdentity.cellIdentityLte[0]);
      RefPtr<nsCellIdentity> cellIdentity =
          new nsCellIdentity(cellInfoType, cellIdentityLte);
      return cellIdentity;
    }
  } else if (aCellIdentity.cellInfoType == CellInfoType::WCDMA) {
    uint32_t numCellIdentityWcdma = aCellIdentity.cellIdentityWcdma.size();
    if (numCellIdentityWcdma > 0) {
      RefPtr<nsCellIdentityWcdma> cellIdentityWcdma =
          convertCellIdentityWcdma(aCellIdentity.cellIdentityWcdma[0]);
      RefPtr<nsCellIdentity> cellIdentity =
          new nsCellIdentity(cellInfoType, cellIdentityWcdma);
      return cellIdentity;
    }
  } else if (aCellIdentity.cellInfoType == CellInfoType::TD_SCDMA) {
    uint32_t numCellIdentityTdScdma = aCellIdentity.cellIdentityTdscdma.size();
    if (numCellIdentityTdScdma > 0) {
      RefPtr<nsCellIdentityTdScdma> cellIdentityTdScdma =
          convertCellIdentityTdScdma(aCellIdentity.cellIdentityTdscdma[0]);
      RefPtr<nsCellIdentity> cellIdentity =
          new nsCellIdentity(cellInfoType, cellIdentityTdScdma);
      return cellIdentity;
    }
  } else if (aCellIdentity.cellInfoType == CellInfoType::CDMA) {
    uint32_t numCellIdentityCdma = aCellIdentity.cellIdentityCdma.size();
    if (numCellIdentityCdma > 0) {
      RefPtr<nsCellIdentityCdma> cellIdentityCdma =
          convertCellIdentityCdma(aCellIdentity.cellIdentityCdma[0]);
      RefPtr<nsCellIdentity> cellIdentity =
          new nsCellIdentity(cellInfoType, cellIdentityCdma);
      return cellIdentity;
    }
  }
  return nullptr;
}

RefPtr<nsCellIdentityGsm> nsRilResult::convertCellIdentityGsm(
    const CellIdentityGsm& aCellIdentityGsm) {
  RefPtr<nsCellIdentityGsm> cellIdentityGsm = new nsCellIdentityGsm(
      NS_ConvertUTF8toUTF16(aCellIdentityGsm.mcc.c_str()),
      NS_ConvertUTF8toUTF16(aCellIdentityGsm.mnc.c_str()), aCellIdentityGsm.lac,
      aCellIdentityGsm.cid, aCellIdentityGsm.arfcn, aCellIdentityGsm.bsic);
  return cellIdentityGsm;
}

RefPtr<nsCellIdentityLte> nsRilResult::convertCellIdentityLte(
    const CellIdentityLte& aCellIdentityLte) {
  RefPtr<nsCellIdentityLte> cellIdentityLte = new nsCellIdentityLte(
      NS_ConvertUTF8toUTF16(aCellIdentityLte.mcc.c_str()),
      NS_ConvertUTF8toUTF16(aCellIdentityLte.mnc.c_str()), aCellIdentityLte.ci,
      aCellIdentityLte.pci, aCellIdentityLte.tac, aCellIdentityLte.earfcn);
  return cellIdentityLte;
}

RefPtr<nsCellIdentityWcdma> nsRilResult::convertCellIdentityWcdma(
    const CellIdentityWcdma& aCellIdentityWcdma) {
  RefPtr<nsCellIdentityWcdma> cellIdentityWcdma = new nsCellIdentityWcdma(
      NS_ConvertUTF8toUTF16(aCellIdentityWcdma.mcc.c_str()),
      NS_ConvertUTF8toUTF16(aCellIdentityWcdma.mnc.c_str()),
      aCellIdentityWcdma.lac, aCellIdentityWcdma.cid, aCellIdentityWcdma.psc,
      aCellIdentityWcdma.uarfcn);
  return cellIdentityWcdma;
}

RefPtr<nsCellIdentityTdScdma> nsRilResult::convertCellIdentityTdScdma(
    const CellIdentityTdscdma& aCellIdentityTdScdma) {
  RefPtr<nsCellIdentityTdScdma> cellIdentityTdScdma = new nsCellIdentityTdScdma(
      NS_ConvertUTF8toUTF16(aCellIdentityTdScdma.mcc.c_str()),
      NS_ConvertUTF8toUTF16(aCellIdentityTdScdma.mnc.c_str()),
      aCellIdentityTdScdma.lac, aCellIdentityTdScdma.cid,
      aCellIdentityTdScdma.cpid);
  return cellIdentityTdScdma;
}

RefPtr<nsCellIdentityCdma> nsRilResult::convertCellIdentityCdma(
    const CellIdentityCdma& aCellIdentityCdma) {
  RefPtr<nsCellIdentityCdma> cellIdentityCdma = new nsCellIdentityCdma(
      aCellIdentityCdma.networkId, aCellIdentityCdma.systemId,
      aCellIdentityCdma.baseStationId, aCellIdentityCdma.longitude,
      aCellIdentityCdma.latitude);
  return cellIdentityCdma;
}

RefPtr<nsSignalStrength> nsRilResult::convertSignalStrength(
    const SignalStrength& aSignalStrength) {
  RefPtr<nsGsmSignalStrength> gsmSignalStrength =
      convertGsmSignalStrength(aSignalStrength.gw);
  RefPtr<nsCdmaSignalStrength> cdmaSignalStrength =
      convertCdmaSignalStrength(aSignalStrength.cdma);
  RefPtr<nsEvdoSignalStrength> evdoSignalStrength =
      convertEvdoSignalStrength(aSignalStrength.evdo);
  RefPtr<nsLteSignalStrength> lteSignalStrength =
      convertLteSignalStrength(aSignalStrength.lte);
  RefPtr<nsTdScdmaSignalStrength> tdscdmaSignalStrength =
      convertTdScdmaSignalStrength(aSignalStrength.tdScdma);

  RefPtr<nsSignalStrength> signalStrength = new nsSignalStrength(
      gsmSignalStrength, cdmaSignalStrength, evdoSignalStrength,
      lteSignalStrength, tdscdmaSignalStrength);

  return signalStrength;
}

RefPtr<nsGsmSignalStrength> nsRilResult::convertGsmSignalStrength(
    const GsmSignalStrength& aGsmSignalStrength) {
  RefPtr<nsGsmSignalStrength> gsmSignalStrength = new nsGsmSignalStrength(
      aGsmSignalStrength.signalStrength, aGsmSignalStrength.bitErrorRate,
      aGsmSignalStrength.timingAdvance);

  return gsmSignalStrength;
}

RefPtr<nsWcdmaSignalStrength> nsRilResult::convertWcdmaSignalStrength(
    const WcdmaSignalStrength& aWcdmaSignalStrength) {
  RefPtr<nsWcdmaSignalStrength> wcdmaSignalStrength = new nsWcdmaSignalStrength(
      aWcdmaSignalStrength.signalStrength, aWcdmaSignalStrength.bitErrorRate);

  return wcdmaSignalStrength;
}
RefPtr<nsCdmaSignalStrength> nsRilResult::convertCdmaSignalStrength(
    const CdmaSignalStrength& aCdmaSignalStrength) {
  RefPtr<nsCdmaSignalStrength> cdmaSignalStrength = new nsCdmaSignalStrength(
      aCdmaSignalStrength.dbm, aCdmaSignalStrength.ecio);

  return cdmaSignalStrength;
}
RefPtr<nsEvdoSignalStrength> nsRilResult::convertEvdoSignalStrength(
    const EvdoSignalStrength& aEvdoSignalStrength) {
  RefPtr<nsEvdoSignalStrength> evdoSignalStrength = new nsEvdoSignalStrength(
      aEvdoSignalStrength.dbm, aEvdoSignalStrength.ecio,
      aEvdoSignalStrength.signalNoiseRatio);

  return evdoSignalStrength;
}
RefPtr<nsLteSignalStrength> nsRilResult::convertLteSignalStrength(
    const LteSignalStrength& aLteSignalStrength) {
  RefPtr<nsLteSignalStrength> lteSignalStrength = new nsLteSignalStrength(
      aLteSignalStrength.signalStrength, aLteSignalStrength.rsrp,
      aLteSignalStrength.rsrq, aLteSignalStrength.rssnr, aLteSignalStrength.cqi,
      aLteSignalStrength.timingAdvance);

  return lteSignalStrength;
}
RefPtr<nsTdScdmaSignalStrength> nsRilResult::convertTdScdmaSignalStrength(
    const TdScdmaSignalStrength& aTdScdmaSignalStrength) {
  RefPtr<nsTdScdmaSignalStrength> tdscdmaSignalStrength =
      new nsTdScdmaSignalStrength(aTdScdmaSignalStrength.rscp);

  return tdscdmaSignalStrength;
}

RefPtr<nsRilCellInfo> nsRilResult::convertRilCellInfo(
    const CellInfo& aCellInfo) {
  int32_t cellInfoType = convertCellInfoType(aCellInfo.cellInfoType);
  bool registered = aCellInfo.registered;
  int32_t timeStampType = convertTimeStampType(aCellInfo.timeStampType);
  int32_t timeStamp = (int32_t)aCellInfo.timeStamp;

  if (aCellInfo.cellInfoType == CellInfoType::GSM) {
    RefPtr<nsCellInfoGsm> cellInfoGsm = convertCellInfoGsm(aCellInfo.gsm[0]);
    RefPtr<nsRilCellInfo> cellInfo = new nsRilCellInfo(
        cellInfoType, registered, timeStampType, timeStamp, cellInfoGsm);
    return cellInfo;
  } else if (aCellInfo.cellInfoType == CellInfoType::LTE) {
    RefPtr<nsCellInfoLte> cellInfoLte = convertCellInfoLte(aCellInfo.lte[0]);
    RefPtr<nsRilCellInfo> cellInfo = new nsRilCellInfo(
        cellInfoType, registered, timeStampType, timeStamp, cellInfoLte);
    return cellInfo;
  } else if (aCellInfo.cellInfoType == CellInfoType::WCDMA) {
    RefPtr<nsCellInfoWcdma> cellInfoWcdma =
        convertCellInfoWcdma(aCellInfo.wcdma[0]);
    RefPtr<nsRilCellInfo> cellInfo = new nsRilCellInfo(
        cellInfoType, registered, timeStampType, timeStamp, cellInfoWcdma);
    return cellInfo;
  } else if (aCellInfo.cellInfoType == CellInfoType::TD_SCDMA) {
    RefPtr<nsCellInfoTdScdma> cellInfoTdScdma =
        convertCellInfoTdScdma(aCellInfo.tdscdma[0]);
    RefPtr<nsRilCellInfo> cellInfo = new nsRilCellInfo(
        cellInfoType, registered, timeStampType, timeStamp, cellInfoTdScdma);
    return cellInfo;
  } else if (aCellInfo.cellInfoType == CellInfoType::CDMA) {
    RefPtr<nsCellInfoCdma> cellInfoCdma =
        convertCellInfoCdma(aCellInfo.cdma[0]);
    RefPtr<nsRilCellInfo> cellInfo = new nsRilCellInfo(
        cellInfoType, registered, timeStampType, timeStamp, cellInfoCdma);
    return cellInfo;
  }
}

RefPtr<nsCellInfoGsm> nsRilResult::convertCellInfoGsm(
    const CellInfoGsm& aCellInfoGsm) {
  RefPtr<nsCellIdentityGsm> cellIdentityGsm =
      convertCellIdentityGsm(aCellInfoGsm.cellIdentityGsm);
  RefPtr<nsGsmSignalStrength> gsmSignalStrength =
      convertGsmSignalStrength(aCellInfoGsm.signalStrengthGsm);
  RefPtr<nsCellInfoGsm> cellInfoGsm =
      new nsCellInfoGsm(cellIdentityGsm, gsmSignalStrength);

  return cellInfoGsm;
}

RefPtr<nsCellInfoCdma> nsRilResult::convertCellInfoCdma(
    const CellInfoCdma& aCellInfoCdma) {
  RefPtr<nsCellIdentityCdma> cellIdentityCdma =
      convertCellIdentityCdma(aCellInfoCdma.cellIdentityCdma);
  RefPtr<nsCdmaSignalStrength> cdmaSignalStrength =
      convertCdmaSignalStrength(aCellInfoCdma.signalStrengthCdma);
  RefPtr<nsEvdoSignalStrength> evdoSignalStrength =
      convertEvdoSignalStrength(aCellInfoCdma.signalStrengthEvdo);
  RefPtr<nsCellInfoCdma> cellInfoCdma = new nsCellInfoCdma(
      cellIdentityCdma, cdmaSignalStrength, evdoSignalStrength);

  return cellInfoCdma;
}

RefPtr<nsCellInfoWcdma> nsRilResult::convertCellInfoWcdma(
    const CellInfoWcdma& aCellInfoWcdma) {
  RefPtr<nsCellIdentityWcdma> cellIdentityWcdma =
      convertCellIdentityWcdma(aCellInfoWcdma.cellIdentityWcdma);
  RefPtr<nsWcdmaSignalStrength> wcdmaSignalStrength =
      convertWcdmaSignalStrength(aCellInfoWcdma.signalStrengthWcdma);
  RefPtr<nsCellInfoWcdma> cellInfoWcdma =
      new nsCellInfoWcdma(cellIdentityWcdma, wcdmaSignalStrength);

  return cellInfoWcdma;
}

RefPtr<nsCellInfoLte> nsRilResult::convertCellInfoLte(
    const CellInfoLte& aCellInfoLte) {
  RefPtr<nsCellIdentityLte> cellIdentityLte =
      convertCellIdentityLte(aCellInfoLte.cellIdentityLte);
  RefPtr<nsLteSignalStrength> lteSignalStrength =
      convertLteSignalStrength(aCellInfoLte.signalStrengthLte);
  RefPtr<nsCellInfoLte> cellInfoLte =
      new nsCellInfoLte(cellIdentityLte, lteSignalStrength);

  return cellInfoLte;
}

RefPtr<nsCellInfoTdScdma> nsRilResult::convertCellInfoTdScdma(
    const CellInfoTdscdma& aCellInfoTdscdma) {
  RefPtr<nsCellIdentityTdScdma> cellIdentityTdScdma =
      convertCellIdentityTdScdma(aCellInfoTdscdma.cellIdentityTdscdma);
  RefPtr<nsTdScdmaSignalStrength> tdscdmaSignalStrength =
      convertTdScdmaSignalStrength(aCellInfoTdscdma.signalStrengthTdscdma);
  RefPtr<nsCellInfoTdScdma> cellInfoTdScdma =
      new nsCellInfoTdScdma(cellIdentityTdScdma, tdscdmaSignalStrength);

  return cellInfoTdScdma;
}

RefPtr<nsSetupDataCallResult> nsRilResult::convertDcResponse(
    const SetupDataCallResult& aDcResponse) {
  RefPtr<nsSetupDataCallResult> dcResponse = new nsSetupDataCallResult(
      convertDataCallFailCause(aDcResponse.status),
      aDcResponse.suggestedRetryTime, aDcResponse.cid, aDcResponse.active,
      NS_ConvertUTF8toUTF16(aDcResponse.type.c_str()),
      NS_ConvertUTF8toUTF16(aDcResponse.ifname.c_str()),
      NS_ConvertUTF8toUTF16(aDcResponse.addresses.c_str()),
      NS_ConvertUTF8toUTF16(aDcResponse.dnses.c_str()),
      NS_ConvertUTF8toUTF16(aDcResponse.gateways.c_str()),
      NS_ConvertUTF8toUTF16(aDcResponse.pcscf.c_str()), aDcResponse.mtu);

  return dcResponse;
}

int32_t nsRilResult::convertCellInfoType(CellInfoType type) {
  switch (type) {
    case CellInfoType::NONE:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_UNKNOW;
    case CellInfoType::GSM:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_GSM;
    case CellInfoType::CDMA:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_CDMA;
    case CellInfoType::LTE:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_LTE;
    case CellInfoType::WCDMA:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_WCDMA;
    case CellInfoType::TD_SCDMA:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_TD_SCDMA;
    default:
      return nsICellInfoType::RADIO_CELL_INFO_TYPE_UNKNOW;
  }
}

int32_t nsRilResult::convertTimeStampType(TimeStampType type) {
  switch (type) {
    case TimeStampType::UNKNOWN:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_UNKNOW;
    case TimeStampType::ANTENNA:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_ANTENNA;
    case TimeStampType::MODEM:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_MODEM;
    case TimeStampType::OEM_RIL:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_OEM_RIL;
    case TimeStampType::JAVA_RIL:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_JAVA_RIL;
    default:
      return nsITimeStampType::RADIO_TIME_STAMP_TYPE_UNKNOW;
  }
}

NS_DEFINE_NAMED_CID(RILRESULT_CID);
