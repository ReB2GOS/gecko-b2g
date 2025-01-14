/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FallbackScreenConfiguration.h"

namespace mozilla {
namespace hal_impl {

void EnableScreenConfigurationNotifications() {}

void DisableScreenConfigurationNotifications() {}

void GetCurrentScreenConfiguration(
    hal::ScreenConfiguration* aScreenConfiguration) {
  fallback::GetCurrentScreenConfiguration(aScreenConfiguration);
}

bool LockScreenOrientation(const ScreenOrientation& aOrientation) {
  return false;
}

void UnlockScreenOrientation() {}

#if !defined MOZ_WIDGET_GONK
bool
GetScreenEnabled()
{
  return true;
}

void
SetScreenEnabled(bool)
{

}
#endif

}  // namespace hal_impl
}  // namespace mozilla
