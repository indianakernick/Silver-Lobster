//
//  image.hpp
//  Silver Lobster
//
//  Created by Indi Kernick on 25/1/20.
//  Copyright © 2020 Indi Kernick. All rights reserved.
//

#ifndef image_hpp
#define image_hpp

#include <memory>
#include <Graphics/surface.hpp>

template <typename Pixel>
class Image {
public:
  // Constructing a Surface from a null Image results in a null Surface!
  Image()
    : storage{}, width_{}, height_{} {}
  Image(const int width, const int height)
    // TODO: make_unique_default_init
    : storage{new Pixel[width * height]}, width_{width}, height_{height} {}
  Image(const gfx::Size size)
    : Image{size.w, size.h} {}
  
  gfx::Surface<const Pixel> view() const noexcept {
    return {storage.get(), width_, width_, height_};
  }
  gfx::Surface<Pixel> view() noexcept {
    return {storage.get(), width_, width_, height_};
  }
  
  operator gfx::Surface<const Pixel>() const noexcept {
    return view();
  }
  operator gfx::Surface<Pixel>() noexcept {
    return view();
  }
  
private:
  std::unique_ptr<Pixel[]> storage;
  int width_;
  int height_;
};

namespace gfx {

template <typename Pixel>
Surface(const Image<Pixel> &) -> Surface<const Pixel>;

template <typename Pixel>
Surface(Image<Pixel> &) -> Surface<Pixel>;

}

#endif
