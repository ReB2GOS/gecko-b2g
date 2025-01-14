/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*-
 * vim: sw=2 ts=4 et :
 */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef mozilla_ipc_ProducerConsumerQueue_h
#define mozilla_ipc_ProducerConsumerQueue_h 1

#include <atomic>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
#include "mozilla/WeakPtr.h"
#include "mozilla/dom/QueueParamTraits.h"
#include "CrossProcessSemaphore.h"
#include "nsThreadUtils.h"

namespace IPC {
template <typename T>
struct ParamTraits;
}  // namespace IPC

namespace mozilla {
namespace webgl {

using mozilla::ipc::IProtocol;
using mozilla::ipc::Shmem;

extern LazyLogModule gPCQLog;
#define PCQ_LOG_(lvl, ...) MOZ_LOG(mozilla::webgl::gPCQLog, lvl, (__VA_ARGS__))
#define PCQ_LOGD(...) PCQ_LOG_(LogLevel::Debug, __VA_ARGS__)
#define PCQ_LOGE(...) PCQ_LOG_(LogLevel::Error, __VA_ARGS__)

class ProducerConsumerQueue;
class PcqProducer;
class PcqConsumer;

/**
 * PcqActor is an actor base-class that is used as a static map that
 * provides casting from an IProtocol to a PcqActor.  PcqActors delegate
 * all needed IProtocol operations and also support weak references.
 * Actors used to construct a PCQ must implement this class.
 * Example:
 * class MyActorParent : public PMyActorParent, public PcqActor {
 *   MyActorParent() : PcqActor(this) {}
 *   // ...
 * }
 * Implementations of abstract methods will typically just forward to IProtocol.
 */
class PcqActor : public SupportsWeakPtr<PcqActor> {
  // The IProtocol part of `this`.
  IProtocol* mProtocol;

  inline static std::unordered_map<IProtocol*, PcqActor*> sMap;

  static bool IsActorThread() {
    static nsIThread* sActorThread = [] { return NS_GetCurrentThread(); }();
    return sActorThread == NS_GetCurrentThread();
  }

 protected:
  explicit PcqActor(IProtocol* aProtocol) : mProtocol(aProtocol) {
    MOZ_ASSERT(IsActorThread());
    sMap.insert({mProtocol, this});
  }
  ~PcqActor() {
    MOZ_ASSERT(IsActorThread());
    sMap.erase(mProtocol);
  }

 public:
  MOZ_DECLARE_WEAKREFERENCE_TYPENAME(PcqActor)

  Shmem::SharedMemory* LookupSharedMemory(int32_t aId) {
    return mProtocol->LookupSharedMemory(aId);
  }
  int32_t Id() const { return mProtocol->Id(); }
  base::ProcessId OtherPid() const { return mProtocol->OtherPid(); }
  bool AllocShmem(size_t aSize,
                  mozilla::ipc::SharedMemory::SharedMemoryType aShmType,
                  mozilla::ipc::Shmem* aShmem) {
    return mProtocol->AllocShmem(aSize, aShmType, aShmem);
  }

  static PcqActor* LookupProtocol(IProtocol* aProtocol) {
    MOZ_ASSERT(IsActorThread());
    auto it = sMap.find(aProtocol);
    return (it != sMap.end()) ? it->second : nullptr;
  }
};

}  // namespace webgl

// NB: detail is in mozilla instead of mozilla::webgl because many points in
// existing code get confused if mozilla::detail and mozilla::webgl::detail
// exist.
namespace detail {
using mozilla::ipc::IProtocol;
using mozilla::ipc::Shmem;
using mozilla::webgl::IsSuccess;
using mozilla::webgl::PcqActor;
using mozilla::webgl::ProducerConsumerQueue;
using mozilla::webgl::QueueStatus;

constexpr size_t GetCacheLineSize() { return 64; }

// NB: The header may end up consuming fewer bytes than this.  This value
// guarantees that we can always byte-align the header contents.
constexpr size_t GetMaxHeaderSize() {
  // Recall that the Shmem contents are laid out like this:
  // -----------------------------------------------------------------------
  // queue contents | align1 | mRead | align2 | mWrite | align3 | User Data
  // -----------------------------------------------------------------------

  constexpr size_t alignment =
      std::max(std::alignment_of<size_t>::value, GetCacheLineSize());
  static_assert(alignment >= sizeof(size_t),
                "alignment expected to be large enough to hold a size_t");

  // We may need up to this many bytes to properly align mRead
  constexpr size_t maxAlign1 = alignment - 1;
  constexpr size_t readAndAlign2 = alignment;
  constexpr size_t writeAndAlign3 = alignment;
  return maxAlign1 + readAndAlign2 + writeAndAlign3;
}

template <typename View, typename Arg, typename... Args>
size_t MinSizeofArgs(View& aView, const Arg& aArg, const Args&... aArgs) {
  return aView.MinSizeParam(aArg) + MinSizeofArgs(aView, aArgs...);
}

template <typename View>
size_t MinSizeofArgs(View&) {
  return 0;
}

class PcqRCSemaphore {
 public:
  NS_INLINE_DECL_THREADSAFE_REFCOUNTING(PcqRCSemaphore)
  explicit PcqRCSemaphore(CrossProcessSemaphore* aSem) : mSem(aSem) {
    MOZ_ASSERT(mSem);
  }

