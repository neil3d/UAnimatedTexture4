/**
 * Copyright 2019 Neil Fang. All Rights Reserved.
 *
 * Animated Texture from GIF file
 *
 * Created by Neil Fang
 * GitHub Repo: https://github.com/neil3d/UnrealAnimatedTexturePlugin
 * GitHub Page: http://neil3d.github.io
 *
 */

#pragma once
#include <string>
#include <vector>

extern "C"
{
#include "giflib/gif_lib.h"
}

/**
 * GIFLIB decoder wrapper class
 *
 */
class FAnimatedGIF {
 public:
  struct Pixel {
    uint8_t B;
    uint8_t G;
    uint8_t R;
    uint8_t A;
  };
  FAnimatedGIF() = default;
  ~FAnimatedGIF();

  bool loadFromMemory(uint8_t* buffer, size_t size);
  void close();

  /**
   * @return frame delay in milliseconds
   */
  int playFrame(int defaultFrameDelay, bool bLooping);
  void reset() {
      mCurrentFrame = 0; mLoopCount = 0; mDoNotDispose = false;
  }

  int getWidth() const;
  int getHeight() const;
  const Pixel* getFrameBuffer() const;

  int getDuration(int defaultFrameDelay) const;
  bool supportsTransparency() const;

 private:
  void _clearFrameBuffer(ColorMapObject* colorMap, bool bTransparent);
  void _GCB_Background(int left, int top, int width, int height,
                       ColorMapObject* colorMap, bool bTransparent);

 private:
  int mCurrentFrame = 0;
  int mLoopCount = 0;
  bool mDoNotDispose = false;

  GifFileType* mGIF = nullptr;
  std::vector<Pixel> mFrameBuffer;
 public:
  FAnimatedGIF(const FAnimatedGIF&) = delete;
  FAnimatedGIF& operator=(const FAnimatedGIF&) = delete;
};