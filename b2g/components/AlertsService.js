/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

const Ci = Components.interfaces;
const Cu = Components.utils;
const Cc = Components.classes;

Cu.import("resource://gre/modules/XPCOMUtils.jsm");
Cu.import("resource://gre/modules/Services.jsm");

XPCOMUtils.defineLazyServiceGetter(this, "uuidGenerator",
                                   "@mozilla.org/uuid-generator;1",
                                   "nsIUUIDGenerator");

XPCOMUtils.defineLazyServiceGetter(this, "notificationStorage",
                                   "@mozilla.org/notificationStorage;1",
                                   "nsINotificationStorage");

XPCOMUtils.defineLazyServiceGetter(this, "serviceWorkerManager",
                                   "@mozilla.org/serviceworkers/manager;1",
                                   "nsIServiceWorkerManager");

XPCOMUtils.defineLazyServiceGetter(this, "appsService",
                                   "@mozilla.org/AppsService;1",
                                   "nsIAppsService");

XPCOMUtils.defineLazyGetter(this, "cpmm", function() {
  return Cc["@mozilla.org/childprocessmessagemanager;1"]
           .getService(Ci.nsIMessageSender);
});

function debug(str) {
  dump("=*= AlertsService.js : " + str + "\n");
}

// -----------------------------------------------------------------------
// Alerts Service
// -----------------------------------------------------------------------

const kMessageAppNotificationSend    = "app-notification-send";
const kMessageAppNotificationReturn  = "app-notification-return";
const kMessageAlertNotificationSend  = "alert-notification-send";
const kMessageAlertNotificationClose = "alert-notification-close";

const kTopicAlertShow          = "alertshow";
const kTopicAlertFinished      = "alertfinished";
const kTopicAlertClickCallback = "alertclickcallback";

function AlertsService() {
  Services.obs.addObserver(this, "xpcom-shutdown", false);
  cpmm.addMessageListener(kMessageAppNotificationReturn, this);
}

AlertsService.prototype = {
  classID: Components.ID("{fe33c107-82a4-41d6-8c64-5353267e04c9}"),
  QueryInterface: ChromeUtils.generateQI([Ci.nsIAlertsService,
                                         Ci.nsIAppNotificationService,
                                         Ci.nsIObserver]),

  observe: function(aSubject, aTopic, aData) {
    switch (aTopic) {
      case "xpcom-shutdown":
        Services.obs.removeObserver(this, "xpcom-shutdown");
        cpmm.removeMessageListener(kMessageAppNotificationReturn, this);
        break;
    }
  },

  // nsIAlertsService
  showAlert: function(aAlert, aAlertListener) {
    if (!aAlert) {
      return;
    }
    cpmm.sendAsyncMessage(kMessageAlertNotificationSend, {
      imageURL: aAlert.imageURL,
      title: aAlert.title,
      text: aAlert.text,
      clickable: aAlert.textClickable,
      cookie: aAlert.cookie,
      listener: aAlertListener,
      id: aAlert.name,
      dir: aAlert.dir,
      lang: aAlert.lang,
      dataStr: aAlert.data,
      inPrivateBrowsing: aAlert.inPrivateBrowsing
    });
  },

  showAlertNotification: function(aImageUrl, aTitle, aText, aTextClickable,
                                  aCookie, aAlertListener, aName, aBidi,
                                  aLang, aDataStr, aPrincipal,
                                  aInPrivateBrowsing) {
    let alert = Cc["@mozilla.org/alert-notification;1"].
      createInstance(Ci.nsIAlertNotification);

    alert.init(aName, aImageUrl, aTitle, aText, aTextClickable, aCookie,
               aBidi, aLang, aDataStr, aPrincipal, aInPrivateBrowsing);

    this.showAlert(alert, aAlertListener);
  },

  closeAlert: function(aName) {
    cpmm.sendAsyncMessage(kMessageAlertNotificationClose, {
      name: aName
    });
  },

  // nsIAppNotificationService
  showAppNotification: function(aImageURL, aTitle, aText, aAlertListener,
                                aDetails) {
    let uid = (aDetails.id == "") ?
          "app-notif-" + uuidGenerator.generateUUID() : aDetails.id;

    this._listeners[uid] = {
      observer: aAlertListener,
      title: aTitle,
      text: aText,
      manifestURL: aDetails.manifestURL,
      imageURL: aImageURL,
      lang: aDetails.lang || undefined,
      id: aDetails.id || undefined,
      dbId: aDetails.dbId || undefined,
      dir: aDetails.dir || undefined,
      tag: aDetails.tag || undefined,
      timestamp: aDetails.timestamp || undefined,
      dataObj: aDetails.data || undefined,
      mozbehavior: aDetails.mozbehavior,
      // TODO: Interactive Notification feature
      serviceWorkerRegistrationScope: aDetails.serviceWorkerRegistrationScope
    };

    cpmm.sendAsyncMessage(kMessageAppNotificationSend, {
      imageURL: aImageURL,
      title: aTitle,
      text: aText,
      uid: uid,
      details: aDetails
    });
  },

  // AlertsService.js custom implementation
  _listeners: [],

  receiveMessage: function(aMessage) {
    let data = aMessage.data;
    let listener = this._listeners[data.uid];
    if (aMessage.name !== kMessageAppNotificationReturn || !listener) {
      return;
    }

    let topic = data.topic;

    try {
      listener.observer.observe(null, topic, null);
    } catch (e) {
      // The non-empty serviceWorkerRegistrationScope means the notification
      // is issued by service worker, so deal with this listener
      // via serviceWorkerManager
      if (listener.serviceWorkerRegistrationScope.length &&
        topic !== kTopicAlertShow) {
        const scope = listener.serviceWorkerRegistrationScope;
        const originAttr = ChromeUtils.createOriginAttributesFromOrigin(scope);
        const originSuffix = ChromeUtils.originAttributesToSuffix(originAttr);
        let eventName;

        if (topic == kTopicAlertClickCallback) {
          eventName = "notificationclick";
        } else if (topic === kTopicAlertFinished) {
          eventName = "notificationclose";
        }

        if (eventName == "notificationclick") {
          serviceWorkerManager.sendNotificationClickEvent(
            originSuffix,
            scope,
            listener.dbId,
            listener.title,
            listener.dir,
            listener.lang,
            listener.text,
            listener.tag,
            listener.imageURL,
            listener.dataObj || undefined,
            listener.mozbehavior
          );
        }
      }
      if (topic === kTopicAlertFinished && listener.dbId) {
        notificationStorage.delete(listener.manifestURL, listener.dbId);
      }
    }

    // we're done with this notification
    if (topic === kTopicAlertFinished) {
      delete this._listeners[data.uid];
    }
  }
};

this.NSGetFactory = XPCOMUtils.generateNSGetFactory([AlertsService]);
