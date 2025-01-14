/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef WifiCertService_h
#define WifiCertService_h

#include "nsIWifiCertService.h"
#include "nsCOMPtr.h"
//#include "nsNSSShutDown.h"
#include "nsThread.h"
#include "mozilla/dom/WifiOptionsBinding.h"

namespace mozilla {
namespace dom {

class WifiCertService final : public nsIWifiCertService
                          /*, public nsNSSShutDownObject*/
{
 public:
  NS_DECL_ISUPPORTS
  NS_DECL_NSIWIFICERTSERVICE

  static already_AddRefed<WifiCertService> FactoryCreate();
  void DispatchResult(
      const mozilla::dom::WifiCertServiceResultOptions& aOptions);

 private:
  WifiCertService();
  ~WifiCertService();
  // virtual void virtualDestroyNSSReference() {};
  nsCOMPtr<nsIWifiEventListener> mListener;
};

}  // namespace dom
}  // namespace mozilla

#endif  // WifiCertService_h
