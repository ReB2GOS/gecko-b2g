/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_WebActivityRequestHandler_h
#define mozilla_dom_WebActivityRequestHandler_h

#include "mozilla/AlreadyAddRefed.h"
#include "mozilla/ErrorResult.h"
#include "mozilla/dom/BindingDeclarations.h"
#include "nsWrapperCache.h"

class nsIGlobalObject;

namespace mozilla {
namespace dom {

struct WebActivityOptions;

class WebActivityRequestHandler final : public nsWrapperCache

{
 public:
  NS_INLINE_DECL_CYCLE_COLLECTING_NATIVE_REFCOUNTING(WebActivityRequestHandler)
  NS_DECL_CYCLE_COLLECTION_SCRIPT_HOLDER_NATIVE_CLASS(WebActivityRequestHandler)

  nsISupports* GetParentObject() const { return nullptr; }

  JSObject* WrapObject(JSContext* aCx,
                       JS::Handle<JSObject*> aGivenProto) override;

  static already_AddRefed<WebActivityRequestHandler> Create(
      nsIGlobalObject* aGlobal, const nsAString& aMessage);
  // WebIDL methods:

  void PostResult(JSContext* aCx, JS::Handle<JS::Value> aResult,
                  ErrorResult& aRv);

  void PostError(const nsAString& aError);

  void GetSource(JSContext* aCx, WebActivityOptions& aResult, ErrorResult& aRv);

 private:
  WebActivityRequestHandler(const nsAString& aMessage, const nsAString& aId,
                            bool aReturnValue);
  ~WebActivityRequestHandler();

  nsString mMessage;
  nsString mActivityId;
  bool mReturnValue;
};
}  // namespace dom
}  // namespace mozilla

#endif  // mozilla_dom_WebActivityRequestHandler_h
