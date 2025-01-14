/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "FMRadioParent.h"
#include "mozilla/Unused.h"
#include "mozilla/dom/ContentParent.h"
#include "mozilla/DebugOnly.h"
#include "FMRadioRequestParent.h"
#include "FMRadioService.h"

BEGIN_FMRADIO_NAMESPACE

FMRadioParent::FMRadioParent() {
  MOZ_COUNT_CTOR(FMRadioParent);

  IFMRadioService::Singleton()->AddObserver(this);
}

FMRadioParent::~FMRadioParent() {
  MOZ_COUNT_DTOR(FMRadioParent);

  IFMRadioService::Singleton()->RemoveObserver(this);
}

void FMRadioParent::ActorDestroy(ActorDestroyReason aWhy) {
  // Implement me! Bug 1005146
}

mozilla::ipc::IPCResult FMRadioParent::RecvGetStatusInfo(
    StatusInfo* aStatusInfo) {
  aStatusInfo->enabled() = IFMRadioService::Singleton()->IsEnabled();
  aStatusInfo->frequency() = IFMRadioService::Singleton()->GetFrequency();
  aStatusInfo->upperBound() =
      IFMRadioService::Singleton()->GetFrequencyUpperBound();
  aStatusInfo->lowerBound() =
      IFMRadioService::Singleton()->GetFrequencyLowerBound();
  aStatusInfo->channelWidth() = IFMRadioService::Singleton()->GetChannelWidth();
  return IPC_OK();
}

PFMRadioRequestParent* FMRadioParent::AllocPFMRadioRequestParent(
    const FMRadioRequestArgs& aArgs) {
  RefPtr<FMRadioRequestParent> requestParent = new FMRadioRequestParent();

  switch (aArgs.type()) {
    case FMRadioRequestArgs::TEnableRequestArgs:
      IFMRadioService::Singleton()->Enable(
          aArgs.get_EnableRequestArgs().frequency(), requestParent);
      break;
    case FMRadioRequestArgs::TDisableRequestArgs:
      IFMRadioService::Singleton()->Disable(requestParent);
      break;
    case FMRadioRequestArgs::TSetFrequencyRequestArgs:
      IFMRadioService::Singleton()->SetFrequency(
          aArgs.get_SetFrequencyRequestArgs().frequency(), requestParent);
      break;
    case FMRadioRequestArgs::TSeekRequestArgs:
      IFMRadioService::Singleton()->Seek(
          aArgs.get_SeekRequestArgs().direction(), requestParent);
      break;
    case FMRadioRequestArgs::TCancelSeekRequestArgs:
      IFMRadioService::Singleton()->CancelSeek(requestParent);
      break;
    case FMRadioRequestArgs::TEnableRDSArgs:
      IFMRadioService::Singleton()->EnableRDS(requestParent);
      break;
    case FMRadioRequestArgs::TDisableRDSArgs:
      IFMRadioService::Singleton()->DisableRDS(requestParent);
      break;
    default:
      MOZ_CRASH();
  }

  // Balanced in DeallocPFMRadioRequestParent
  return requestParent.forget().take();
}

bool FMRadioParent::DeallocPFMRadioRequestParent(
    PFMRadioRequestParent* aActor) {
  FMRadioRequestParent* parent = static_cast<FMRadioRequestParent*>(aActor);
  NS_RELEASE(parent);
  return true;
}

void FMRadioParent::Notify(const FMRadioEventType& aType) {
  switch (aType) {
    case FrequencyChanged:
      Unused << SendNotifyFrequencyChanged(
          IFMRadioService::Singleton()->GetFrequency());
      break;
    case EnabledChanged:
      Unused << SendNotifyEnabledChanged(
          IFMRadioService::Singleton()->IsEnabled(),
          IFMRadioService::Singleton()->GetFrequency());
      break;
    case RDSEnabledChanged:
      Unused << SendNotifyRDSEnabledChanged(
          IFMRadioService::Singleton()->IsRDSEnabled());
      break;
    case PIChanged: {
      Nullable<unsigned short> pi = IFMRadioService::Singleton()->GetPi();
      Unused << SendNotifyPIChanged(!pi.IsNull(), pi.IsNull() ? 0 : pi.Value());
      break;
    }
    case PTYChanged: {
      Nullable<uint8_t> pty = IFMRadioService::Singleton()->GetPty();
      Unused << SendNotifyPTYChanged(!pty.IsNull(),
                                     pty.IsNull() ? 0 : pty.Value());
      break;
    }
    case PSChanged: {
      nsAutoString psname;
      IFMRadioService::Singleton()->GetPs(psname);
      Unused << SendNotifyPSChanged(psname);
      break;
    }
    case RadiotextChanged: {
      nsAutoString radiotext;
      IFMRadioService::Singleton()->GetRt(radiotext);
      Unused << SendNotifyRadiotextChanged(radiotext);
      break;
    }
    case NewRDSGroup: {
      uint64_t group;
      DebugOnly<bool> rdsgroupset =
          IFMRadioService::Singleton()->GetRdsgroup(group);
      MOZ_ASSERT(rdsgroupset);
      Unused << SendNotifyNewRDSGroup(group);
      break;
    }
    default:
      MOZ_CRASH("not reached");
      break;
  }
}

mozilla::ipc::IPCResult FMRadioParent::RecvEnableAudio(
    const bool& aAudioEnabled) {
  IFMRadioService::Singleton()->EnableAudio(aAudioEnabled);
  return IPC_OK();
}

mozilla::ipc::IPCResult FMRadioParent::RecvSetRDSGroupMask(
    const uint32_t& aRDSGroupMask) {
  IFMRadioService::Singleton()->SetRDSGroupMask(aRDSGroupMask);
  return IPC_OK();
}

END_FMRADIO_NAMESPACE
