/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef GONKACTIVITYMANAGERSERVICE_H
#define GONKACTIVITYMANAGERSERVICE_H

#include <binder/BinderService.h>
#include <binder/IActivityManager.h>

namespace android {
class BnActivityManager : public BnInterface<IActivityManager>
{
public:
    // NOLINTNEXTLINE(google-default-arguments)
    virtual status_t    onTransact( uint32_t code,
                                    const Parcel& data,
                                    Parcel* reply,
                                    uint32_t flags = 0);
};

} // namespace android

namespace mozilla {

/*
 * Replacement of AOSP Java ActivityManagerService.
 * From Android Pie (API 28), Google introduced some permission check
 * for some resouces, such as microphone in AudioPolicyService.
 * In AudioPolicyService, it gets the APP foreground information from Java
 * ActivityManagerService. Since we don't have Java, we need to provide our
 * C++ version service to let AOSP AudioPolicyService to use.
 */
class GonkActivityManagerService :
  public android::BinderService<GonkActivityManagerService>,
  public android::BnActivityManager
{
public:
  virtual ~GonkActivityManagerService() {}
  // From android::BinderService
  static GonkActivityManagerService* GetInstance();
  static const char *getServiceName() {
    return "activity";
  }
  static void instantiate();

  // protected:
  // IBinder* onAsBinder() override;

  // From IActivityManager
  virtual int openContentUri(const android::String16& stringUri) override;
  virtual void registerUidObserver(const android::sp<android::IUidObserver>& observer,
                                    const int32_t event,
                                    const int32_t cutpoint,
                                    const android::String16& callingPackage) override;
  virtual void unregisterUidObserver(const android::sp<android::IUidObserver>& observer) override;
  virtual bool isUidActive(const uid_t uid, const android::String16& callingPackage) override;
  virtual int32_t getUidProcessState(const uid_t uid, const android::String16& callingPackage) override;
private:
  GonkActivityManagerService(): android::BnActivityManager() {}

};

} // namespace mozilla

#endif // GONKACTIVITYMANAGERSERVICE_H
