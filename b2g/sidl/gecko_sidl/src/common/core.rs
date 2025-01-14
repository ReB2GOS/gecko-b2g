use crate::common::traits::EventIds;
use serde::{Deserialize, Serialize};

/// The different kind of messages that a service/object can
/// process:
/// - requests
/// - responses
/// - events
#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub enum BaseMessageKind {
    Request(u64),
    Response(u64),
    Event,
}

/// The high-level message, encapsulating the metadata
/// used to route the message (service, object)
/// alongside the content of the actual message.
#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct BaseMessage {
    pub service: u32,
    pub object: u32,
    pub kind: BaseMessageKind,
    pub content: Vec<u8>,
}

impl BaseMessage {
    pub fn empty_from(msg: &BaseMessage) -> Self {
        BaseMessage {
            service: msg.service,
            object: msg.object,
            kind: msg.kind.clone(),
            content: vec![],
        }
    }

    pub fn request(&self) -> u64 {
        match self.kind {
            BaseMessageKind::Request(value) => value,
            _ => panic!("Expected request, got {:?}", self.kind),
        }
    }

    pub fn response(&self) -> u64 {
        match self.kind {
            BaseMessageKind::Response(value) => value,
            _ => panic!("Expected response, got {:?}", self.kind),
        }
    }
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct GetServiceRequest {
    pub name: String,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct GetServiceResponse {
    pub success: bool,
    pub service: u32,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct HasServiceRequest {
    pub name: String,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct HasServiceResponse {
    pub success: bool,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct ReleaseObjectRequest {
    pub service: u32,
    pub object: u32,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct ReleaseObjectResponse {
    pub success: bool,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct EnableEventListenerRequest {
    pub service: u32,
    pub object: u32,
    pub event: u32,
}

impl EventIds for EnableEventListenerRequest {
    fn ids(&self) -> (u32, u32, u32) {
        (self.service, self.object, self.event)
    }
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct EnableEventListenerResponse {
    pub success: bool,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct DisableEventListenerRequest {
    pub service: u32,
    pub object: u32,
    pub event: u32,
}

impl EventIds for DisableEventListenerRequest {
    fn ids(&self) -> (u32, u32, u32) {
        (self.service, self.object, self.event)
    }
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub struct DisableEventListenerResponse {
    pub success: bool,
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub enum CoreRequest {
    GetService(GetServiceRequest),
    HasService(HasServiceRequest),
    ReleaseObject(ReleaseObjectRequest),
    EnableEvent(EnableEventListenerRequest),
    DisableEvent(DisableEventListenerRequest),
}

#[derive(Clone, Debug, PartialEq, Serialize, Deserialize)]
pub enum CoreResponse {
    GetService(GetServiceResponse),
    HasService(HasServiceResponse),
    ReleaseObject(ReleaseObjectResponse),
    EnableEvent(EnableEventListenerResponse),
    DisableEvent(DisableEventListenerResponse),
}
