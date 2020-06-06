/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
const { Services } = ChromeUtils.import("resource://gre/modules/Services.jsm");
const { XPCOMUtils } = ChromeUtils.import(
  "resource://gre/modules/XPCOMUtils.jsm"
);

function debug(msg) {
  console.log("B2GAboutRedirector: " + msg);
}

function netErrorURL() {
  let systemManifestURL = Services.prefs.getCharPref("b2g.system_startup_url");
  systemManifestURL = Services.io.newURI(systemManifestURL, null, null);
  let netErrorURL = Services.prefs.getCharPref("b2g.neterror.url");
  netErrorURL = Services.io.newURI(netErrorURL, null, systemManifestURL);
  return netErrorURL.spec;
}

var modules = {
  certerror: {
    uri: "chrome://b2g/content/aboutCertError.xhtml",
    privileged: true,
    hide: true
  },
  neterror: {
    uri: netErrorURL(),
    privileged: true,
    hide: true
  }
};

function B2GAboutRedirector() {}
B2GAboutRedirector.prototype = {
  QueryInterface: ChromeUtils.generateQI([Ci.nsIAboutModule]),
  classID: Components.ID("{920400b1-cf8f-4760-a9c4-441417b15134}"),

  _getModuleInfo: function (aURI) {
    try {
      let moduleName = aURI.spec.replace(/[?#].*/, "").toLowerCase().split(":")[1];
      return modules[moduleName];
    } catch (e) {
      return null;
    }
  },

  // nsIAboutModule
  getURIFlags: function(aURI) {
    let flags;
    let moduleInfo = this._getModuleInfo(aURI);
    if (moduleInfo.hide)
      flags = Ci.nsIAboutModule.HIDE_FROM_ABOUTABOUT;

    return flags | Ci.nsIAboutModule.ALLOW_SCRIPT;
  },

  newChannel: function(aURI, aLoadInfo) {
    let moduleInfo = this._getModuleInfo(aURI);

    if (!moduleInfo) {
      return null;
    }

    var ios = Cc["@mozilla.org/network/io-service;1"].
              getService(Ci.nsIIOService);

    var newURI = ios.newURI(moduleInfo.uri, null, null);

    var channel = ios.newChannelFromURIWithLoadInfo(newURI, aLoadInfo);

    if (!moduleInfo.privileged) {
      // Setting the owner to null means that we'll go through the normal
      // path in GetChannelPrincipal and create a codebase principal based
      // on the channel's originalURI
      channel.owner = null;
    }

    channel.originalURI = aURI;

    return channel;
  }
};

this.NSGetFactory = XPCOMUtils.generateNSGetFactory([B2GAboutRedirector]);