  bool Wait(const Maybe<TimeDuration>& aTime) { return mSem->Wait(aTime); }
  void Signal() { mSem->Signal(); }
  bool IsAvailable() {
    MOZ_ASSERT_UNREACHABLE("Unimplemented");
    return false;
  }
  CrossProcessSemaphoreHandle ShareToProcess(base::ProcessId aTargetPid) {
    return mSem->ShareToProcess(aTargetPid);
  }
  void CloseHandle() { mSem->CloseHandle(); }

 private:
  ~PcqRCSemaphore() { delete mSem; }
  CrossProcessSemaphore* mSem;
};

/**
 * Common base class for PcqProducer and Consumer.
 */
class PcqBase {
 public:
  /**
   * Bytes used in the queue if the parameters are the read/write heads.
   */
  size_t UsedBytes(size_t aRead, size_t aWrite) {
    MOZ_ASSERT(ValidState(aRead, aWrite));
    return mozilla::webgl::UsedBytes(QueueBufferSize(), aRead, aWrite);
  }

  /**
   * Bytes free in the queue if the parameters are the read/write heads.
   */
  size_t FreeBytes(size_t aRead, size_t aWrite) {
    MOZ_ASSERT(ValidState(aRead, aWrite));
    return mozilla::webgl::FreeBytes(QueueBufferSize(), aRead, aWrite);
  }

  /**
   * True when this queue is valid with the parameters as the read/write heads.
   */
  bool ValidState(size_t aRead, size_t aWrite) {
    return (aRead < QueueBufferSize()) && (aWrite < QueueBufferSize());
  }

  /**
   * True when this queue is empty with the parameters as the read/write heads.
   */
  bool IsEmpty(size_t aRead, size_t aWrite) {
    MOZ_ASSERT(ValidState(aRead, aWrite));
    return UsedBytes(aRead, aWrite) == 0;
  }

  /**
   * True when this queue is full with the parameters as the read/write heads.
   */
  bool IsFull(size_t aRead, size_t aWrite) {
    MOZ_ASSERT(ValidState(aRead, aWrite));
    return FreeBytes(aRead, aWrite) == 0;
  }

  // Cheaply get the used size of the current queue.  This does no
  // synchronization so the information may be stale.  On the PcqProducer
  // side, it will never underestimate the number of bytes used and,
  // on the Consumer side, it will never overestimate them.
  // (The reciprocal is true of FreeBytes.)
  size_t UsedBytes() {
    size_t write = mWrite->load(std::memory_order_relaxed);
    size_t read = mRead->load(std::memory_order_relaxed);
    return UsedBytes(read, write);
  }

  // This does no synchronization so the information may be stale.
  size_t FreeBytes() { return QueueSize() - UsedBytes(); }

  // This does no synchronization so the information may be stale.
  bool IsEmpty() { return IsEmpty(GetReadRelaxed(), GetWriteRelaxed()); }

  // This does no synchronization so the information may be stale.
  bool IsFull() { return IsFull(GetReadRelaxed(), GetWriteRelaxed()); }

 protected:
  friend struct mozilla::ipc::IPDLParamTraits<PcqBase>;
  friend ProducerConsumerQueue;

  PcqBase() = default;

  PcqBase(Shmem& aShmem, IProtocol* aProtocol, size_t aQueueSize,
          RefPtr<PcqRCSemaphore> aMaybeNotEmptySem,
          RefPtr<PcqRCSemaphore> aMaybeNotFullSem) {
    Set(aShmem, aProtocol, aQueueSize, aMaybeNotEmptySem, aMaybeNotFullSem);
  }

  PcqBase(const PcqBase&) = delete;
  PcqBase(PcqBase&&) = default;
  PcqBase& operator=(const PcqBase&) = delete;
  PcqBase& operator=(PcqBase&&) = default;

