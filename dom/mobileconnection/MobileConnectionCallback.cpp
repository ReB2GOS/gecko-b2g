/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
* License, v. 2.0. If a copy of the MPL was not distributed with this file,
* You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "MobileConnectionCallback.h"

#include "mozilla/dom/MobileNetworkInfo.h"
#include "mozilla/dom/DOMMobileNetworkInfo.h"
#include "mozilla/dom/MobileConnectionBinding.h"
#include "mozilla/dom/ToJSValue.h"
#include "nsJSUtils.h"
#include "nsServiceManagerUtils.h"

namespace mozilla {
namespace dom {
namespace mobileconnection {

#define CONVERT_ENUM_TO_STRING(_enumType, _enum, _string)               \
{                                                                       \
  uint32_t index = uint32_t(_enum);                                     \
  _string.AssignASCII(_enumType##Values::strings[index].value,          \
                      _enumType##Values::strings[index].length);        \
}

NS_IMPL_ISUPPORTS(MobileConnectionCallback, nsIMobileConnectionCallback)

MobileConnectionCallback::MobileConnectionCallback(nsPIDOMWindowInner* aWindow,
                                                   DOMRequest* aRequest)
  : mWindow(aWindow)
  , mRequest(aRequest)
{
}

/**
 * Notify Success.
 */
nsresult
MobileConnectionCallback::NotifySuccess(JS::Handle<JS::Value> aResult)
{
  nsCOMPtr<nsIDOMRequestService> rs = do_GetService(DOMREQUEST_SERVICE_CONTRACTID);
  NS_ENSURE_TRUE(rs, NS_ERROR_FAILURE);

  return rs->FireSuccessAsync(mRequest, aResult);
}

/**
 * Notify Success with string.
 */
nsresult
MobileConnectionCallback::NotifySuccessWithString(const nsAString& aResult)
{
  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);

  if (!ToJSValue(cx, aResult, &jsResult)) {
    jsapi.ClearException();
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
}

// nsIMobileConnectionCallback

NS_IMETHODIMP
MobileConnectionCallback::NotifySuccess()
{
  return NotifySuccess(JS::UndefinedHandleValue);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifySuccessWithBoolean(bool aResult)
{
  return aResult ? NotifySuccess(JS::TrueHandleValue)
                 : NotifySuccess(JS::FalseHandleValue);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetNetworksSuccess(uint32_t aCount,
                                                   nsIMobileNetworkInfo** aNetworks)
{

  nsTArray<RefPtr<DOMMobileNetworkInfo>> results;
  for (uint32_t i = 0; i < aCount; i++)
  {
    RefPtr<DOMMobileNetworkInfo> networkInfo = new DOMMobileNetworkInfo(mWindow);
    networkInfo->Update(aNetworks[i]);
    results.AppendElement(networkInfo);
  }

  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);

  if (!ToJSValue(cx, results, &jsResult)) {
    jsapi.ClearException();
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetCallForwardingSuccess(uint32_t aCount,
                                                         nsIMobileCallForwardingOptions** aResults)
{
  nsTArray<CallForwardingOptions> results;
  for (uint32_t i = 0; i < aCount; i++)
  {
    CallForwardingOptions result;
    int16_t pShort;
    nsString pString;
    bool pBool;

    aResults[i]->GetActive(&pBool);
    result.mActive.SetValue(pBool);

    aResults[i]->GetAction(&pShort);
    if (pShort != nsIMobileConnection::CALL_FORWARD_ACTION_UNKNOWN) {
      result.mAction = pShort;
    }

    aResults[i]->GetReason(&pShort);
    if (pShort != nsIMobileConnection::CALL_FORWARD_REASON_UNKNOWN) {
      result.mReason = pShort;
    }

    aResults[i]->GetNumber(pString);
    result.mNumber = pString.get();

    aResults[i]->GetTimeSeconds(&pShort);
    if (pShort >= 0) {
      result.mTimeSeconds = pShort;
    }

    aResults[i]->GetServiceClass(&pShort);
    if (pShort != nsIMobileConnection::ICC_SERVICE_CLASS_NONE) {
      result.mServiceClass = pShort;
    }

    results.AppendElement(result);
  }

  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);

  if (!ToJSValue(cx, results, &jsResult)) {
    jsapi.ClearException();
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetCallBarringSuccess(uint16_t aProgram,
                                                      bool aEnabled,
                                                      uint16_t aServiceClass)
{
  CallBarringOptions result;
  result.mProgram.SetValue(aProgram);
  result.mEnabled.SetValue(aEnabled);
  result.mServiceClass.SetValue(aServiceClass);

  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);
  if (!ToJSValue(cx, result, &jsResult)) {
    jsapi.ClearException();
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetCallWaitingSuccess(uint16_t aServiceClass)
{
  return (aServiceClass & nsIMobileConnection::ICC_SERVICE_CLASS_VOICE)
           ? NotifySuccess(JS::TrueHandleValue)
           : NotifySuccess(JS::FalseHandleValue);
}

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetClirStatusSuccess(uint16_t aN, uint16_t aM)
{
  ClirStatus result;
  result.mN = aN;
  result.mM = aM;

  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);
  if (!ToJSValue(cx, result, &jsResult)) {
    jsapi.ClearException();
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
};

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetPreferredNetworkTypeSuccess(int32_t aType)
{
  MOZ_ASSERT(aType < static_cast<int32_t>(MobilePreferredNetworkType::EndGuard_));
  MobilePreferredNetworkType type = static_cast<MobilePreferredNetworkType>(aType);

  nsAutoString typeString;
  CONVERT_ENUM_TO_STRING(MobilePreferredNetworkType, type, typeString);

  return NotifySuccessWithString(typeString);
};

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetRoamingPreferenceSuccess(int32_t aMode)
{
  MOZ_ASSERT(aMode < static_cast<int32_t>(MobileRoamingMode::EndGuard_));
  MobileRoamingMode mode = static_cast<MobileRoamingMode>(aMode);

  nsAutoString modeString;
  CONVERT_ENUM_TO_STRING(MobileRoamingMode, mode, modeString);

  return NotifySuccessWithString(modeString);
};

NS_IMETHODIMP
MobileConnectionCallback::NotifyGetDeviceIdentitiesRequestSuccess(
  nsIMobileDeviceIdentities* aResult)
{
  nsString pString;
  MobileDeviceIds result;

  RefPtr<DOMMobileDeviceIdentities> DeviceIdentities = new DOMMobileDeviceIdentities(mWindow);
  DeviceIdentities->Update(aResult);

  DeviceIdentities->GetIdentities()->GetImei(pString);
  result.mImei = pString;

  DeviceIdentities->GetIdentities()->GetImeisv(pString);
  result.mImeisv = pString;

  DeviceIdentities->GetIdentities()->GetEsn(pString);
  result.mEsn = pString;

  DeviceIdentities->GetIdentities()->GetMeid(pString);
  result.mMeid = pString;

  AutoJSAPI jsapi;
  if (NS_WARN_IF(!jsapi.Init(mWindow))) {
    return NS_ERROR_FAILURE;
  }

  JSContext* cx = jsapi.cx();
  JS::Rooted<JS::Value> jsResult(cx);
  if (!ToJSValue(cx, result, &jsResult)) {
    JS_ClearPendingException(cx);
    return NS_ERROR_DOM_TYPE_MISMATCH_ERR;
  }

  return NotifySuccess(jsResult);
};

NS_IMETHODIMP
MobileConnectionCallback::NotifyError(const nsAString& aName)
{
  nsCOMPtr<nsIDOMRequestService> rs = do_GetService(DOMREQUEST_SERVICE_CONTRACTID);
  NS_ENSURE_TRUE(rs, NS_ERROR_FAILURE);
  return rs->FireErrorAsync(mRequest, aName);
}

} // namespace mobileconnection
} // namespace dom
} // namespace mozilla
