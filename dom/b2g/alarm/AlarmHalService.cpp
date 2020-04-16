/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "AlarmHalService.h"

namespace mozilla {
namespace dom {
namespace alarm {

using namespace hal;

NS_IMPL_ISUPPORTS(AlarmHalService, nsIAlarmHalService)

void AlarmHalService::Init() {
  mAlarmEnabled = RegisterTheOneAlarmObserver(this);
  if (!mAlarmEnabled) {
    return;
  }
// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
  RegisterSystemTimezoneChangeObserver(this);
#endif
}

/* virtual */ AlarmHalService::~AlarmHalService() {
  if (mAlarmEnabled) {
    UnregisterTheOneAlarmObserver();
// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
    UnregisterSystemTimezoneChangeObserver(this);
#endif
  }
}

/* static */ StaticRefPtr<AlarmHalService> AlarmHalService::sSingleton;

/* static */ already_AddRefed<AlarmHalService> AlarmHalService::GetInstance() {
  if (!sSingleton) {
    sSingleton = new AlarmHalService();
    sSingleton->Init();
    ClearOnShutdown(&sSingleton);
  }

  RefPtr<AlarmHalService> service = sSingleton.get();
  return service.forget();
}

NS_IMETHODIMP
AlarmHalService::SetAlarm(int32_t aSeconds, int32_t aNanoseconds,
                          bool* aStatus) {
  if (!mAlarmEnabled) {
    return NS_ERROR_FAILURE;
  }

  bool status = hal::SetAlarm(aSeconds, aNanoseconds);
  if (status) {
    *aStatus = status;
    return NS_OK;
  } else {
    return NS_ERROR_FAILURE;
  }
}

NS_IMETHODIMP
AlarmHalService::SetAlarmFiredCb(nsIAlarmFiredCb* aAlarmFiredCb) {
  mAlarmFiredCb = aAlarmFiredCb;
  return NS_OK;
}

// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
NS_IMETHODIMP
AlarmHalService::SetTimezoneChangedCb(
    nsITimezoneChangedCb* aTimeZoneChangedCb) {
  mTimezoneChangedCb = aTimeZoneChangedCb;
  return NS_OK;
}
#endif

void AlarmHalService::Notify(const void_t& aVoid) {
  if (!mAlarmFiredCb) {
    return;
  }
  mAlarmFiredCb->OnAlarmFired();
}

// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
void AlarmHalService::Notify(
    const SystemTimezoneChangeInformation& aSystemTimezoneChangeInfo) {
  if (!mTimezoneChangedCb) {
    return;
  }
  mTimezoneChangedCb->OnTimezoneChanged(
      aSystemTimezoneChangeInfo.newTimezoneOffsetMinutes());
}

#endif
}  // namespace alarm
}  // namespace dom
}  // namespace mozilla
