/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  An interrupt-safe byte stream backed by a circular buffer.
 */

#pragma once

#include <stdint.h>
#include "Pufferfish/Statuses.h"
#include "Pufferfish/HAL/Types.h"

namespace Pufferfish {
namespace Util {


/**
 * Circular buffer with non-blocking queue interface.
 *
 * Inspired by https://hackaday.com/2015/10/29/embed-with-elliot-going-round-with-circular-buffers/
 * This class provides a bounded-length queue data structure which is
 * statically allocated. Behind the scenes, it is backed by an array.
 * BufferSize is recommended to be a power of two for compiler optimization.
 * Methods are declared volatile because they are usable with ISRs.
 */
template <HAL::AtomicSize BufferSize>
class RingBuffer {
public:
  RingBuffer();

  static const HAL::AtomicSize maxSize = BufferSize;

  /**
   * Attempt to "pop" a byte from the head of the queue.
   *
   * Gives up without causing any side-effects if the queue is empty;
   * if it gives up, readByte will be left unmodified.
   * @param[out] readByte the byte popped from the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus read(uint8_t &readByte) volatile;

  /**
   * Attempt to "peek" at the byte at the head of the queue.
   *
   * Gives up without causing any side-effects if the queue is empty;
   * if it gives up, peekByte will be left unmodified.
   * @param[out] peekByte the byte at the head of the queue
   * @return ok on success, empty otherwise
   */
  BufferStatus peek(uint8_t &peekByte) const volatile;

  /**
   * Attempt to "push" the provided byte onto the tail of the queue.
   *
   * Gives up without causing any side-effects if the queue is full.
   * @param writeByte the byte to push onto the tail of the queue
   * @return ok on success, full otherwise
   */
  BufferStatus write(uint8_t writeByte) volatile;

protected:
  uint8_t buffer[BufferSize];
  HAL::AtomicSize newestIndex = 0;
  HAL::AtomicSize oldestIndex = 0;
};

} // namespace Util
} // namespace Pufferfish

#include "RingBuffer.tpp"
