/* -*- Mode: IDL; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
 * The capabilities of Wifi. These are guaranteed not to change over the
 * lifetime of that particular instance.
 */
enum WifiSecurityMethod {
  "OPEN",
  "WEP",
  "WPA-PSK",
  "WPA-EAP",
  "WAPI-PSK",
  "WAPI-CERT"
};

enum WifiWpaMethod {
  "SIM",
  "AKA",
  "AKA'",
  "PEAP",
  "TTLS",
  "TLS"
};

enum WifiWpaPhase2Method {
  "PAP",
  "MSCHAP",
  "MSCHAPV2",
  "GTC"
};

enum WifiWpaCertificate {
  "SERVER",
  "USER"
};

[JSImplementation="@mozilla.org/wificapabilities;1",
 Func="B2G::HasWifiManagerSupport",
 Exposed=Window]
interface WifiCapabilities {
  //[Constant, Cached] readonly attribute sequence<WifiSecurityMethod> security;
  //[Constant, Cached] readonly attribute sequence<WifiWpaMethod> eapMethod;
  //[Constant, Cached] readonly attribute sequence<WifiWpaPhase2Method> eapPhase2;
  //[Constant, Cached] readonly attribute sequence<WifiWpaCertificate> certificate;

  //jsonifier;
};

