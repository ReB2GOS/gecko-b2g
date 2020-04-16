/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

[Exposed=(Window,Worker)]
interface B2G {
  // objects implementing this interface also implement the interfaces given
  // below
};

partial interface B2G {
  [Throws, Exposed=(Window,Worker), Pref="dom.alarm.enabled"]
  readonly attribute AlarmManager alarmManager;
};

partial interface B2G {
  [Throws, Exposed=Window]
  readonly attribute TetheringManager tetheringManager;
};

#ifdef MOZ_B2G_RIL
[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.mobileconnection.enabled"]
  readonly attribute MozMobileConnectionArray mobileConnections;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.telephony.enabled"]
  readonly attribute Telephony telephony;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.icc.enabled"]
  readonly attribute MozIccManager? iccManager;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.datacall.enabled"]
  readonly attribute DataCallManager? dataCallManager;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.subsidylock.enabled"]
  readonly attribute SubsidyLockManager? subsidyLockManager;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.voicemail.enabled"]
  readonly attribute MozVoicemail voicemail;
};

[Exposed=Window]
partial interface B2G {
  [Throws, Pref="dom.cellbroadcast.enabled"]
  readonly attribute CellBroadcast cellBroadcast;
};
#endif //MOZ_B2G_RIL

#ifdef HAS_KOOST_MODULES
[Exposed=(Window,Worker)]
partial interface B2G {
  [Throws]
  readonly attribute ExternalAPI externalapi;
};
#endif

#ifdef MOZ_B2G_BT
[Exposed=Window]
partial interface B2G {
  [Throws]
  readonly attribute BluetoothManager bluetooth;
};
#endif // MOZ_B2G_BT

#ifndef DISABLE_WIFI
partial interface B2G {
  [Throws, Func="B2G::HasWifiManagerSupport", Exposed=Window]
  readonly attribute WifiManager wifiManager;
};
#endif // DISABLE_WIFI

partial interface B2G {
  [Throws, Pref="dom.downloads.enabled", Exposed=Window]
  readonly attribute DownloadManager downloadManager;
};
