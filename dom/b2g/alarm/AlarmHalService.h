/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_alarm_AlarmHalService_h
#define mozilla_dom_alarm_AlarmHalService_h

#include "base/basictypes.h"
#include "mozilla/ClearOnShutdown.h"
#include "mozilla/StaticPtr.h"
#include "mozilla/Hal.h"
#include "mozilla/Services.h"
#include "nsIAlarmHalService.h"
#include "nsIObserver.h"
#include "nsIObserverService.h"

namespace mozilla {
namespace dom {
namespace alarm {

typedef Observer<void_t> AlarmObserver;
// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
typedef Observer<hal::SystemTimezoneChangeInformation>
    SystemTimezoneChangeObserver;

class AlarmHalService : public nsIAlarmHalService,
                        public AlarmObserver,
                        public SystemTimezoneChangeObserver
#else
class AlarmHalService : public nsIAlarmHalService,
                        public AlarmObserver
#endif

{
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIALARMHALSERVICE

  AlarmHalService() : mAlarmEnabled(false) {}

  void Init();

  static already_AddRefed<AlarmHalService> GetInstance();

  // Implementing hal::AlarmObserver
  void Notify(const void_t& aVoid) override;

// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
  // Implementing hal::SystemTimezoneChangeObserver
  void Notify(const hal::SystemTimezoneChangeInformation&
                  aSystemTimezoneChangeInfo) override;
#endif

 private:
  virtual ~AlarmHalService();

  bool mAlarmEnabled;
  static StaticRefPtr<AlarmHalService> sSingleton;

  nsCOMPtr<nsIAlarmFiredCb> mAlarmFiredCb;
// TODO: hal timezone change
#ifdef HAL_TIMEZONE_CHANGE
  nsCOMPtr<nsITimezoneChangedCb> mTimezoneChangedCb;
#endif
};

}  // namespace alarm
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_alarm_AlarmHalService_h