  void Set(Shmem& aShmem, IProtocol* aProtocol, size_t aQueueSize,
           RefPtr<PcqRCSemaphore> aMaybeNotEmptySem,
           RefPtr<PcqRCSemaphore> aMaybeNotFullSem) {
    mActor = PcqActor::LookupProtocol(aProtocol);
    MOZ_RELEASE_ASSERT(mActor);

    mOtherPid = mActor->OtherPid();
    mShmem = aShmem;
    mQueue = aShmem.get<uint8_t>();

    // NB: The buffer needs one extra byte for the queue contents
    mQueueBufferSize = aQueueSize + 1;

    // Recall that the Shmem contents are laid out like this:
    // -----------------------------------------------------------------------
    // queue contents | align1 | mRead | align2 | mWrite | align3 | User Data
    // -----------------------------------------------------------------------

    size_t shmemSize = aShmem.Size<uint8_t>();
    uint8_t* header = mQueue + mQueueBufferSize;

    constexpr size_t alignment =
        std::max(std::alignment_of<size_t>::value, GetCacheLineSize());
    static_assert(alignment >= sizeof(size_t),
                  "alignment expected to be large enough to hold a size_t");

    static_assert((alignment & (alignment - 1)) == 0,
                  "alignment must be a power of 2");

    // We may need up to this many bytes to properly align mRead
    constexpr size_t maxAlign1 = alignment - 1;

    // Find the lowest value of align1 that assures proper byte-alignment.
    uintptr_t alignValue = reinterpret_cast<uintptr_t>(header + maxAlign1);
    alignValue &= ~(alignment - 1);
    uint8_t* metadata = reinterpret_cast<uint8_t*>(alignValue);

    // NB: We do not call the nontrivial constructor here (we do not write
    // `new std::atomic_size_t()`) because it would zero the read/write values
    // in the shared memory, which may already represent data in the queue.
    mRead = new (metadata) std::atomic_size_t;
    mWrite = new (metadata + alignment) std::atomic_size_t;

    // The actual number of bytes we needed to properly align mRead
    size_t align1 = metadata - header;
    MOZ_ASSERT(align1 <= maxAlign1);

    // The rest of the memory is the user reserved memory
    size_t headerSize = align1 + 2 * alignment;
    size_t userSize = shmemSize - mQueueBufferSize - headerSize;
    if (userSize > 0) {
      mUserReservedMemory = mQueue + mQueueBufferSize + headerSize;
      mUserReservedSize = userSize;
    } else {
      mUserReservedMemory = nullptr;
      mUserReservedSize = 0;
    }

    // We use Monitors to wait for data when reading from an empty queue
    // and to wait for free space when writing to a full one.
    MOZ_ASSERT(aMaybeNotEmptySem && aMaybeNotFullSem);
    mMaybeNotEmptySem = aMaybeNotEmptySem;
    mMaybeNotFullSem = aMaybeNotFullSem;

    PCQ_LOGD("Created queue (%p) with size: %zu, alignment: %zu, align1: %zu",
             this, aQueueSize, alignment, align1);
  }

  ~PcqBase() {
    PCQ_LOGD("Destroying queue (%p).", this);
    // NB: We would call the destructors for mRead and mWrite here (but not
    // delete since their memory belongs to the shmem) but the std library's
    // type aliases make this tricky and, by the spec for std::atomic, their
    // destructors are trivial (i.e. no-ops) anyway.
  }

  size_t GetReadRelaxed() { return mRead->load(std::memory_order_relaxed); }

  size_t GetWriteRelaxed() { return mWrite->load(std::memory_order_relaxed); }

  /**
   * The QueueSize is the number of bytes the queue can hold.  The queue is
   * backed by a buffer that is one byte larger than this, meaning that one
   * byte of the buffer is always wasted.
   * This is usually the right method to use when testing queue capacity.
   */
  size_t QueueSize() { return QueueBufferSize() - 1; }

  /**
   * The QueueBufferSize is the number of bytes in the buffer that the queue
   * uses for storage.
   * This is usually the right method to use when calculating read/write head
   * positions.
   */
  size_t QueueBufferSize() { return mQueueBufferSize; }

  // Actor used for making Shmems.
  WeakPtr<PcqActor> mActor;

  // PID of process on the other end.  Both ends may run on the same process.
  base::ProcessId mOtherPid = 0;

  uint8_t* mQueue = nullptr;
  size_t mQueueBufferSize = 0;

  // Pointer to memory reserved for use by the user, or null if none
  uint8_t* mUserReservedMemory = nullptr;
  size_t mUserReservedSize = 0;

  // These std::atomics are in shared memory so DO NOT DELETE THEM!  We should,
  // however, call their destructors.
  std::atomic_size_t* mRead = nullptr;
  std::atomic_size_t* mWrite = nullptr;

  // The Shmem contents are laid out like this:
  // -----------------------------------------------------------------------
  // queue contents | align1 | mRead | align2 | mWrite | align3 | User Data
  // -----------------------------------------------------------------------
  // where align1 is chosen so that mRead is properly aligned for a
  // std_atomic_size_t and is on a cache line separate from the queue contents
  // align2 and align3 is chosen to separate mRead/mWrite and mWrite/User Data
  // similarly.
  Shmem mShmem;

