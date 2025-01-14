/* -*- Mode: c++; c-basic-offset: 2; indent-tabs-mode: nil; tab-width: 4; -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef SURFACE_TYPES_H_
#define SURFACE_TYPES_H_

#include "mozilla/RefPtr.h"
#include "mozilla/Attributes.h"
#include <stdint.h>

namespace mozilla {
namespace layers {
class LayersIPCChannel;
}  // namespace layers

namespace gl {

struct SurfaceCaps final {
  bool any = false;
  bool color = false;
  bool alpha = false;
  bool bpp16 = false;
  bool depth = false;
  bool stencil = false;
  bool premultAlpha = true;
  bool preserve = false;

  // The surface allocator that we want to create this
  // for.  May be null.
  RefPtr<layers::LayersIPCChannel> surfaceAllocator;

  SurfaceCaps();
  SurfaceCaps(const SurfaceCaps& other);
  ~SurfaceCaps();

  void Clear() { *this = {}; }

  SurfaceCaps& operator=(const SurfaceCaps& other);

  // We can't use just 'RGB' here, since it's an ancient Windows macro.
  static SurfaceCaps ForRGB() {
    SurfaceCaps caps;

    caps.color = true;

    return caps;
  }

  static SurfaceCaps ForRGBA() {
    SurfaceCaps caps;

    caps.color = true;
    caps.alpha = true;

    return caps;
  }

  static SurfaceCaps Any() {
    SurfaceCaps caps;

    caps.any = true;

    return caps;
  }
};

enum class SharedSurfaceType : uint8_t {
  Unknown = 0,

  Basic,
  EGLImageShare,
  EGLSurfaceANGLE,
  DXGLInterop,
  DXGLInterop2,
  IOSurface,
  GLXDrawable,
  SharedGLTexture,
  AndroidSurfaceTexture,
  Gralloc,
  EGLSurfaceDMABUF,

  Max
};

enum class AttachmentType : uint8_t {
  Screen = 0,

  GLTexture,
  GLRenderbuffer,

  Max
};

}  // namespace gl

} /* namespace mozilla */

#endif /* SURFACE_TYPES_H_ */
