/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_dom_bluetooth_ipc_BluetoothServiceChildProcess_h
#define mozilla_dom_bluetooth_ipc_BluetoothServiceChildProcess_h

#include "BluetoothService.h"

BEGIN_BLUETOOTH_NAMESPACE

class BluetoothChild;

class BluetoothServiceChildProcess : public BluetoothService {
  friend class mozilla::dom::bluetooth::BluetoothChild;

 public:
  static BluetoothServiceChildProcess* Create();

  virtual void RegisterBluetoothSignalHandler(
      const nsAString& aNodeName,
      BluetoothSignalObserver* aMsgHandler) override;

  virtual void UnregisterBluetoothSignalHandler(
      const nsAString& aNodeName,
      BluetoothSignalObserver* aMsgHandler) override;

  virtual nsresult GetAdaptersInternal(
      BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult StartInternal(BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult StopInternal(BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult GetPairedDevicePropertiesInternal(
      const nsTArray<BluetoothAddress>& aDeviceAddresses,
      BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult GetConnectedDevicePropertiesInternal(
      uint16_t aServiceUuid, BluetoothReplyRunnable* aRunnable) override;
  virtual nsresult FetchUuidsInternal(
      const BluetoothAddress& aDeviceAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void StopDiscoveryInternal(
      BluetoothReplyRunnable* aRunnable) override;

  virtual void StartDiscoveryInternal(
      BluetoothReplyRunnable* aRunnable) override;

  virtual void StopLeScanInternal(const BluetoothUuid& aScanUuid,
                                  BluetoothReplyRunnable* aRunnable) override;

  virtual void StartLeScanInternal(const nsTArray<BluetoothUuid>& aServiceUuids,
                                   BluetoothReplyRunnable* aRunnable) override;

  virtual void StartAdvertisingInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothGattAdvertisingData& aAdvData,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void StopAdvertisingInternal(
      const BluetoothUuid& aAppUuid,
      BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult SetProperty(BluetoothObjectType aType,
                               const BluetoothNamedValue& aValue,
                               BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult CreatePairedDeviceInternal(
      const BluetoothAddress& aDeviceAddress, int aTimeout,
      BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult RemoveDeviceInternal(
      const BluetoothAddress& aDeviceAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual nsresult GetServiceChannel(
      const BluetoothAddress& aDeviceAddress, const BluetoothUuid& aServiceUuid,
      BluetoothProfileManagerBase* aManager) override;

  virtual bool UpdateSdpRecords(const BluetoothAddress& aDeviceAddress,
                                BluetoothProfileManagerBase* aManager) override;

  virtual void PinReplyInternal(const BluetoothAddress& aDeviceAddress,
                                bool aAccept, const BluetoothPinCode& aPinCode,
                                BluetoothReplyRunnable* aRunnable) override;

  virtual void SspReplyInternal(const BluetoothAddress& aDeviceAddress,
                                BluetoothSspVariant aVariant, bool aAccept,
                                BluetoothReplyRunnable* aRunnable) override;

  virtual void Connect(const BluetoothAddress& aDeviceAddress, uint32_t aCod,
                       uint16_t aServiceUuid,
                       BluetoothReplyRunnable* aRunnable) override;

  virtual void Disconnect(const BluetoothAddress& aDeviceAddress,
                          uint16_t aServiceUuid,
                          BluetoothReplyRunnable* aRunnable) override;

  virtual void AcceptConnection(const uint16_t aServiceUuid,
                                BluetoothReplyRunnable* aRunnable) override;

  virtual void RejectConnection(const uint16_t aServiceUuid,
                                BluetoothReplyRunnable* aRunnable) override;

  virtual void SendFile(const BluetoothAddress& aDeviceAddress, BlobImpl* aBlob,
                        BluetoothReplyRunnable* aRunnable) override;

  virtual void StopSendingFile(const BluetoothAddress& aDeviceAddress,
                               BluetoothReplyRunnable* aRunnable) override;

  virtual void ConfirmReceivingFile(const BluetoothAddress& aDeviceAddress,
                                    bool aConfirm,
                                    BluetoothReplyRunnable* aRunnable) override;

  virtual void ConnectSco(BluetoothReplyRunnable* aRunnable) override;

  virtual void DisconnectSco(BluetoothReplyRunnable* aRunnable) override;

  virtual void IsScoConnected(BluetoothReplyRunnable* aRunnable) override;

  virtual void SetObexPassword(const nsAString& aPassword,
                               BluetoothReplyRunnable* aRunnable) override;

  virtual void RejectObexAuth(BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyTovCardPulling(BlobImpl* aBlob,
                                   BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToPhonebookPulling(
      BlobImpl* aBlob, uint16_t aPhonebookSize,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyTovCardListing(BlobImpl* aBlob, uint16_t aPhonebookSize,
                                   BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapFolderListing(
      uint8_t aMasId, const nsAString& aFolderlists,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapMessagesListing(
      uint8_t aMasId, BlobImpl* aBlob, bool aNewMessage,
      const nsAString& aTimestamp, int aSize,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapGetMessage(uint8_t aMasId, BlobImpl* aBlob,
                                    BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapSetMessageStatus(
      uint8_t aMasId, bool aStatus, BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapSendMessage(
      uint8_t aMasId, const nsAString& aHandleId, bool aStatus,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void ReplyToMapMessageUpdate(
      uint8_t aMasId, bool aStatus, BluetoothReplyRunnable* aRunnable) override;

#ifdef MOZ_B2G_RIL
  virtual void AnswerWaitingCall(BluetoothReplyRunnable* aRunnable) override;

  virtual void IgnoreWaitingCall(BluetoothReplyRunnable* aRunnable) override;

  virtual void ToggleCalls(BluetoothReplyRunnable* aRunnable) override;
#endif

  virtual void SendMetaData(const nsAString& aTitle, const nsAString& aArtist,
                            const nsAString& aAlbum, int64_t aMediaNumber,
                            int64_t aTotalMediaCount, int64_t aDuration,
                            BluetoothReplyRunnable* aRunnable) override;

  virtual void SendPlayStatus(int64_t aDuration, int64_t aPosition,
                              ControlPlayStatus aPlayStatus,
                              BluetoothReplyRunnable* aRunnable) override;

  virtual void SendMessageEvent(uint8_t aMasId, BlobImpl* aBlob,
                                BluetoothReplyRunnable* aRunnable) override;

  virtual void ConnectGattClientInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aDeviceAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void DisconnectGattClientInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aDeviceAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void DiscoverGattServicesInternal(
      const BluetoothUuid& aAppUuid,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientStartNotificationsInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServId,
      const BluetoothGattId& aCharId,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientStopNotificationsInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServId,
      const BluetoothGattId& aCharId,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void UnregisterGattClientInternal(
      int aClientIf, BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientReadRemoteRssiInternal(
      int aClientIf, const BluetoothAddress& aDeviceAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientReadCharacteristicValueInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServiceId,
      const BluetoothGattId& aCharacteristicId,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientWriteCharacteristicValueInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServiceId,
      const BluetoothGattId& aCharacteristicId,
      const BluetoothGattWriteType& aWriteType, const nsTArray<uint8_t>& aValue,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientReadDescriptorValueInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServiceId,
      const BluetoothGattId& aCharacteristicId,
      const BluetoothGattId& aDescriptorId,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattClientWriteDescriptorValueInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServiceId,
      const BluetoothGattId& aCharacteristicId,
      const BluetoothGattId& aDescriptorId, const nsTArray<uint8_t>& aValue,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerRegisterInternal(
      const BluetoothUuid& aAppUuid,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerConnectPeripheralInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerDisconnectPeripheralInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aAddress,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void UnregisterGattServerInternal(
      int aServerIf, BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerAddServiceInternal(
      const BluetoothUuid& aAppUuid, const BluetoothGattServiceId& aServiceId,
      uint16_t aHandleCount, BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerAddIncludedServiceInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      const BluetoothAttributeHandle& aIncludedServiceHandle,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerAddCharacteristicInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      const BluetoothUuid& aCharacteristicUuid,
      BluetoothGattAttrPerm aPermissions, BluetoothGattCharProp aProperties,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerAddDescriptorInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      const BluetoothAttributeHandle& aCharacteristicHandle,
      const BluetoothUuid& aDescriptorUuid, BluetoothGattAttrPerm aPermissions,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerRemoveServiceInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerStartServiceInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerStopServiceInternal(
      const BluetoothUuid& aAppUuid,
      const BluetoothAttributeHandle& aServiceHandle,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerSendResponseInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aAddress,
      uint16_t aStatus, int32_t aRequestId, const BluetoothGattResponse& aRsp,
      BluetoothReplyRunnable* aRunnable) override;

  virtual void GattServerSendIndicationInternal(
      const BluetoothUuid& aAppUuid, const BluetoothAddress& aAddress,
      const BluetoothAttributeHandle& aCharacteristicHandle, bool aConfirm,
      const nsTArray<uint8_t>& aValue,
      BluetoothReplyRunnable* aRunnable) override;

 protected:
  BluetoothServiceChildProcess();
  virtual ~BluetoothServiceChildProcess();

  void NoteDeadActor();

  void NoteShutdownInitiated();

  virtual nsresult HandleStartup() override;

  virtual nsresult HandleShutdown() override;

 private:
  bool IsSignalRegistered(const nsAString& aNodeName) {
    return !!mBluetoothSignalObserverTable.Get(aNodeName);
  }
};

END_BLUETOOTH_NAMESPACE

#endif  // mozilla_dom_bluetooth_ipc_BluetoothServiceChildProcess_h
