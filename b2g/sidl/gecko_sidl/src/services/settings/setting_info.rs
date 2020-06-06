/// Events related support.
use crate::common::sidl_task::*;
use crate::services::settings::messages::SettingInfo;
use log::debug;
use nserror::{nsresult, NS_ERROR_NOT_IMPLEMENTED, NS_OK};
use nsstring::*;
use xpcom::{
    interfaces::{nsISidlEventListener, nsISupports},
    RefPtr,
};

// XPCOM implementation of nsISettingInfo, used to dispatch SettingInfo to events and observers.
#[derive(xpcom)]
#[xpimplements(nsISettingInfo)]
#[refcnt = "atomic"]
pub struct InitSettingInfoXpcom {
    name: nsString,
    value: nsString,
}

impl SettingInfoXpcom {
    pub fn new(setting: &SettingInfo) -> RefPtr<Self> {
        debug!("SettingInfoXpcom::new");

        Self::allocate(InitSettingInfoXpcom {
            name: nsString::from(&setting.name),
            value: nsString::from(setting.value.clone()),
        })
    }

    xpcom_method!(set_name => SetName(aName: *const nsAString));
    fn set_name(&self, _name: *const nsAString) -> Result<(), nsresult> {
        Err(NS_ERROR_NOT_IMPLEMENTED)
    }

    xpcom_method!(set_value => SetValue(aValue: *const nsAString));
    fn set_value(&self, _value: *const nsAString) -> Result<(), nsresult> {
        Err(NS_ERROR_NOT_IMPLEMENTED)
    }

    xpcom_method!(get_name => GetName(name: *mut nsAString));
    fn get_name(&self, name: *mut nsAString) -> Result<(), nsresult> {
        unsafe {
            (*name).assign(&self.name);
        }
        Ok(())
    }

    xpcom_method!(get_value => GetValue(value: *mut nsAString));
    fn get_value(&self, value: *mut nsAString) -> Result<(), nsresult> {
        unsafe {
            (*value).assign(&self.value);
        }
        Ok(())
    }
}

// Turns a SettingInfo into an xpcom nsISupports.
macro_rules! as_isupports {
    ($obj:ident) => {
        {
            let xpcom = SettingInfoXpcom::new(&$obj);
            // TODO: Check if instead of AddRef we can use mem::forget() like in settings_manager_construct()
            xpcom.AddRef();
            xpcom.coerce::<nsISupports>()
        }

    }
}

// Turns a SettingInfo into an xpcom nsISettingsInfo.
#[macro_export]
macro_rules! as_isettingsinfo {
    ($obj:ident) => {
        {
            let xpcom = SettingInfoXpcom::new(&$obj);
            // TODO: Check if instead of AddRef we can use mem::forget() like in settings_manager_construct()
            xpcom.AddRef();
            xpcom.coerce::<nsISettingInfo>()
        }

    }
}

sidl_event_for!(nsISidlEventListener, SettingInfo, as_isupports);
