/*
 * Copyright (C) 2010 The Android Open Source Project
 * Copyright (C) 2014 Mozilla Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NATIVEWINDOW_GONKCONSUMERBASE_M_H
#define NATIVEWINDOW_GONKCONSUMERBASE_M_H

#include <ui/GraphicBuffer.h>

#include <utils/String8.h>
#include <utils/Vector.h>
#include <utils/threads.h>
#include <gui/IConsumerListener.h>

#include "GonkBufferQueue.h"

namespace android {
// ----------------------------------------------------------------------------

class String8;

// GonkConsumerBase is a base class for GonkBufferQueue consumer end-points. It
// handles common tasks like management of the connection to the GonkBufferQueue
// and the buffer pool.
class GonkConsumerBase : public virtual RefBase,
        protected ConsumerListener {
public:
    struct FrameAvailableListener : public virtual RefBase {
        // onFrameAvailable() is called each time an additional frame becomes
        // available for consumption. This means that frames that are queued
        // while in asynchronous mode only trigger the callback if no previous
        // frames are pending. Frames queued while in synchronous mode always
        // trigger the callback.
        //
        // This is called without any lock held and can be called concurrently
        // by multiple threads.
        virtual void onFrameAvailable() = 0;
    };

    virtual ~GonkConsumerBase();

    // abandon frees all the buffers and puts the GonkConsumerBase into the
    // 'abandoned' state.  Once put in this state the GonkConsumerBase can never
    // leave it.  When in the 'abandoned' state, all methods of the
    // IGraphicBufferProducer interface will fail with the NO_INIT error.
    //
    // Note that while calling this method causes all the buffers to be freed
    // from the perspective of the the GonkConsumerBase, if there are additional
    // references on the buffers (e.g. if a buffer is referenced by a client
    // or by OpenGL ES as a texture) then those buffer will remain allocated.
    void abandon();

    // set the name of the GonkConsumerBase that will be used to identify it in
    // log messages.
    void setName(const String8& name);

    // dump writes the current state to a string. Child classes should add
    // their state to the dump by overriding the dumpLocked method, which is
    // called by these methods after locking the mutex.
    void dump(String8& result) const;
    void dump(String8& result, const char* prefix) const;

    // setFrameAvailableListener sets the listener object that will be notified
    // when a new frame becomes available.
    void setFrameAvailableListener(const wp<FrameAvailableListener>& listener);

private:
    GonkConsumerBase(const GonkConsumerBase&);
    void operator=(const GonkConsumerBase&);

protected:
    // GonkConsumerBase constructs a new GonkConsumerBase object to consume image
    // buffers from the given IGonkGraphicBufferConsumer.
    // The controlledByApp flag indicates that this consumer is under the application's
    // control.
    explicit GonkConsumerBase(const sp<IGonkGraphicBufferConsumer>& consumer, bool controlledByApp = false);

    // onLastStrongRef gets called by RefBase just before the dtor of the most
    // derived class.  It is used to clean up the buffers so that GonkConsumerBase
    // can coordinate the clean-up by calling into virtual methods implemented
    // by the derived classes.  This would not be possible from the
    // ConsuemrBase dtor because by the time that gets called the derived
    // classes have already been destructed.
    //
    // This methods should not need to be overridden by derived classes, but
    // if they are overridden the GonkConsumerBase implementation must be called
    // from the derived class.
    virtual void onLastStrongRef(const void* id);

    // Implementation of the IConsumerListener interface.  These
    // calls are used to notify the GonkConsumerBase of asynchronous events in the
    // GonkBufferQueue.  The onFrameAvailable and onBuffersReleased methods should
    // not need to be overridden by derived classes, but if they are overridden
    // the GonkConsumerBase implementation must be called from the derived class.
    // The GonkConsumerBase version of onSidebandStreamChanged does nothing and can
    // be overriden by derived classes if they want the notification.
    virtual void onFrameAvailable(const ::android::BufferItem& item);
    virtual void onFrameReplaced(const ::android::BufferItem& item) {};
    virtual void onBuffersReleased();
    virtual void onSidebandStreamChanged();

    // freeBufferLocked frees up the given buffer slot.  If the slot has been
    // initialized this will release the reference to the GraphicBuffer in that
    // slot.  Otherwise it has no effect.
    //
    // Derived classes should override this method to clean up any state they
    // keep per slot.  If it is overridden, the derived class's implementation
    // must call GonkConsumerBase::freeBufferLocked.
    //
    // This method must be called with mMutex locked.
    virtual void freeBufferLocked(int slotIndex);

    // abandonLocked puts the GonkBufferQueue into the abandoned state, causing
    // all future operations on it to fail. This method rather than the public
    // abandon method should be overridden by child classes to add abandon-
    // time behavior.
    //
    // Derived classes should override this method to clean up any object
    // state they keep (as opposed to per-slot state).  If it is overridden,
    // the derived class's implementation must call GonkConsumerBase::abandonLocked.
    //
    // This method must be called with mMutex locked.
    virtual void abandonLocked();

    // dumpLocked dumps the current state of the GonkConsumerBase object to the
    // result string.  Each line is prefixed with the string pointed to by the
    // prefix argument.  The buffer argument points to a buffer that may be
    // used for intermediate formatting data, and the size of that buffer is
    // indicated by the size argument.
    //
    // Derived classes should override this method to dump their internal
    // state.  If this method is overridden the derived class's implementation
    // should call GonkConsumerBase::dumpLocked.
    //
    // This method must be called with mMutex locked.
    virtual void dumpLocked(String8& result, const char* prefix) const;

    // acquireBufferLocked fetches the next buffer from the GonkBufferQueue and
    // updates the buffer slot for the buffer returned.
    //
    // Derived classes should override this method to perform any
    // initialization that must take place the first time a buffer is assigned
    // to a slot.  If it is overridden the derived class's implementation must
    // call GonkConsumerBase::acquireBufferLocked.
    virtual status_t acquireBufferLocked(IGonkGraphicBufferConsumer::BufferItem *item,
        nsecs_t presentWhen);

    // releaseBufferLocked relinquishes control over a buffer, returning that
    // control to the GonkBufferQueue.
    //
    // Derived classes should override this method to perform any cleanup that
    // must take place when a buffer is released back to the GonkBufferQueue.  If
    // it is overridden the derived class's implementation must call
    // GonkConsumerBase::releaseBufferLocked.
    virtual status_t releaseBufferLocked(int slot, const sp<GraphicBuffer> graphicBuffer);

    // returns true iff the slot still has the graphicBuffer in it.
    bool stillTracking(int slot, const sp<GraphicBuffer> graphicBuffer);

    // addReleaseFence* adds the sync points associated with a fence to the set
    // of sync points that must be reached before the buffer in the given slot
    // may be used after the slot has been released.  This should be called by
    // derived classes each time some asynchronous work is kicked off that
    // references the buffer.
    status_t addReleaseFence(int slot,
            const sp<GraphicBuffer> graphicBuffer, const sp<Fence>& fence);
    status_t addReleaseFenceLocked(int slot,
            const sp<GraphicBuffer> graphicBuffer, const sp<Fence>& fence);

    // Slot contains the information and object references that
    // GonkConsumerBase maintains about a GonkBufferQueue buffer slot.
    struct Slot {
        // mGraphicBuffer is the Gralloc buffer store in the slot or NULL if
        // no Gralloc buffer is in the slot.
        sp<GraphicBuffer> mGraphicBuffer;

        // mFence is a fence which will signal when the buffer associated with
        // this buffer slot is no longer being used by the consumer and can be
        // overwritten. The buffer can be dequeued before the fence signals;
        // the producer is responsible for delaying writes until it signals.
        sp<Fence> mFence;

        // the frame number of the last acquired frame for this slot
        uint64_t mFrameNumber;
    };

    // mSlots stores the buffers that have been allocated by the GonkBufferQueue
    // for each buffer slot.  It is initialized to null pointers, and gets
    // filled in with the result of GonkBufferQueue::acquire when the
    // client dequeues a buffer from a
    // slot that has not yet been used. The buffer allocated to a slot will also
    // be replaced if the requested buffer usage or geometry differs from that
    // of the buffer allocated to a slot.
    Slot mSlots[GonkBufferQueue::NUM_BUFFER_SLOTS];

    // mAbandoned indicates that the GonkBufferQueue will no longer be used to
    // consume images buffers pushed to it using the IGraphicBufferProducer
    // interface. It is initialized to false, and set to true in the abandon
    // method.  A GonkBufferQueue that has been abandoned will return the NO_INIT
    // error from all IConsumerBase methods capable of returning an error.
    bool mAbandoned;

    // mName is a string used to identify the GonkConsumerBase in log messages.
    // It can be set by the setName method.
    String8 mName;

    // mFrameAvailableListener is the listener object that will be called when a
    // new frame becomes available. If it is not NULL it will be called from
    // queueBuffer.
    wp<FrameAvailableListener> mFrameAvailableListener;

    // The GonkConsumerBase has-a GonkBufferQueue and is responsible for creating this object
    // if none is supplied
    sp<IGonkGraphicBufferConsumer> mConsumer;

    // mMutex is the mutex used to prevent concurrent access to the member
    // variables of GonkConsumerBase objects. It must be locked whenever the
    // member variables are accessed or when any of the *Locked methods are
    // called.
    //
    // This mutex is intended to be locked by derived classes.
    mutable Mutex mMutex;
};

// ----------------------------------------------------------------------------
}; // namespace android

#endif // NATIVEWINDOW_GONKCONSUMERBASE_LL_H
