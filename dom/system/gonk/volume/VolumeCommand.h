/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_system_volumecommand_h__
#define mozilla_system_volumecommand_h__

#include "nsString.h"
#include "nsISupportsImpl.h"
#include "mozilla/RefPtr.h"
#include <algorithm>

namespace mozilla {
namespace system {

class Volume;
class VolumeCommand;

/***************************************************************************
 *
 *   The VolumeResponseCallback class is an abstract base class. The
 *ResponseReceived method will be called for each response received.
 *
 *   Depending on the command, there may be multiple responses for the
 *   command. Done() will return true if this is the last response.
 *
 *   The responses from vold are all of the form:
 *
 *     <ResponseCode> <String>
 *
 *   Valid Response codes can be found in the vold/ResponseCode.h header.
 *
 ***************************************************************************/

class VolumeResponseCallback {
 protected:
  virtual ~VolumeResponseCallback() {}

 public:
  NS_INLINE_DECL_REFCOUNTING(VolumeResponseCallback)
  VolumeResponseCallback() : mResponseCode(0), mPending(false) {}

  bool Done() const {
    // Response codes from the 200, 400, and 500 series all indicated that
    // the command has completed.

    return false;
  }

  bool WasSuccessful() const {
    return false;
  }

  bool IsPending() const { return mPending; }
  int ResponseCode() const { return mResponseCode; }
  const nsCString& ResponseStr() const { return mResponseStr; }

 protected:
  virtual void ResponseReceived(const VolumeCommand* aCommand) = 0;

 private:
  friend class VolumeCommand;  // Calls HandleResponse and SetPending

  void HandleResponse(const VolumeCommand* aCommand, int aResponseCode,
                      nsACString& aResponseStr) {
    mResponseCode = aResponseCode;
    // There's a sequence number here that we don't care about
    // We expect it to be 0. See VolumeCommand::SetCmd
    mResponseStr = Substring(aResponseStr, 2);
    ResponseReceived(aCommand);
  }

  void SetPending(bool aPending) { mPending = aPending; }

  int mResponseCode;       // The response code parsed from vold
  nsCString mResponseStr;  // The rest of the line.
  bool mPending;           // Waiting for response?
};

/***************************************************************************
 *
 *   The VolumeCommand class is an abstract base class used to encapsulate
 *   volume commands send to vold.
 *
 *   See VolumeManager.h for a list of the volume commands.
 *
 *   Commands sent to vold need an explicit null character so we add one
 *   to the command to ensure that it's included in the length.
 *
 *   All of these commands are asynchronous in nature, and the
 *   ResponseReceived callback will be called when a response is available.
 *
 ***************************************************************************/

class VolumeCommand {
 protected:
  virtual ~VolumeCommand() {}

 public:
  NS_INLINE_DECL_REFCOUNTING(VolumeCommand)

  explicit VolumeCommand(VolumeResponseCallback* aCallback)
      : mBytesConsumed(0), mCallback(aCallback) {
    SetCmd(NS_LITERAL_CSTRING(""));
  }

  VolumeCommand(const nsACString& aCommand, VolumeResponseCallback* aCallback)
      : mBytesConsumed(0), mCallback(aCallback) {
    SetCmd(aCommand);
  }

  void SetCmd(const nsACString& aCommand) {
    mCmd.Truncate();
    // JB requires a sequence number at the beginning of messages.
    // It doesn't matter what we use, so we use 0.
    mCmd = "0 ";
    mCmd.Append(aCommand);
    // Add a null character. We want this to be included in the length since
    // vold uses it to determine the end of the command.
    mCmd.Append('\0');
  }

  const char* CmdStr() const { return mCmd.get(); }
  const char* Data() const { return mCmd.Data() + mBytesConsumed; }
  size_t BytesConsumed() const { return mBytesConsumed; }

  size_t BytesRemaining() const {
    return mCmd.Length() - std::min((size_t)mCmd.Length(), mBytesConsumed);
  }

  void ConsumeBytes(size_t aNumBytes) {
    mBytesConsumed += std::min(BytesRemaining(), aNumBytes);
  }

 private:
  friend class VolumeManager;  // Calls SetPending & HandleResponse

  void SetPending(bool aPending) {
    if (mCallback) {
      mCallback->SetPending(aPending);
    }
  }

  void HandleResponse(int aResponseCode, nsACString& aResponseStr) {
    if (mCallback) {
      mCallback->HandleResponse(this, aResponseCode, aResponseStr);
    }
  }

  nsCString mCmd;         // Command being sent
  size_t mBytesConsumed;  // How many bytes have been sent

  // Called when a response to the command is received.
  RefPtr<VolumeResponseCallback> mCallback;
};

class VolumeActionCommand : public VolumeCommand {
 public:
  VolumeActionCommand(Volume* aVolume, const char* aAction,
                      const char* aExtraArgs,
                      VolumeResponseCallback* aCallback);

 private:
  RefPtr<Volume> mVolume;
};

class VolumeResetCommand : public VolumeCommand {
 public:
  explicit VolumeResetCommand(VolumeResponseCallback* aCallback);
};


}  // namespace system
}  // namespace mozilla

#endif  // mozilla_system_volumecommand_h__
