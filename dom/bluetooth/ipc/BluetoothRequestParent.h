/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef mozilla_dom_bluetooth_ipc_BluetoothRequestParent_h
#define mozilla_dom_bluetooth_ipc_BluetoothRequestParent_h

#include "mozilla/dom/bluetooth/PBluetoothParent.h"
#include "mozilla/dom/bluetooth/PBluetoothRequestParent.h"

template <class T>
class nsRevocableEventPtr;

BEGIN_BLUETOOTH_NAMESPACE

class BluetoothService;

/*******************************************************************************
 * BluetoothAdapterRequestParent
 ******************************************************************************/

class BluetoothRequestParent : public PBluetoothRequestParent {
  class ReplyRunnable;
  friend class BluetoothParent;

  friend class ReplyRunnable;

  RefPtr<BluetoothService> mService;
  nsRevocableEventPtr<ReplyRunnable> mReplyRunnable;

#ifdef DEBUG
  Request::Type mRequestType;
#endif

 protected:
  explicit BluetoothRequestParent(BluetoothService* aService);
  virtual ~BluetoothRequestParent();

  virtual void ActorDestroy(ActorDestroyReason aWhy) override;

  void RequestComplete();

  bool DoRequest(const GetAdaptersRequest& aRequest);

  bool DoRequest(const StartBluetoothRequest& aRequest);

  bool DoRequest(const StopBluetoothRequest& aRequest);

  bool DoRequest(const SetPropertyRequest& aRequest);

  bool DoRequest(const GetPropertyRequest& aRequest);

  bool DoRequest(const StartDiscoveryRequest& aRequest);

  bool DoRequest(const StopDiscoveryRequest& aRequest);

  bool DoRequest(const StartLeScanRequest& aRequest);

  bool DoRequest(const StopLeScanRequest& aRequest);

  bool DoRequest(const StartAdvertisingRequest& aRequest);

  bool DoRequest(const StopAdvertisingRequest& aRequest);

  bool DoRequest(const PairRequest& aRequest);

  bool DoRequest(const UnpairRequest& aRequest);

  bool DoRequest(const PairedDevicePropertiesRequest& aRequest);

  bool DoRequest(const ConnectedDevicePropertiesRequest& aRequest);

  bool DoRequest(const FetchUuidsRequest& aRequest);

  bool DoRequest(const PinReplyRequest& aRequest);

  bool DoRequest(const SspReplyRequest& aRequest);

  bool DoRequest(const ConnectRequest& aRequest);

  bool DoRequest(const DisconnectRequest& aRequest);

  bool DoRequest(const AcceptConnectionRequest& aRequest);

  bool DoRequest(const RejectConnectionRequest& aRequest);

  bool DoRequest(const SendFileRequest& aRequest);

  bool DoRequest(const StopSendingFileRequest& aRequest);

  bool DoRequest(const ConfirmReceivingFileRequest& aRequest);

  bool DoRequest(const DenyReceivingFileRequest& aRequest);

  bool DoRequest(const ConnectScoRequest& aRequest);

  bool DoRequest(const DisconnectScoRequest& aRequest);

  bool DoRequest(const IsScoConnectedRequest& aRequest);

  bool DoRequest(const SetObexPasswordRequest& aRequest);

  bool DoRequest(const RejectObexAuthRequest& aRequest);

  bool DoRequest(const ReplyTovCardPullingRequest& aRequest);

  bool DoRequest(const ReplyToPhonebookPullingRequest& aRequest);

  bool DoRequest(const ReplyTovCardListingRequest& aRequest);

  bool DoRequest(const ReplyToFolderListingRequest& aRequest);

  bool DoRequest(const ReplyToMessagesListingRequest& aRequest);

  bool DoRequest(const ReplyToGetMessageRequest& aRequest);

  bool DoRequest(const ReplyToSetMessageStatusRequest& aRequest);

  bool DoRequest(const ReplyToSendMessageRequest& aRequest);

  bool DoRequest(const ReplyToMessageUpdateRequest& aRequest);

#ifdef MOZ_B2G_RIL
  bool DoRequest(const AnswerWaitingCallRequest& aRequest);

  bool DoRequest(const IgnoreWaitingCallRequest& aRequest);

  bool DoRequest(const ToggleCallsRequest& aRequest);
#endif

  bool DoRequest(const SendMetaDataRequest& aRequest);

  bool DoRequest(const SendPlayStatusRequest& aRequest);

  bool DoRequest(const SendMessageEventRequest& aRequest);

  bool DoRequest(const ConnectGattClientRequest& aRequest);

  bool DoRequest(const DisconnectGattClientRequest& aRequest);

  bool DoRequest(const DiscoverGattServicesRequest& aRequest);

  bool DoRequest(const GattClientStartNotificationsRequest& aRequest);

  bool DoRequest(const GattClientStopNotificationsRequest& aRequest);

  bool DoRequest(const UnregisterGattClientRequest& aRequest);

  bool DoRequest(const GattClientReadRemoteRssiRequest& aRequest);

  bool DoRequest(const GattClientReadCharacteristicValueRequest& aRequest);

  bool DoRequest(const GattClientWriteCharacteristicValueRequest& aRequest);

  bool DoRequest(const GattClientReadDescriptorValueRequest& aRequest);

  bool DoRequest(const GattClientWriteDescriptorValueRequest& aRequest);

  bool DoRequest(const GattServerRegisterRequest& aRequest);

  bool DoRequest(const GattServerConnectPeripheralRequest& aRequest);

  bool DoRequest(const GattServerDisconnectPeripheralRequest& aRequest);

  bool DoRequest(const UnregisterGattServerRequest& aRequest);

  bool DoRequest(const GattServerAddServiceRequest& aRequest);

  bool DoRequest(const GattServerAddIncludedServiceRequest& aRequest);

  bool DoRequest(const GattServerAddCharacteristicRequest& aRequest);

  bool DoRequest(const GattServerAddDescriptorRequest& aRequest);

  bool DoRequest(const GattServerRemoveServiceRequest& aRequest);

  bool DoRequest(const GattServerStartServiceRequest& aRequest);

  bool DoRequest(const GattServerStopServiceRequest& aRequest);

  bool DoRequest(const GattServerSendResponseRequest& aRequest);

  bool DoRequest(const GattServerSendIndicationRequest& aRequest);
};

END_BLUETOOTH_NAMESPACE

#endif  // mozilla_dom_bluetooth_ipc_BluetoothRequestParent_h