  // Two semaphores that are signaled when the queue goes from a state
  // where it definitely is empty/full to a state where it "may not be".
  // Therefore, we can wait on them and know that we will be awakened if
  // there may be work to do.
  // Our use of these semaphores leans heavily on the assumption that
  // the queue is used by one producer and one consumer.
  RefPtr<PcqRCSemaphore> mMaybeNotEmptySem;
  RefPtr<PcqRCSemaphore> mMaybeNotFullSem;
};

}  // namespace detail

namespace webgl {

using mozilla::ipc::Shmem;

/**
 * The PcqProducer is the endpoint that inserts elements into the queue.  It
 * should only be used from one thread at a time.
 */
class PcqProducer : public detail::PcqBase {
 public:
  PcqProducer(PcqProducer&& aOther) = default;
  PcqProducer& operator=(PcqProducer&&) = default;
  PcqProducer() = default;  // for IPDL

  /**
   * The number of bytes that the queue can hold.
   */
  size_t Size() { return QueueSize(); }

  /**
   * Attempts to insert aArgs into the queue.  If the operation does not
   * succeed then the queue is unchanged.
   */
  template <typename... Args>
  QueueStatus TryInsert(Args&&... aArgs) {
    size_t write = mWrite->load(std::memory_order_relaxed);
    const size_t initWrite = write;
    size_t read = mRead->load(std::memory_order_acquire);

    if (!ValidState(read, write)) {
      PCQ_LOGE(
          "Queue was found in an invalid state.  Queue Size: %zu.  "
          "Read: %zu.  Write: %zu",
          Size(), read, write);
      return QueueStatus::kFatalError;
    }

    ProducerView view(this, read, &write);

    // Check that the queue has enough unoccupied room for all Args types.
    // This is based on the user's size estimate for args from QueueParamTraits.
    size_t bytesNeeded = detail::MinSizeofArgs(view, aArgs...);

    if (Size() < bytesNeeded) {
      PCQ_LOGE(
          "Queue is too small for objects.  Queue Size: %zu.  "
          "Needed: %zu",
          Size(), bytesNeeded);
      return QueueStatus::kTooSmall;
    }

    if (FreeBytes(read, write) < bytesNeeded) {
      PCQ_LOGD(
          "Not enough room to insert.  Has: %zu (%zu,%zu).  "
          "Needed: %zu",
          FreeBytes(read, write), read, write, bytesNeeded);
      return QueueStatus::kNotReady;
    }

    // Try to insert args in sequence.  Only update the queue if the
    // operation was successful.  We already checked all normal means of
    // failure but we can expect occasional failure here if the user's
    // QueueParamTraits::MinSize method was inexact.
    QueueStatus status = TryInsertHelper(view, aArgs...);
    if (!status) {
      PCQ_LOGD(
          "Failed to insert with error (%d).  Has: %zu (%zu,%zu).  "
          "Estimate of bytes needed: %zu",
          (int)status, FreeBytes(read, write), read, write, bytesNeeded);
      return status;
    }

    MOZ_ASSERT(ValidState(read, write));

    // Check that at least bytesNeeded were produced.  Failing this means
    // that some QueueParamTraits::MinSize estimated too many bytes.
    bool enoughBytes =
        UsedBytes(read, write) >=
        UsedBytes(read, (initWrite + bytesNeeded) % QueueBufferSize());
    MOZ_ASSERT(enoughBytes);
    if (!enoughBytes) {
      return QueueStatus::kFatalError;
    }

    // Commit the transaction.
    PCQ_LOGD(
        "Successfully inserted.  PcqProducer used %zu bytes total.  "
        "Write index: %zu -> %zu",
        bytesNeeded, initWrite, write);
    mWrite->store(write, std::memory_order_release);

    // Set the semaphore (unless it is already set) to let the consumer know
    // that the queue may not be empty.  We just need to guarantee that it
    // was set (i.e. non-zero) at some time after mWrite was updated.
    if (!mMaybeNotEmptySem->IsAvailable()) {
      mMaybeNotEmptySem->Signal();
    }
    return status;
  }

  /**
   * Attempts to insert aArgs into the queue.  If the operation does not
   * succeed in the time allotted then the queue is unchanged.
   */
  template <typename... Args>
  QueueStatus TryWaitInsert(const Maybe<TimeDuration>& aDuration,
                            Args&&... aArgs) {
    return TryWaitInsertImpl(false, aDuration, std::forward<Args>(aArgs)...);
  }

  QueueStatus AllocShmem(mozilla::ipc::Shmem* aShmem, size_t aBufferSize,
                         const void* aBuffer = nullptr) {
    if (!mActor) {
      return QueueStatus::kFatalError;
    }

    if (!mActor->AllocShmem(
            aBufferSize,
            mozilla::ipc::SharedMemory::SharedMemoryType::TYPE_BASIC, aShmem)) {
      return QueueStatus::kOOMError;
    }

    if (aBuffer) {
      memcpy(aShmem->get<uint8_t>(), aBuffer, aBufferSize);
    }
    return QueueStatus::kSuccess;
  }

