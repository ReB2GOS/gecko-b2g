/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "IccInfo.h"
#include "mozilla/dom/icc/IccChild.h"
#include "mozilla/dom/icc/IccIPCUtils.h"
#include "nsIStkCmdFactory.h"
#include "nsIStkProactiveCmd.h"

using mozilla::dom::IccInfo;

namespace mozilla {
namespace dom {
namespace icc {

/**
 * PIccChild Implementation.
 */

IccChild::IccChild()
  : mCardState(nsIIcc::CARD_STATE_UNKNOWN)
  , mIsAlive(true)
{
}

IccChild::~IccChild()
{
}

void
IccChild::Init()
{
  OptionalIccInfoData infoData;

  bool rv = SendInit(&infoData, &mCardState);
  NS_ENSURE_TRUE_VOID(rv);

  UpdateIccInfo(infoData);
}

void
IccChild::Shutdown(){
  if (mIsAlive) {
    mIsAlive = false;
    Send__delete__(this);
  }

  mListeners.Clear();
  mIccInfo = nullptr;
  mCardState = nsIIcc::CARD_STATE_UNKNOWN;
}

void
IccChild::ActorDestroy(ActorDestroyReason why)
{
  mIsAlive = false;
}

mozilla::ipc::IPCResult
IccChild::RecvNotifyCardStateChanged(const uint32_t& aCardState)
{
  mCardState = aCardState;

  for (int32_t i = 0; i < mListeners.Count(); i++) {
    mListeners[i]->NotifyCardStateChanged();
  }

  return IPC_OK();
}

mozilla::ipc::IPCResult
IccChild::RecvNotifyIccInfoChanged(const OptionalIccInfoData& aInfoData)
{
  UpdateIccInfo(aInfoData);

  for (int32_t i = 0; i < mListeners.Count(); i++) {
    mListeners[i]->NotifyIccInfoChanged();
  }

  return IPC_OK();
}

mozilla::ipc::IPCResult
IccChild::RecvNotifyStkCommand(const nsString& aStkProactiveCmd)
{
  nsCOMPtr<nsIStkCmdFactory> cmdFactory =
    do_GetService(ICC_STK_CMD_FACTORY_CONTRACTID);
  NS_ENSURE_TRUE(cmdFactory, IPC_FAIL_NO_REASON(this));

  nsCOMPtr<nsIStkProactiveCmd> cmd;
  cmdFactory->InflateCommand(aStkProactiveCmd, getter_AddRefs(cmd));

  NS_ENSURE_TRUE(cmd, IPC_FAIL_NO_REASON(this));

  for (int32_t i = 0; i < mListeners.Count(); i++) {
    mListeners[i]->NotifyStkCommand(cmd);
  }

  return IPC_OK();
}

mozilla::ipc::IPCResult
IccChild::RecvNotifyStkSessionEnd()
{
  for (int32_t i = 0; i < mListeners.Count(); i++) {
    mListeners[i]->NotifyStkSessionEnd();
  }

  return IPC_OK();
}

PIccRequestChild*
IccChild::AllocPIccRequestChild(const IccRequest& aRequest)
{
  MOZ_CRASH("Caller is supposed to manually construct a request!");
}
bool
IccChild::DeallocPIccRequestChild(PIccRequestChild* aActor)
{
  delete aActor;
  return true;
}

bool
IccChild::SendRequest(const IccRequest& aRequest, nsIIccCallback* aRequestReply)
{
  NS_ENSURE_TRUE(mIsAlive, IPC_FAIL_NO_REASON(this));

  // Deallocated in IccChild::DeallocPIccRequestChild().
  IccRequestChild* actor = new IccRequestChild(aRequestReply);
  SendPIccRequestConstructor(actor, aRequest);

  return true;
}

bool
IccChild::SendChannelRequest(const IccRequest& aRequest, nsIIccChannelCallback* aChannelRequestReply)
{
  NS_ENSURE_TRUE(mIsAlive, IPC_FAIL_NO_REASON(this));

  IccRequestChild* actor = new IccRequestChild(aChannelRequestReply);
  SendPIccRequestConstructor(actor, aRequest);

  return true;
}

void
IccChild::UpdateIccInfo(const OptionalIccInfoData& aInfoData) {
  if (aInfoData.type() == OptionalIccInfoData::Tvoid_t) {
    mIccInfo = nullptr;
    return;
  }

  NS_ENSURE_TRUE_VOID(aInfoData.type() == OptionalIccInfoData::TIccInfoData);

  RefPtr<IccInfo> iccInfo;
  const IccInfoData& infoData = aInfoData.get_IccInfoData();
  if (infoData.iccType().EqualsLiteral("sim")
      || infoData.iccType().EqualsLiteral("usim")) {
    iccInfo = new GsmIccInfo(infoData);
  } else if (infoData.iccType().EqualsLiteral("ruim")
             || infoData.iccType().EqualsLiteral("csim")){
    iccInfo = new CdmaIccInfo(infoData);
  } else {
    iccInfo = new IccInfo(infoData);
  }

  // We update the orignal one instead of replacing with a new one
  // if the IccType is the same.
  if (mIccInfo) {
    nsAutoString oldIccType;
    nsAutoString newIccType;
    nsIIccInfo* tmpInfo;

    mIccInfo->GetIccInfo(&tmpInfo);
    tmpInfo->GetIccType(oldIccType);

    iccInfo->GetIccInfo(&tmpInfo);
    tmpInfo->GetIccType(newIccType);

    if (oldIccType.Equals(newIccType)) {
      mIccInfo->Update(iccInfo);
      return;
    }
  }

  mIccInfo = iccInfo;
}

/**
 * nsIIcc Implementation.
 */

NS_IMPL_ISUPPORTS(IccChild, nsIIcc)

NS_IMETHODIMP
IccChild::RegisterListener(nsIIccListener *aListener)
{
  NS_ENSURE_TRUE(!mListeners.Contains(aListener), NS_ERROR_UNEXPECTED);

  mListeners.AppendObject(aListener);
  return NS_OK;
}

NS_IMETHODIMP
IccChild::UnregisterListener(nsIIccListener *aListener)
{
  NS_ENSURE_TRUE(mListeners.Contains(aListener), NS_ERROR_UNEXPECTED);

  mListeners.RemoveObject(aListener);
  return NS_OK;
}

NS_IMETHODIMP
IccChild::GetIccInfo(nsIIccInfo** aIccInfo)
{
  nsIIccInfo* iccInfo = nullptr;

  if (mIccInfo) {
    mIccInfo->GetIccInfo(&iccInfo);
  }

  nsCOMPtr<nsIIccInfo> info(iccInfo);
  info.forget(aIccInfo);
  return NS_OK;
}

NS_IMETHODIMP
IccChild::GetIsimInfo(nsIIsimIccInfo**)
{
  return NS_OK;
}

NS_IMETHODIMP
IccChild::GetCardState(uint32_t* aCardState)
{
  *aCardState = mCardState;
  return NS_OK;
}

NS_IMETHODIMP
IccChild::GetImsi(nsAString & aImsi)
{
  NS_WARNING("IMSI shall not directly be fetched in child process.");
  return NS_ERROR_NOT_IMPLEMENTED;
}

NS_IMETHODIMP
IccChild::GetCardLockEnabled(uint32_t aLockType,
                             nsIIccCallback* aRequestReply)
{
  return SendRequest(GetCardLockEnabledRequest(aLockType), aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::UnlockCardLock(uint32_t aLockType,
                         const nsAString& aPassword,
                         const nsAString& aNewPin,
                         nsIIccCallback* aRequestReply)
{
  return SendRequest(UnlockCardLockRequest(aLockType,
                                           nsAutoString(aPassword),
                                           nsAutoString(aNewPin)),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::SetCardLockEnabled(uint32_t aLockType,
                             const nsAString& aPassword,
                             bool aEnabled,
                             nsIIccCallback* aRequestReply)
{
  return SendRequest(SetCardLockEnabledRequest(aLockType,
                                               nsAutoString(aPassword),
                                               aEnabled),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::ChangeCardLockPassword(uint32_t aLockType,
                                 const nsAString& aPassword,
                                 const nsAString& aNewPassword,
                                 nsIIccCallback* aRequestReply)
{
  return SendRequest(ChangeCardLockPasswordRequest(aLockType,
                                                   nsAutoString(aPassword),
                                                   nsAutoString(aNewPassword)),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::GetCardLockRetryCount(uint32_t aLockType,
                                nsIIccCallback* aRequestReply)
{
  return SendRequest(GetCardLockRetryCountRequest(aLockType), aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::MatchMvno(uint32_t aMvnoType,
                    const nsAString& aMvnoData,
                    nsIIccCallback* aRequestReply)
{
  return SendRequest(MatchMvnoRequest(aMvnoType, nsAutoString(aMvnoData)),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::GetIccAuthentication(uint32_t aAppType,
                               uint32_t aAuthType,
                               const nsAString& aData,
                               nsIIccCallback* aRequestReply)
{
  return SendRequest(GetIccAuthenticationRequest(aAppType, aAuthType,
                                                nsAutoString(aData)),
                                                aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::GetServiceStateEnabled(uint32_t aService,
                                 nsIIccCallback* aRequestReply)
{
  return SendRequest(GetServiceStateEnabledRequest(aService),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::IccOpenChannel(const nsAString& aAid, nsIIccChannelCallback* aCallback)
{
  return SendChannelRequest(IccOpenChannelRequest(nsAutoString(aAid)), aCallback)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::IccExchangeAPDU(int32_t aChannel, uint8_t aCla, uint8_t aIns, uint8_t aP1,
                          uint8_t aP2, int16_t aP3, const nsAString & aData,
                          nsIIccChannelCallback* aCallback)
{
  return SendChannelRequest(IccExchangeAPDURequest(aChannel, aCla, aIns, aP1, aP2, aP3,
                                                   nsAutoString(aData)),
                                                   aCallback)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::IccCloseChannel(int32_t aChannel, nsIIccChannelCallback* aCallback)
{
  return SendChannelRequest(IccCloseChannelRequest(aChannel), aCallback)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::SendStkResponse(nsIStkProactiveCmd* aCommand, nsIStkTerminalResponse* aResponse)
{
  nsCOMPtr<nsIStkCmdFactory> cmdFactory =
    do_GetService(ICC_STK_CMD_FACTORY_CONTRACTID);
  NS_ENSURE_TRUE(cmdFactory, NS_ERROR_FAILURE);

  nsAutoString cmd, response;

  nsresult rv = cmdFactory->DeflateCommand(aCommand, cmd);
  NS_ENSURE_SUCCESS(rv, rv);

  rv = cmdFactory->DeflateResponse(aResponse, response);
  NS_ENSURE_SUCCESS(rv, rv);

  return PIccChild::SendStkResponse(cmd, response) ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::SendStkMenuSelection(uint16_t aItemIdentifier, bool aHelpRequested)
{
  return PIccChild::SendStkMenuSelection(aItemIdentifier, aHelpRequested)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::SendStkTimerExpiration(uint16_t aTimerId, uint32_t aTimerValue)
{
  return PIccChild::SendStkTimerExpiration(aTimerId, aTimerValue)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::SendStkEventDownload(nsIStkDownloadEvent* aEvent)
{
  MOZ_ASSERT(aEvent);

  nsCOMPtr<nsIStkCmdFactory> cmdFactory =
    do_GetService(ICC_STK_CMD_FACTORY_CONTRACTID);
  NS_ENSURE_TRUE(cmdFactory, NS_ERROR_FAILURE);

  nsAutoString event;

  nsresult rv = cmdFactory->DeflateDownloadEvent(aEvent, event);
  NS_ENSURE_SUCCESS(rv, rv);

  return PIccChild::SendStkEventDownload(event) ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::ReadContacts(uint32_t aContactType, nsIIccCallback* aRequestReply)
{
  return SendRequest(ReadContactsRequest(aContactType),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

NS_IMETHODIMP
IccChild::UpdateContact(uint32_t aContactType, nsIIccContact* aContact,
                        const nsAString& aPin2,
                        nsIIccCallback* aRequestReply)
{
  MOZ_ASSERT(aContact);

  IccContactData contactData;
  IccIPCUtils::GetIccContactDataFromIccContact(aContact, contactData);

  return SendRequest(UpdateContactRequest(aContactType,
                                          nsAutoString(aPin2),
                                          contactData),
                     aRequestReply)
    ? NS_OK : NS_ERROR_FAILURE;
}

/**
 * PIccRequestChild Implementation.
 */

IccRequestChild::IccRequestChild(nsIIccCallback* aRequestReply)
  : mRequestReply(aRequestReply)
{
  MOZ_COUNT_CTOR(IccRequestChild);
  MOZ_ASSERT(aRequestReply);
}

IccRequestChild::IccRequestChild(nsIIccChannelCallback* aChannelRequestReply)
  : mChannelRequestReply(aChannelRequestReply)
{
  MOZ_COUNT_CTOR(IccRequestChild);
  MOZ_ASSERT(aChannelRequestReply);
}

mozilla::ipc::IPCResult
IccRequestChild::Recv__delete__(const IccReply& aResponse)
{
  MOZ_ASSERT(mRequestReply);

  switch(aResponse.type()) {
    case IccReply::TIccReplySuccess:
      return NS_SUCCEEDED(mRequestReply->NotifySuccess())? IPC_OK(): IPC_FAIL_NO_REASON(this);
    case IccReply::TIccReplySuccessWithBoolean: {
      const IccReplySuccessWithBoolean& resultWithBoolean
        = aResponse.get_IccReplySuccessWithBoolean();
      return NS_SUCCEEDED(
        mRequestReply->NotifySuccessWithBoolean(resultWithBoolean.result()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyCardLockRetryCount: {
      const IccReplyCardLockRetryCount& retryCount
        = aResponse.get_IccReplyCardLockRetryCount();
      return NS_SUCCEEDED(
        mRequestReply->NotifyGetCardLockRetryCount(retryCount.count()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyError: {
      const IccReplyError& error = aResponse.get_IccReplyError();
      return NS_SUCCEEDED(mRequestReply->NotifyError(error.message()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyCardLockError: {
      const IccReplyCardLockError& error
        = aResponse.get_IccReplyCardLockError();
      return NS_SUCCEEDED(
        mRequestReply->NotifyCardLockError(error.message(),error.retryCount()))?
        IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyReadContacts: {
      const nsTArray<IccContactData>& data
        = aResponse.get_IccReplyReadContacts().contacts();

      uint32_t count = data.Length();
      nsCOMArray<nsIIccContact> contactList;
      nsresult rv;
      for (uint32_t i = 0; i < count; i++) {
        nsCOMPtr<nsIIccContact> contact;
        rv = IccContact::Create(data[i].id(),
                                data[i].names(),
                                data[i].numbers(),
                                data[i].emails(),
                                getter_AddRefs(contact));
        NS_ENSURE_SUCCESS(rv, IPC_FAIL_NO_REASON(this));
        contactList.AppendElement(contact);
      }

      rv = mRequestReply->NotifyRetrievedIccContacts(contactList.Elements(),
                                                     count);

      return NS_SUCCEEDED(rv)? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyUpdateContact: {
      IccContactData data
        = aResponse.get_IccReplyUpdateContact().contact();
      nsCOMPtr<nsIIccContact> contact;
      IccContact::Create(data.id(),
                         data.names(),
                         data.numbers(),
                         data.emails(),
                         getter_AddRefs(contact));

      return NS_SUCCEEDED(
        mRequestReply->NotifyUpdatedIccContact(contact))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyAuthResponse: {
      const IccReplyAuthResponse& res = aResponse.get_IccReplyAuthResponse();
      return NS_SUCCEEDED(mRequestReply->NotifyAuthResponse(res.data()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyOpenChannel: {
      const IccReplyOpenChannel& channel = aResponse.get_IccReplyOpenChannel();
      return NS_SUCCEEDED(
        mChannelRequestReply->NotifyOpenChannelSuccess(channel.channel()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyCloseChannel: {
      return NS_SUCCEEDED(mChannelRequestReply->NotifyCloseChannelSuccess())? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyExchangeAPDU: {
      nsresult rv;
      const IccReplyExchangeAPDU& apdu = aResponse.get_IccReplyExchangeAPDU();
      rv = mChannelRequestReply->NotifyExchangeAPDUResponse(apdu.sw1(),
                                                            apdu.sw2(),
                                                            apdu.data());
      return NS_SUCCEEDED(rv)? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    case IccReply::TIccReplyChannelError: {
      const IccReplyChannelError& error = aResponse.get_IccReplyChannelError();
      return NS_SUCCEEDED(mChannelRequestReply->NotifyError(error.message()))? IPC_OK(): IPC_FAIL_NO_REASON(this);
    }
    default:
      MOZ_CRASH("Received invalid response type!");
  }

  return IPC_OK();
}

} // namespace icc
} // namespace dom
} // namespace mozilla
