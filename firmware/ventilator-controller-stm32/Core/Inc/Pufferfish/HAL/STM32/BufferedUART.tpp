/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: Ethan Li
 */

#include "BufferedUART.h"

namespace Pufferfish {
namespace HAL {

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
BufferedUART<RXBufferSize, TXBufferSize>::BufferedUART(
    UART_HandleTypeDef &huart
) : huart(huart) {}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
BufferReadStatus BufferedUART<RXBufferSize, TXBufferSize>::read(
    uint8_t &readByte
) volatile {
  return rxBuffer.read(readByte);
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
BufferWriteStatus BufferedUART<RXBufferSize, TXBufferSize>::write(
    uint8_t writeByte
) volatile {
  BufferWriteStatus status = txBuffer.write(writeByte);
  __HAL_UART_ENABLE_IT(&huart, UART_IT_TXE); // write a byte on the next TX empty interrupt
  return status;
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
AtomicSize BufferedUART<RXBufferSize, TXBufferSize>::write(
    const uint8_t *writeBytes, AtomicSize writeSize
) volatile {
  AtomicSize i;
  for (i = 0; i < writeSize; ++i) {
    if (write(writeBytes[i]) != BufferWriteStatus::ok) {
      break;
    }
  }
  return i;
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
BufferWriteStatus BufferedUART<RXBufferSize, TXBufferSize>::writeBlock(
    uint8_t writeByte, uint32_t timeout
) volatile {
  uint32_t start = HAL_GetTick();
  while (true) {
    if (write(writeByte) == BufferWriteStatus::ok) {
      return BufferWriteStatus::ok;
    }
    if ((timeout > 0) && ((HAL_GetTick() - start) > timeout)) {
      return BufferWriteStatus::full;
    }
  }
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
AtomicSize BufferedUART<RXBufferSize, TXBufferSize>::writeBlock(
    const uint8_t *writeBytes, AtomicSize writeSize, uint32_t timeout
) volatile {
  AtomicSize written = 0;
  uint32_t start = HAL_GetTick();
  while (written < writeSize) {
    written += write(writeBytes + written, writeSize - written);
    if ((timeout > 0) && ((HAL_GetTick() - start) > timeout)) {
      break;
    }
  }
  return written;
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
void BufferedUART<RXBufferSize, TXBufferSize>::setupIRQ() volatile {
  UART_MASK_COMPUTATION(&huart);
  __HAL_UART_ENABLE_IT(&huart, UART_IT_RXNE);
  // We only enable TXE when after we write to txBuffer
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
void BufferedUART<RXBufferSize, TXBufferSize>::handleIRQ() volatile {
  handleIRQRX();
  handleIRQTX();
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
void BufferedUART<RXBufferSize, TXBufferSize>::handleIRQRX() volatile {
  bool rxneEnabled = __HAL_UART_GET_IT_SOURCE(&huart, UART_IT_RXNE) != RESET;
  bool rxneFlagged = __HAL_UART_GET_FLAG(&huart, UART_FLAG_RXNE) != RESET;
  if (!rxneEnabled|| !rxneFlagged) { // check for RX not empty interrupt
    return;
  }

  uint8_t rxByte = static_cast<uint8_t>(huart.Instance->RDR & huart.Mask); // assumes 8-bit byte
  rxBuffer.write(rxByte); // but do nothing if the buffer is full
  __HAL_UART_SEND_REQ(&huart, UART_RXDATA_FLUSH_REQUEST); // clear RXNE flag
}

template <AtomicSize RXBufferSize, AtomicSize TXBufferSize>
void BufferedUART<RXBufferSize, TXBufferSize>::handleIRQTX() volatile {
  bool txeEnabled = __HAL_UART_GET_IT_SOURCE(&huart, UART_IT_TXE) != RESET;
  bool txeFlagged = __HAL_UART_GET_FLAG(&huart, UART_FLAG_TXE) != RESET;
  if (!txeEnabled|| !txeFlagged) { // check for TX empty interrupt
    return;
  }

  uint8_t txByte;
  if (txBuffer.read(txByte) == BufferReadStatus::empty) {
    // stop receiving TX empty interrupts until we have more data for TX
    __HAL_UART_DISABLE_IT(&huart, UART_IT_TXE);
    return;
  }

  huart.Instance->TDR = txByte;
}

} // namespace HAL
} // namespace Pufferfish