 protected:
  friend ProducerConsumerQueue;
  friend ProducerView<PcqProducer>;

  template <typename Arg, typename... Args>
  QueueStatus TryInsertHelper(ProducerView<PcqProducer>& aView, Arg&& aArg,
                              Args&&... aArgs) {
    QueueStatus status = TryInsertItem(aView, std::forward<Arg>(aArg));
    return IsSuccess(status) ? TryInsertHelper(aView, aArgs...) : status;
  }

  QueueStatus TryInsertHelper(ProducerView<PcqProducer>&) {
    return QueueStatus::kSuccess;
  }

  template <typename Arg>
  QueueStatus TryInsertItem(ProducerView<PcqProducer>& aView, Arg&& aArg) {
    return QueueParamTraits<typename RemoveCVR<Arg>::Type>::Write(
        aView, std::forward<Arg>(aArg));
  }

  template <typename... Args>
  QueueStatus TryWaitInsertImpl(bool aRecursed,
                                const Maybe<TimeDuration>& aDuration,
                                Args&&... aArgs) {
    // Wait up to aDuration for the not-full semaphore to be signaled.
    // If we run out of time then quit.
    TimeStamp start(TimeStamp::Now());
    if (aRecursed && (!mMaybeNotFullSem->Wait(aDuration))) {
      return QueueStatus::kNotReady;
    }

    // Attempt to insert all args.  No waiting is done here.
    QueueStatus status = TryInsert(std::forward<Args>(aArgs)...);

    TimeStamp now;
    if (aRecursed && IsSuccess(status)) {
      // If our local view of the queue is that it is still not full then
      // we know it won't get full without us (we are the only producer).
      // So re-set the not-full semaphore unless it's already set.
      // (We are also the only not-full semaphore decrementer so it can't
      // become 0.)
      if ((!IsFull()) && (!mMaybeNotFullSem->IsAvailable())) {
        mMaybeNotFullSem->Signal();
      }
    } else if ((status == QueueStatus::kNotReady) &&
               (aDuration.isNothing() ||
                ((now = TimeStamp::Now()) - start) < aDuration.value())) {
      // We don't have enough room but still have time, e.g. because
      // the consumer read some data but not enough or because the
      // not-full semaphore gave a false positive.  Either way, retry.
      status =
          aDuration.isNothing()
              ? TryWaitInsertImpl(true, aDuration, std::forward<Args>(aArgs)...)
              : TryWaitInsertImpl(true, Some(aDuration.value() - (now - start)),
                                  std::forward<Args>(aArgs)...);
    }

    return status;
  }

  template <typename Arg>
  QueueStatus WriteObject(size_t aRead, size_t* aWrite, const Arg& arg,
                          size_t aArgSize) {
    return Marshaller::WriteObject(mQueue, QueueBufferSize(), aRead, aWrite,
                                   arg, aArgSize);
  }

  // Currently, the PCQ requires any parameters expected to need more than
  // 1/16 the total number of bytes in the command queue to use their own
  // SharedMemory.
  bool NeedsSharedMemory(size_t aRequested) {
    return (Size() / 16) < aRequested;
  }

  PcqProducer(Shmem& aShmem, IProtocol* aProtocol, size_t aQueueSize,
              RefPtr<detail::PcqRCSemaphore> aMaybeNotEmptySem,
              RefPtr<detail::PcqRCSemaphore> aMaybeNotFullSem)
      : PcqBase(aShmem, aProtocol, aQueueSize, aMaybeNotEmptySem,
                aMaybeNotFullSem) {
    // Since they are shared, this initializes mRead/mWrite in the PcqConsumer
    // as well.
    *mRead = 0;
    *mWrite = 0;
  }

  PcqProducer(const PcqProducer&) = delete;
  PcqProducer& operator=(const PcqProducer&) = delete;
};

class PcqConsumer : public detail::PcqBase {
 public:
  PcqConsumer(PcqConsumer&& aOther) = default;
  PcqConsumer& operator=(PcqConsumer&&) = default;
  PcqConsumer() = default;  // for IPDL

  /**
   * The number of bytes that the queue can hold.
   */
  size_t Size() { return QueueSize(); }

  /**
   * Attempts to copy and remove aArgs from the queue.  If the operation does
   * not succeed then the queue is unchanged.
   */
  template <typename... Args>
  QueueStatus TryRemove(Args&... aArgs) {
    return TryRemoveImpl(aArgs...);
  }

  /**
   * Wait for up to aDuration to remove the requested data from the queue.
   * Pass Nothing to wait until removal succeeds.
   */
  template <typename... Args>
  QueueStatus TryWaitRemove(const Maybe<TimeDuration>& aDuration,
                            Args&... aArgs) {
    return TryWaitRemoveImpl(false, aDuration, aArgs...);
  }

