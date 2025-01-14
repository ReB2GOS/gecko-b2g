/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_network_MobileConnectionArray_h__
#define mozilla_dom_network_MobileConnectionArray_h__

#include "mozilla/dom/MobileConnection.h"
#include "nsWrapperCache.h"

namespace mozilla {
namespace dom {

class MobileConnectionArray final : public nsISupports
                                  , public nsWrapperCache
{
  nsCOMPtr<nsIGlobalObject> mOwner;
public:
  NS_DECL_CYCLE_COLLECTING_ISUPPORTS
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_CLASS(MobileConnectionArray)

  explicit MobileConnectionArray(nsIGlobalObject* aGlobal);

  nsIGlobalObject*
  GetParentObject() const;

  // WrapperCache
  virtual JSObject*
  WrapObject(JSContext* aCx, JS::Handle<JSObject*> aGivenProto) override;

  //  WebIDL
  MobileConnection*
  Item(uint32_t aIndex);

  uint32_t
  Length();

  MobileConnection*
  IndexedGetter(uint32_t aIndex, bool& aFound);

private:
  ~MobileConnectionArray();

  bool mLengthInitialized;

  nsTArray<RefPtr<MobileConnection>> mMobileConnections;
};

} // namespace dom
} // namespace mozilla

#endif // mozilla_dom_network_MobileConnectionArray_h__
