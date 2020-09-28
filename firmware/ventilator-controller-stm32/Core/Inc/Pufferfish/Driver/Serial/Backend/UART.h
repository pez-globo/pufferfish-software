/*
 * UARTSerial.h
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/Application/States.h"
#include "Pufferfish/Driver/Serial/Backend/Backend.h"
#include "Pufferfish/HAL/STM32/BufferedUART.h"
#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/Protocols/States.h"

namespace Pufferfish::Driver::Serial::Backend {

template <typename BufferedUART>
class UARTBackendReceiver {
 public:
  enum class Status { available = 0, waiting, invalid };

  UARTBackendReceiver(volatile BufferedUART &uart, BackendReceiver &serial)
      : uart_(uart), serial_(serial) {}

  // The return value indicates whether an output is available and written to outputBuffer
  Status output(BackendMessage &output_message);

 private:
  volatile BufferedUART &uart_;
  BackendReceiver &serial_;

  void input();
};

template <typename BufferedUART>
class UARTBackendSender {
 public:
  enum class Status { ok = 0, invalid };

  UARTBackendSender(volatile BufferedUART &uart, BackendSender &serial)
      : uart_(uart), serial_(serial) {}

  Status input(const BackendMessage &input_message);

 private:
  static const uint32_t write_timeout = 10000;

  volatile BufferedUART &uart_;
  BackendSender &serial_;
};

class UARTBackend {
 public:
  using BufferedUART = HAL::LargeBufferedUART;
  using Receiver = UARTBackendReceiver<BufferedUART>;
  using Sender = UARTBackendSender<BufferedUART>;

  UARTBackend(volatile BufferedUART &uart, HAL::CRC32C &crc32c, Application::States &states)
      : crc32c_(crc32c),
        uart_(uart),
        receiver_protocol_(crc32c),
        sender_protocol_(crc32c),
        receiver_(uart, receiver_protocol_),
        sender_(uart, sender_protocol_),
        synchronizer_(states, state_sync_schedule) {}

  void setup_irq();
  void receive();
  void update_clock(uint32_t current_time);
  void send();

 private:
  using StateSynchronizer = Protocols::StateSynchronizer<
      Application::States,
      Application::StateSegment,
      Application::MessageTypes,
      state_sync_schedule.size()>;

  HAL::CRC32C &crc32c_;
  volatile BufferedUART &uart_;
  BackendReceiver receiver_protocol_;
  BackendSender sender_protocol_;
  UARTBackendReceiver<BufferedUART> receiver_;
  UARTBackendSender<BufferedUART> sender_;

  StateSynchronizer synchronizer_;

  typename Receiver::Status receive(BackendMessage &receive_message);
  typename Sender::Status send(const BackendMessage &send_message);
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "UART.tpp"