  mozilla::ipc::Shmem::SharedMemory* LookupSharedMemory(uint32_t aId) {
    if (!mActor) {
      return nullptr;
    }
    return mActor->LookupSharedMemory(aId);
  }

 protected:
  friend ProducerConsumerQueue;
  friend ConsumerView<PcqConsumer>;

  template <typename... Args>
  QueueStatus TryRemoveImpl(Args&... aArgs) {
    size_t write = mWrite->load(std::memory_order_acquire);
    size_t read = mRead->load(std::memory_order_relaxed);
    const size_t initRead = read;

    if (!ValidState(read, write)) {
      PCQ_LOGE(
          "Queue was found in an invalid state.  Queue Size: %zu.  "
          "Read: %zu.  Write: %zu",
          Size(), read, write);
      return QueueStatus::kFatalError;
    }

    ConsumerView<PcqConsumer> view(this, &read, write);

    // Check that the queue has enough unoccupied room for all Args types.
    // This is based on the user's size estimate for Args from QueueParamTraits.
    size_t bytesNeeded = detail::MinSizeofArgs(view, aArgs...);

    if (Size() < bytesNeeded) {
      PCQ_LOGE(
          "Queue is too small for objects.  Queue Size: %zu.  "
          "Bytes needed: %zu.",
          Size(), bytesNeeded);
      return QueueStatus::kTooSmall;
    }

    if (UsedBytes(read, write) < bytesNeeded) {
      PCQ_LOGD(
          "Not enough data in queue.  Has: %zu (%zu,%zu).  "
          "Bytes needed: %zu",
          UsedBytes(read, write), read, write, bytesNeeded);
      return QueueStatus::kNotReady;
    }

    // Only update the queue if the operation was successful.
    QueueStatus status = TryRemoveArgs(view, aArgs...);
    if (!status) {
      return status;
    }

    // Check that at least bytesNeeded were consumed.  Failing this means
    // that some QueueParamTraits::MinSize estimated too many bytes.
    bool enoughBytes =
        FreeBytes(read, write) >=
        FreeBytes((initRead + bytesNeeded) % QueueBufferSize(), write);
    MOZ_ASSERT(enoughBytes);
    if (!enoughBytes) {
      return QueueStatus::kFatalError;
    }

    MOZ_ASSERT(ValidState(read, write));

    PCQ_LOGD(
        "Successfully removed.  PcqConsumer used %zu bytes total.  "
        "Read index: %zu -> %zu",
        bytesNeeded, initRead, read);

    // Commit the transaction.
    mRead->store(read, std::memory_order_release);
    // Set the semaphore (unless it is already set) to let the producer know
    // that the queue may not be full.  We just need to guarantee that it
    // was set (i.e. non-zero) at some time after mRead was updated.
    if (!mMaybeNotFullSem->IsAvailable()) {
      mMaybeNotFullSem->Signal();
    }
    return status;
  }

  template <typename... Args>
  QueueStatus TryWaitRemoveImpl(bool aRecursed,
                                const Maybe<TimeDuration>& aDuration,
                                Args&... aArgs) {
    // Wait up to aDuration for the not-empty semaphore to be signaled.
    // If we run out of time then quit.
    TimeStamp start(TimeStamp::Now());
    if (aRecursed && (!mMaybeNotEmptySem->Wait(aDuration))) {
      return QueueStatus::kNotReady;
    }

    // Attempt to read all args.  No waiting is done here.
    QueueStatus status = TryRemove(aArgs...);

    TimeStamp now;
    if (aRecursed && IsSuccess(status)) {
      // If our local view of the queue is that it is still not empty then
      // we know it won't get empty without us (we are the only consumer).
      // So re-set the not-empty semaphore unless it's already set.
      // (We are also the only not-empty semaphore decrementer so it can't
      // become 0.)
      if ((!IsEmpty()) && (!mMaybeNotEmptySem->IsAvailable())) {
        mMaybeNotEmptySem->Signal();
      }
    } else if ((status == QueueStatus::kNotReady) &&
               (aDuration.isNothing() ||
                ((now = TimeStamp::Now()) - start) < aDuration.value())) {
      // We don't have enough data but still have time, e.g. because
      // the producer wrote some data but not enough or because the
      // not-empty semaphore gave a false positive.  Either way, retry.
      status =
          aDuration.isNothing()
              ? TryWaitRemoveImpl(true, aDuration, aArgs...)
              : TryWaitRemoveImpl(true, Some(aDuration.value() - (now - start)),
                                  aArgs...);
    }

    return status;
  }

  // Version of the helper for copying values out of the queue.
  template <typename... Args>
  QueueStatus TryRemoveArgs(ConsumerView<PcqConsumer>& aView, Args&... aArgs);

