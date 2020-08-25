/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 *
 *  An interrupt-backed UART I/O endpoint, exposing a buffered read/write interface.
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "Pufferfish/Types.h"
#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/RingBuffer.h"

namespace Pufferfish {
namespace HAL {

// Interrupt-driven UART RX and TX from ring buffers
template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
class BufferedUART {
public:
  BufferedUART(UART_HandleTypeDef &huart);

  BufferStatus read(uint8_t &readByte) volatile;
  BufferStatus write(uint8_t writeByte) volatile;
  BufferStatus write(
      const uint8_t *writeBytes, AtomicSize writeSize,
      HAL::AtomicSize &writtenSize
  ) volatile;
  BufferStatus writeBlock(uint8_t writeByte, uint32_t timeout) volatile;
  BufferStatus writeBlock(
      const uint8_t *writeBytes, AtomicSize writeSize, uint32_t timeout,
      HAL::AtomicSize &writtenSize
  ) volatile;

  void setupIRQ() volatile;
  void handleIRQ() volatile;

  uint32_t rxDropped() const volatile;

protected:
  UART_HandleTypeDef &huart;

  volatile Util::RingBuffer<RXBufferSize> rxBuffer;
  volatile Util::RingBuffer<TXBufferSize> txBuffer;

  void handleIRQRX() volatile;
  void handleIRQTX() volatile;

  volatile uint32_t mRxDropped = 0;
};

using LargeBufferedUART = BufferedUART<4096, 4096>;

} // namespace HAL
} // namespace Pufferfish

#include "BufferedUART.tpp"
