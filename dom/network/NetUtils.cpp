/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "NetUtils.h"
#include <dlfcn.h>
#include <errno.h>
#include "prinit.h"
#include "mozilla/Assertions.h"
#include "nsDebug.h"
#include "SystemProperty.h"

using mozilla::system::Property;

static void* sNetUtilsLib;
static PRCallOnceType sInitNetUtilsLib;

static PRStatus InitNetUtilsLib() {
  sNetUtilsLib = dlopen("libnetutils.so", RTLD_LAZY);
  // We might fail to open the hardware lib. That's OK.
  return PR_SUCCESS;
}

static void* GetNetUtilsLibHandle() {
  PR_CallOnce(&sInitNetUtilsLib, InitNetUtilsLib);
  return sNetUtilsLib;
}

// static
void* NetUtils::GetSharedLibrary() {
  void* netLib = GetNetUtilsLibHandle();
  if (!netLib) {
    NS_WARNING("No libnetutils.so");
  }
  return netLib;
}

// static
int32_t NetUtils::SdkVersion() {
  char propVersion[Property::VALUE_MAX_LENGTH];
  Property::Get("ro.build.version.sdk", propVersion, "0");
  int32_t version = strtol(propVersion, nullptr, 10);
  return version;
}

DEFINE_DLFUNC(ifc_enable, int32_t, const char*)
DEFINE_DLFUNC(ifc_disable, int32_t, const char*)
DEFINE_DLFUNC(ifc_configure, int32_t, const char*, in_addr_t, uint32_t,
              in_addr_t, in_addr_t, in_addr_t)
DEFINE_DLFUNC(ifc_reset_connections, int32_t, const char*, const int32_t)
DEFINE_DLFUNC(ifc_set_default_route, int32_t, const char*, in_addr_t)
DEFINE_DLFUNC(ifc_add_route, int32_t, const char*, const char*, uint32_t,
              const char*)
DEFINE_DLFUNC(ifc_remove_route, int32_t, const char*, const char*, uint32_t,
              const char*)
DEFINE_DLFUNC(ifc_remove_host_routes, int32_t, const char*)
DEFINE_DLFUNC(ifc_remove_default_route, int32_t, const char*)
DEFINE_DLFUNC(dhcp_stop, int32_t, const char*)

NetUtils::NetUtils() { mDhcpUtils.reset(new DhcpUtils()); }

int32_t NetUtils::do_ifc_enable(const char* ifname) {
  USE_DLFUNC(ifc_enable)
  return ifc_enable(ifname);
}

int32_t NetUtils::do_ifc_disable(const char* ifname) {
  USE_DLFUNC(ifc_disable)
  return ifc_disable(ifname);
}

int32_t NetUtils::do_ifc_configure(const char* ifname, in_addr_t address,
                                   uint32_t prefixLength, in_addr_t gateway,
                                   in_addr_t dns1, in_addr_t dns2) {
  USE_DLFUNC(ifc_configure)
  int32_t ret =
      ifc_configure(ifname, address, prefixLength, gateway, dns1, dns2);
  return ret;
}

int32_t NetUtils::do_ifc_reset_connections(const char* ifname,
                                           const int32_t resetMask) {
  USE_DLFUNC(ifc_reset_connections)
  return ifc_reset_connections(ifname, resetMask);
}

int32_t NetUtils::do_ifc_set_default_route(const char* ifname,
                                           in_addr_t gateway) {
  USE_DLFUNC(ifc_set_default_route)
  return ifc_set_default_route(ifname, gateway);
}

int32_t NetUtils::do_ifc_add_route(const char* ifname, const char* dst,
                                   uint32_t prefixLength, const char* gateway) {
  USE_DLFUNC(ifc_add_route)
  return ifc_add_route(ifname, dst, prefixLength, gateway);
}

int32_t NetUtils::do_ifc_remove_route(const char* ifname, const char* dst,
                                      uint32_t prefixLength,
                                      const char* gateway) {
  USE_DLFUNC(ifc_remove_route)
  return ifc_remove_route(ifname, dst, prefixLength, gateway);
}

int32_t NetUtils::do_ifc_remove_host_routes(const char* ifname) {
  USE_DLFUNC(ifc_remove_host_routes)
  return ifc_remove_host_routes(ifname);
}

int32_t NetUtils::do_ifc_remove_default_route(const char* ifname) {
  USE_DLFUNC(ifc_remove_default_route)
  return ifc_remove_default_route(ifname);
}

int32_t NetUtils::do_dhcp_stop(const char* ifname) {
  return mDhcpUtils->DhcpStop(ifname);
}

int32_t NetUtils::do_dhcp_do_request(const char* ifname, char* ipaddr,
                                     char* gateway, uint32_t* prefixLength,
                                     char* dns1, char* dns2, char* server,
                                     uint32_t* lease, char* vendorinfo) {
  int32_t ret = -1;

  ret = mDhcpUtils->DhcpStart(ifname);
  if (ret != 0) return ret;

  char* dns[3] = {dns1, dns2, nullptr};
  char domains[Property::VALUE_MAX_LENGTH];
  char mtu[Property::VALUE_MAX_LENGTH];
  ret = mDhcpUtils->GetDhcpResults(ifname, ipaddr, gateway, prefixLength, dns,
                                   server, lease, vendorinfo, domains, mtu);
  return ret;
}