  template <typename Arg, typename... Args>
  QueueStatus TryRemoveArgs(ConsumerView<PcqConsumer>& aView, Arg& aArg,
                            Args&... aArgs) {
    QueueStatus status = TryCopyItem(aView, aArg);
    return IsSuccess(status) ? TryRemoveArgs(aView, aArgs...) : status;
  }

  QueueStatus TryRemoveArgs(ConsumerView<PcqConsumer>&) {
    return QueueStatus::kSuccess;
  }

  // If an item is available then it is copied into aArg.  The item is skipped
  // over if aArg is null.
  template <typename Arg>
  QueueStatus TryCopyItem(ConsumerView<PcqConsumer>& aView, Arg& aArg) {
    MOZ_ASSERT(aArg);
    return QueueParamTraits<typename RemoveCVR<Arg>::Type>::Read(
        aView, const_cast<std::remove_cv_t<Arg>*>(&aArg));
  }

  template <typename Arg>
  QueueStatus ReadObject(size_t* aRead, size_t aWrite, Arg* arg,
                         size_t aArgSize) {
    return Marshaller::ReadObject(mQueue, QueueBufferSize(), aRead, aWrite, arg,
                                  aArgSize);
  }

  // Currently, the PCQ requires any parameters expected to need more than
  // 1/16 the total number of bytes in the command queue to use their own
  // SharedMemory.
  bool NeedsSharedMemory(size_t aRequested) {
    return (Size() / 16) < aRequested;
  }

  PcqConsumer(Shmem& aShmem, IProtocol* aProtocol, size_t aQueueSize,
              RefPtr<detail::PcqRCSemaphore> aMaybeNotEmptySem,
              RefPtr<detail::PcqRCSemaphore> aMaybeNotFullSem)
      : PcqBase(aShmem, aProtocol, aQueueSize, aMaybeNotEmptySem,
                aMaybeNotFullSem) {}

  PcqConsumer(const PcqConsumer&) = delete;
  PcqConsumer& operator=(const PcqConsumer&) = delete;
};

using mozilla::detail::GetCacheLineSize;
using mozilla::detail::GetMaxHeaderSize;

/**
 * A single producer + single consumer queue, implemented as a
 * circular queue.  The object is backed with a Shmem, which allows
 * it to be used across processes.
 *
 * This is a single-producer/single-consumer queue.  Another way of saying that
 * is to say that the PcqProducer and PcqConsumer objects are not thread-safe.
 */
class ProducerConsumerQueue {
 public:
  /**
   * Create a queue whose endpoints are the same as those of aProtocol.
   * In choosing a queueSize, be aware that both the queue and the Shmem will
   * allocate additional shared memory for internal accounting (see
   * GetMaxHeaderSize) and that Shmem sizes are a multiple of the operating
   * system's page sizes.
   *
   * aAdditionalBytes of shared memory will also be allocated.
   * Clients may use this shared memory for their own purposes.
   * See GetUserReservedMemory() and GetUserReservedMemorySize()
   */
  static UniquePtr<ProducerConsumerQueue> Create(IProtocol* aProtocol,
                                                 size_t aQueueSize,
                                                 size_t aAdditionalBytes = 0) {
    MOZ_ASSERT(aProtocol);
    // Protocol must subclass PcqActor
    MOZ_ASSERT(PcqActor::LookupProtocol(aProtocol));
    Shmem shmem;

    // NB: We need one extra byte for the queue contents (hence the "+1").
    uint32_t totalShmemSize =
        aQueueSize + 1 + GetMaxHeaderSize() + aAdditionalBytes;

    if (!aProtocol->AllocUnsafeShmem(
            totalShmemSize, mozilla::ipc::SharedMemory::TYPE_BASIC, &shmem)) {
      return nullptr;
    }

    // NB: We need one extra byte for the queue contents (hence the "+1").
    if ((!shmem.IsWritable()) || (!shmem.IsReadable()) ||
        ((GetMaxHeaderSize() + aQueueSize + 1) > totalShmemSize)) {
      return nullptr;
    }

    return WrapUnique(new ProducerConsumerQueue(shmem, aProtocol, aQueueSize,
                                                aAdditionalBytes));
  }

  /**
   * The queue needs a few bytes for 2 shared counters.  It takes these from the
   * underlying Shmem.  This will still work if the cache line size is incorrect
   * for some architecture but operations may be less efficient.
   */
  static constexpr size_t GetMaxHeaderSize() {
    return mozilla::detail::GetMaxHeaderSize();
  }

  /**
   * Cache line size for the machine.  We assume a 64-byte cache line size.
   */
  static constexpr size_t GetCacheLineSize() {
    return mozilla::detail::GetCacheLineSize();
  }

  using Producer = PcqProducer;
  using Consumer = PcqConsumer;

