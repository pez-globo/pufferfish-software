/// \file
/// \brief An interrupt-safe stream backed by a circular buffer.

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

#include <cstdint>

#include "Pufferfish/HAL/Types.h"
#include "Pufferfish/Statuses.h"

namespace Pufferfish::Util {

/**
 * Byte stream with non-blocking queue interface and static allocation.
 *
 * Inspired by
 * https://hackaday.com/2015/10/29/embed-with-elliot-going-round-with-circular-buffers/
 * This class provides a bounded-length queue data structure which is
 * statically allocated. Behind the scenes, it is backed by an array.
 * BufferSize is recommended to be a power of two for compiler optimization.
 * Volatile methods are declared so that this can be usable with ISRs.
 * It is interrupt-safe (i.e. usable with ISRs) if the element type is uint8_t;
 * otherwise, no guarantees are made.
 */
template <HAL::AtomicSize buffer_size, typename ElementType>
class RingBuffer {
 public:
  RingBuffer();

  /**
   * Attempt to "pop" an element from the head of the queue.
   *
   * Gives up without causing any side-effects if the queue is empty;
   * if it gives up, read_element will be left unmodified.
   * @param[out] read_element the element popped from the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus pop(ElementType &read_element) volatile;

  /**
   * Attempt to "pop" an element from the head of the queue.
   *
   * Gives up without causing any side-effects if the queue is empty;
   * if it gives up, read_element will be left unmodified.
   * @param[out] read_element the element popped from the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus pop(ElementType &read_element);

  /**
   * Attempt to "peek" at the element at the head of the queue.
   *
   * Gives up without causing any side-effects if the queue is empty;
   * if it gives up, peek_element will be left unmodified.
   * @param[out] peek_element the byte at the head of the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus peek(ElementType &peek_element) const volatile;

  /**
   * Attempt to "peek" at the element at some offset after the head of the queue.
   *
   * Not interrupt-safe! Gives up without causing any side-effects if
   * no element exists at the specified offset; if it gives up, peek_element
   * will be left unmodified.
   * @param offset how many elements beyond the head to "look ahead"
   * @param[out] peek_element the byte at the head of the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus peek(ElementType &peek_element, size_t offset = 0) const;

  /**
   * Attempt to "push" the provided element onto the tail of the queue.
   *
   * Gives up without causing any side-effects if the queue is full.
   * @param writeByte the byte to push onto the tail of the queue
   * @return ok on success, full otherwise
   */
  BufferStatus push(const ElementType &write_element) volatile;

  /**
   * Attempt to "push" the provided element onto the tail of the queue.
   *
   * Gives up without causing any side-effects if the queue is full.
   * @param writeByte the byte to push onto the tail of the queue
   * @return ok on success, full otherwise
   */
  BufferStatus push(const ElementType &write_element);

  /**
   * Report the number of elements in the buffer.
   *
   * Not interrupt-safe! So a volatile version is not provided.
   * @return number of elements in the buffer
   */
  size_t size() const;

 private:
  // We have to use a C-style array because std::array doesn't work with
  // volatile
  // NOLINTNEXTLINE(modernize-avoid-c-arrays)
  ElementType buffer_[buffer_size];

  HAL::AtomicSize newest_index_ = 0;
  HAL::AtomicSize oldest_index_ = 0;
};

}  // namespace Pufferfish::Util

#include "RingBuffer.tpp"
