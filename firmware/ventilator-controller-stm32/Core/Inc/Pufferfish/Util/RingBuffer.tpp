/// \file
/// \brief An interrupt-safe byte stream backed by a circular buffer.

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

// This is an adaptation of code published at
// https://github.com/hexagon5un/embed_with_elliot-circular_buffer
// by Elliot Williams under the MIT License. This implementation makes
// significant implementations to provide a C++ interface.
// Third-party works included this file are licensed as follows:
//
// Copyright (c) 2015 Elliot Williams
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <cstring>

#include "RingBuffer.h"

namespace Pufferfish::Util {

template <HAL::AtomicSize buffer_size, typename ElementType>
RingBuffer<buffer_size, ElementType>::RingBuffer() = default;

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::pop(ElementType &read_element) volatile {
  if (newest_index_ == oldest_index_) {
    return BufferStatus::empty;
  }

  read_element = buffer_[oldest_index_];
  oldest_index_ = (oldest_index_ + 1) % buffer_size;
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::pop(ElementType &read_element) {
  if (newest_index_ == oldest_index_) {
    return BufferStatus::empty;
  }

  read_element = buffer_[oldest_index_];
  oldest_index_ = (oldest_index_ + 1) % buffer_size;
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::peek(ElementType &peek_element) const volatile {
  if (newest_index_ == oldest_index_) {
    return BufferStatus::empty;
  }

  peek_element = buffer_[oldest_index_];
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::peek(
    ElementType &peek_element, size_t offset) const {
  HAL::AtomicSize peek_index = oldest_index_;
  if (offset >= buffer_size) {
    return BufferStatus::empty;
  }

  // Advance the peek index until offset is zero
  while (true) {
    if (peek_index == newest_index_) {
      // Offset is greater than the number of elements in the buffer
      return BufferStatus::empty;
    }

    if (!offset) {
      break;
    }

    peek_index = (peek_index + 1) % buffer_size;
    --offset;
  }

  peek_element = buffer_[peek_index];
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::push(const ElementType &write_element) volatile {
  HAL::AtomicSize next_index = (newest_index_ + 1) % buffer_size;
  if (next_index == oldest_index_) {
    return BufferStatus::full;
  }

  buffer_[newest_index_] = write_element;
  newest_index_ = next_index;
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
BufferStatus RingBuffer<buffer_size, ElementType>::push(const ElementType &write_element) {
  HAL::AtomicSize next_index = (newest_index_ + 1) % buffer_size;
  if (next_index == oldest_index_) {
    return BufferStatus::full;
  }

  buffer_[newest_index_] = write_element;
  newest_index_ = next_index;
  return BufferStatus::ok;
}

template <HAL::AtomicSize buffer_size, typename ElementType>
size_t RingBuffer<buffer_size, ElementType>::size() const {
  return (newest_index_ - oldest_index_) % buffer_size;
}

}  // namespace Pufferfish::Util