  UniquePtr<Producer> TakeProducer() { return std::move(mProducer); }
  UniquePtr<Consumer> TakeConsumer() { return std::move(mConsumer); }

 private:
  ProducerConsumerQueue(Shmem& aShmem, IProtocol* aProtocol, size_t aQueueSize,
                        size_t aAdditionalBytes) {
    auto notempty = MakeRefPtr<detail::PcqRCSemaphore>(
        CrossProcessSemaphore::Create("webgl-notempty", 0));
    auto notfull = MakeRefPtr<detail::PcqRCSemaphore>(
        CrossProcessSemaphore::Create("webgl-notfull", 1));

    mProducer = WrapUnique(
        new Producer(aShmem, aProtocol, aQueueSize, notempty, notfull));
    mConsumer = WrapUnique(
        new Consumer(aShmem, aProtocol, aQueueSize, notempty, notfull));

    // The system may have reserved more bytes than the user asked for.
    // Make sure they aren't given access to the extra.
    MOZ_ASSERT(mProducer->mUserReservedSize >= aAdditionalBytes);
    mProducer->mUserReservedSize = aAdditionalBytes;
    mConsumer->mUserReservedSize = aAdditionalBytes;
    if (aAdditionalBytes == 0) {
      mProducer->mUserReservedMemory = nullptr;
      mConsumer->mUserReservedMemory = nullptr;
    }

    PCQ_LOGD(
        "Constructed PCQ (%p).  Shmem Size = %zu. Queue Size = %zu.  "
        "Other process ID: %08x.",
        this, aShmem.Size<uint8_t>(), aQueueSize,
        (uint32_t)aProtocol->OtherPid());
  }

  UniquePtr<Producer> mProducer;
  UniquePtr<Consumer> mConsumer;
};

}  // namespace webgl

namespace ipc {

template <>
struct IPDLParamTraits<mozilla::detail::PcqBase> {
  typedef mozilla::detail::PcqBase paramType;

  static void Write(IPC::Message* aMsg, IProtocol* aActor, paramType& aParam) {
    // Must be sent using the queue's underlying actor, which must still exist!
    MOZ_RELEASE_ASSERT(aParam.mActor && aActor->Id() == aParam.mActor->Id());
    WriteIPDLParam(aMsg, aActor, aParam.mActor->Id());
    WriteIPDLParam(aMsg, aActor, aParam.QueueSize());
    WriteIPDLParam(aMsg, aActor, std::move(aParam.mShmem));

    // May not currently share a PcqProducer or PcqConsumer with a process that
    // it's Shmem is not related to.
    MOZ_ASSERT(aActor->OtherPid() == aParam.mOtherPid);
    WriteIPDLParam(
        aMsg, aActor,
        aParam.mMaybeNotEmptySem->ShareToProcess(aActor->OtherPid()));

    WriteIPDLParam(aMsg, aActor,
                   aParam.mMaybeNotFullSem->ShareToProcess(aActor->OtherPid()));
  }

  static bool Read(const IPC::Message* aMsg, PickleIterator* aIter,
                   IProtocol* aActor, paramType* aResult) {
    int32_t iProtocolId;
    size_t queueSize;
    Shmem shmem;
    CrossProcessSemaphoreHandle notEmptyHandle;
    CrossProcessSemaphoreHandle notFullHandle;

    if (!ReadIPDLParam(aMsg, aIter, aActor, &iProtocolId) ||
        (iProtocolId != aActor->Id()) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &queueSize) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &shmem) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &notEmptyHandle) ||
        !ReadIPDLParam(aMsg, aIter, aActor, &notFullHandle)) {
      return false;
    }

    MOZ_ASSERT(IsHandleValid(notEmptyHandle) && IsHandleValid(notFullHandle));
    aResult->Set(shmem, aActor, queueSize,
                 MakeRefPtr<detail::PcqRCSemaphore>(
                     CrossProcessSemaphore::Create(notEmptyHandle)),
                 MakeRefPtr<detail::PcqRCSemaphore>(
                     CrossProcessSemaphore::Create(notFullHandle)));
    return true;
  }

  static void Log(const paramType& aParam, std::wstring* aLog) {
    IPDLParamTraits<Shmem>::Log(aParam.mShmem, aLog);
  }
};

template <>
struct IPDLParamTraits<mozilla::webgl::PcqProducer>
    : public IPDLParamTraits<mozilla::detail::PcqBase> {
  typedef mozilla::webgl::PcqProducer paramType;
};

template <>
struct IPDLParamTraits<mozilla::webgl::PcqConsumer>
    : public IPDLParamTraits<mozilla::detail::PcqBase> {
  typedef mozilla::webgl::PcqConsumer paramType;
};

}  // namespace ipc
}  // namespace mozilla

#endif  // mozilla_ipc_ProducerConsumerQueue_h
