/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* vim: set ts=8 sts=2 et sw=2 tw=80: */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef MOZILLA_GFX_TEXTURECLIENT_SHAREDSURFACE_H
#define MOZILLA_GFX_TEXTURECLIENT_SHAREDSURFACE_H

#include <cstddef>           // for size_t
#include <stdint.h>          // for uint32_t, uint8_t, uint64_t
#include "GLContextTypes.h"  // for GLContext (ptr only), etc
#include "TextureClient.h"
#include "mozilla/Assertions.h"              // for MOZ_ASSERT, etc
#include "mozilla/RefPtr.h"                  // for RefPtr, RefCounted
#include "mozilla/gfx/Point.h"               // for IntSize
#include "mozilla/gfx/Types.h"               // for SurfaceFormat
#include "mozilla/layers/CompositorTypes.h"  // for TextureFlags, etc
#include "mozilla/layers/LayersSurfaces.h"   // for SurfaceDescriptor

namespace mozilla {
namespace gl {
class GLContext;
class SharedSurface;
class SurfaceFactory;
}  // namespace gl

namespace layers {

class GrallocTextureData;
class SharedSurfaceTextureClient;

class SharedSurfaceTextureData : public TextureData {
 protected:
  const UniquePtr<gl::SharedSurface> mSurf;

  friend class SharedSurfaceTextureClient;

  explicit SharedSurfaceTextureData(UniquePtr<gl::SharedSurface> surf);

 public:
  virtual ~SharedSurfaceTextureData();

  bool Lock(OpenMode) override { return false; }

  void Unlock() override {}

  void FillInfo(TextureData::Info& aInfo) const override;

  bool Serialize(SurfaceDescriptor& aOutDescriptor) override;

  void Deallocate(LayersIPCChannel*) override;

#ifdef MOZ_WIDGET_GONK
  virtual GrallocTextureData* AsGrallocTextureData() override;
#endif

  gl::SharedSurface* Surf() const { return mSurf.get(); }
};

class SharedSurfaceTextureClient : public TextureClient {
 public:
  SharedSurfaceTextureClient(SharedSurfaceTextureData* aData,
                             TextureFlags aFlags, LayersIPCChannel* aAllocator);

  ~SharedSurfaceTextureClient();

  static already_AddRefed<SharedSurfaceTextureClient> Create(
      UniquePtr<gl::SharedSurface> surf, gl::SurfaceFactory* factory,
      LayersIPCChannel* aAllocator, TextureFlags aFlags);

  gl::SharedSurface* Surf() const {
    return static_cast<const SharedSurfaceTextureData*>(GetInternalData())
        ->Surf();
  }
};

}  // namespace layers
}  // namespace mozilla

#endif  // MOZILLA_GFX_TEXTURECLIENT_SHAREDSURFACE_H
