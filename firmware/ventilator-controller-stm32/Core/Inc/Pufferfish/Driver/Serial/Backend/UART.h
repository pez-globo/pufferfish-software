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
  Status output(Application::Message &output_message);

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

  Status input(const Application::Message &input_message);

 private:
  static const uint32_t write_timeout = 10000;

  volatile BufferedUART &uart_;
  BackendSender &serial_;
};

template <typename BufferedUART>
class UARTBackendDriver {
 public:
  using Receiver = UARTBackendReceiver<BufferedUART>;
  using Sender = UARTBackendSender<BufferedUART>;

  UARTBackendDriver(volatile BufferedUART &uart, HAL::CRC32C &crc32c)
      : crc32c_(crc32c),
        uart_(uart),
        receiver_protocol_(crc32c),
        sender_protocol_(crc32c),
        receiver_(uart, receiver_protocol_),
        sender_(uart, sender_protocol_) {}

  void setup_irq();
  typename Receiver::Status receive(Application::Message &receive_message);
  typename Sender::Status send(const Application::Message &send_message);

 private:
  HAL::CRC32C &crc32c_;
  volatile BufferedUART &uart_;
  BackendReceiver receiver_protocol_;
  BackendSender sender_protocol_;
  UARTBackendReceiver<BufferedUART> receiver_;
  UARTBackendSender<BufferedUART> sender_;
};

class UARTBackend {
 public:
  using Driver = UARTBackendDriver<HAL::LargeBufferedUART>;

  UARTBackend(
      volatile HAL::LargeBufferedUART &uart, HAL::CRC32C &crc32c, Application::States &states)
      : driver_(uart, crc32c), synchronizer_(states, state_sync_schedule){};

  void setup_irq();
  void receive();
  void update_clock(uint32_t current_time);
  void send();

 private:
  using StateSynchronizer = Protocols::StateSynchronizer<
      Application::States,
      Application::Message,
      Application::MessageTypes,
      state_sync_schedule.size()>;

  Driver driver_;
  StateSynchronizer synchronizer_;
};

}  // namespace Pufferfish::Driver::Serial::Backend

#include "UART.tpp"
