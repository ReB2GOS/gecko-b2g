"use strict";

// A base class to use by Web Embedders, providing an ergonomic
// api over Gecko specific various hooks.
// It runs with chrome privileges in the system app.

const { XPCOMUtils } = ChromeUtils.import(
  "resource://gre/modules/XPCOMUtils.jsm"
);

XPCOMUtils.defineLazyModuleGetters(this, {
  ChromeNotifications: "resource://gre/modules/ChromeNotifications.jsm",
});

(function() {
  const { Services } = ChromeUtils.import(
    "resource://gre/modules/Services.jsm"
  );

  const { AlertsHelper } = ChromeUtils.import(
    "resource://gre/modules/AlertsHelper.jsm"
  );

  const systemAlerts = {
    resendAll: (resendCallback) => {
      ChromeNotifications.resendAllNotifications(resendCallback);
    },
  };

  // Enable logs when according to the pref value, and listen to changes.
  let webEmbedLogEnabled = Services.prefs.getBoolPref(
    "webembed.log.enabled",
    false
  );

  function updateLogStatus() {
    webEmbedLogEnabled = Services.prefs.getBoolPref(
      "webembed.log.enabled",
      false
    );
  }

  Services.prefs.addObserver("webembed.log.enabled", updateLogStatus);
  window.document.addEventListener(
    "unload",
    () => {
      Services.prefs.removeObserver("webembed.log.enabled", updateLogStatus);
    },
    { once: true }
  );

  function _webembed_log(msg) {
    webEmbedLogEnabled && console.log(`WebEmbedder: ${msg}`);
  }

  function _webembed_error(msg) {
    console.error(`WebEmbedder: ${msg}`);
  }

  function BrowserDOMWindow(embedder) {
    _webembed_log(
      `Creating BrowserDOMWindow implementing ${Ci.nsIBrowserDOMWindow}`
    );
    this.embedder = embedder;
  }

  BrowserDOMWindow.prototype = {
    QueryInterface: ChromeUtils.generateQI([Ci.nsIBrowserDOMWindow]),

    openURI(aURI, aOpener, aWhere, aFlags, aTriggeringPrincipal, aCsp) {
      _webembed_log(`BrowserDOMWindow::openURI ${aURI.spec}`);
      if (this.embedder && this.embedder.browserDomWindow) {
        return this.embedder.browserDomWindow.openURI(
          aURI,
          aOpener,
          aWhere,
          aFlags,
          aTriggeringPrincipal,
          aCsp
        );
      }
      _webembed_error("openURI NOT IMPLEMENTED");
      throw new Error("NOT IMPLEMENTED");
    },

    createContentWindow(
      aURI,
      aOpener,
      aWhere,
      aFlags,
      aTriggeringPrincipal,
      aCsp
    ) {
      _webembed_log(`BrowserDOMWindow::createContentWindow ${aURI.spec}`);
      if (this.embedder && this.embedder.browserDomWindow) {
        return this.embedder.browserDomWindow.createContentWindow(
          aURI,
          aOpener,
          aWhere,
          aFlags,
          aTriggeringPrincipal,
          aCsp
        );
      }
      _webembed_error("createContentWindow NOT IMPLEMENTED");
      throw new Error("NOT IMPLEMENTED");
    },

    openURIInFrame(aURI, aParams, aWhere, aFlags, aNextRemoteTabId, aName) {
      // We currently ignore aNextRemoteTabId on mobile.  This needs to change
      // when Fennec starts to support e10s.  Assertions will fire if this code
      // isn't fixed by then.
      //
      // We also ignore aName if it is set, as it is currently only used on the
      // e10s codepath.
      _webembed_log(`BrowserDOMWindow::openURIInFrame ${aURI.spec}`);
      if (this.embedder && this.embedder.browserDomWindow) {
        let res = this.embedder.browserDomWindow.openURIInFrame(
          aURI,
          aParams,
          aWhere,
          aFlags,
          aNextRemoteTabId,
          aName
        );
        if (res) {
          return res.frame;
        }
      }
      _webembed_error("openURIInFrame NOT IMPLEMENTED");
      throw new Error("NOT IMPLEMENTED");
    },

    createContentWindowInFrame(
      aURI,
      aParams,
      aWhere,
      aFlags,
      aNextRemoteTabId,
      aName
    ) {
      _webembed_log(
        `BrowserDOMWindow::createContentWindowInFrame ${aURI.spec}`
      );
      if (this.embedder && this.embedder.browserDomWindow) {
        let res = this.embedder.browserDomWindow.createContentWindowInFrame(
          aURI,
          aParams,
          aWhere,
          aFlags,
          aNextRemoteTabId,
          aName
        );
        if (res) {
          return res.frame;
        }
      }
      _webembed_error("createContentWindowInFrame NOT IMPLEMENTED");
      throw new Error("NOT IMPLEMENTED");
    },

    isTabContentWindow(aWindow) {
      _webembed_log(`BrowserDOMWindow::isTabContentWindow`);
      if (this.embedder && this.embedder.browserDomWindow) {
        return this.embedder.browserDomWindow.isTabContentWindow(aWindow);
      }
      return false;
    },

    canClose() {
      _webembed_log(`BrowserDOMWindow::canClose`);
      if (this.embedder && this.embedder.browserDomWindow) {
        return this.embedder.browserDomWindow.canClose();
      }
      return true;
    },
  };

  class WebEmbedder extends EventTarget {
    constructor(delegates) {
      super();

      _webembed_log(`constructor in ${window}`);

      this.browserDomWindow = delegates.windowProvider;

      this.systemAlerts = systemAlerts;

      Services.obs.addObserver(
        (/* shell_window */) => {
          this.dispatchEvent(new CustomEvent("runtime-ready"));
        },
        "shell-ready"
      );

      // Hook up the process provider implementation.
      // First make sure the service was started so it can receive the observer notification.
      let pp_service = Cc["@mozilla.org/ipc/processselector;1"].getService(
        Ci.nsIContentProcessProvider
      );
      if (!pp_service) {
        _webembed_error("No ContentProcessProvider service available!");
        return;
      }

      Services.obs.notifyObservers(
        { wrappedJSObject: delegates.processSelector },
        "web-embedder-set-process-selector"
      );

      // Notify the shell that a new embedder was created and send it the window provider.
      Services.obs.notifyObservers(
        new BrowserDOMWindow(this),
        "web-embedder-created"
      );

      Services.obs.addObserver(wrappedDetail => {
        _webembed_log("receive activity-choice");
        let detail = wrappedDetail.wrappedJSObject;
        delegates.activityChooser.choseActivity(detail).then(
          choice => {
            Services.obs.notifyObservers(
              { wrappedJSObject: choice },
              "activity-choice-result"
            );
          },
          error => {
            _webembed_log(`Error in choseActivity: ${error}`);
          }
        );
      }, "activity-choice");

      if (delegates.notifications) {
        Services.obs.notifyObservers(
          { wrappedJSObject: delegates.notifications },
          "web-embedder-notifications"
        );
      }
    }

    launchPreallocatedProcess() {
      _webembed_log(`launchPreallocatedProcess`);
      return Services.appinfo.ensureContentProcess();
    }

    isGonk() {
      const { AppConstants } = ChromeUtils.import(
        "resource://gre/modules/AppConstants.jsm"
      );
      return AppConstants.platform === "gonk";
    }
  }

  window.WebEmbedder = WebEmbedder;
})();
