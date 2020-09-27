/*
 * UARTSerial.h
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "Pufferfish/HAL/STM32/BufferedUART.h"
#include "Pufferfish/HAL/STM32/CRC.h"
#include "Pufferfish/Driver/Serial/Backend/Backend.h"
#include "Pufferfish/Application/States.h"

namespace Pufferfish::Driver::Serial::Backend {

template<class BufferedUART>
class UARTBackendReceiver {
public:
  enum class Status {available = 0, waiting, invalid};

  UARTBackendReceiver(
      volatile BufferedUART &uart, BackendReceiver &serial
  );

  // The return value indicates whether an output is available and written to outputBuffer
  Status output(Application::Message &outputMessage);

protected:
  volatile BufferedUART &uart;
  BackendReceiver &serial;

  void input();
};

template<class BufferedUART>
class UARTBackendSender {
public:
  enum class Status {ok = 0, invalid};

  UARTBackendSender(
        volatile BufferedUART &uart, BackendSender &serial
    );

  Status input(const Application::Message &inputMessage);

private:
  volatile BufferedUART &uart;
  BackendSender &serial;
};

template<class BufferedUART>
class UARTBackendDriver {
public:
  using Receiver = UARTBackendReceiver<BufferedUART>;
  using Sender = UARTBackendSender<BufferedUART>;

  UARTBackendDriver(volatile BufferedUART &uart, HAL::CRC32C &crc32c);

  volatile BufferedUART &uart;

  void setup_irq();
  typename Receiver::Status receive(Application::Message &receiveMessage);
  typename Sender::Status send(const Application::Message &sendMessage);

protected:
  HAL::CRC32C &crc32c;
  BackendReceiver receiverProtocol;
  BackendSender senderProtocol;
  UARTBackendReceiver<BufferedUART> receiver;
  UARTBackendSender<BufferedUART> sender;
};

class UARTBackend {
public:
  using Driver = UARTBackendDriver<HAL::LargeBufferedUART>;

  UARTBackend(
      volatile HAL::LargeBufferedUART &uart, HAL::CRC32C &crc32c,
      Application::States &states
  );

  void setup_irq();
  void receive();
  void update_clock(uint32_t currentTime);
  void send();

protected:
  Driver driver;
  Application::States &states;
  Application::StateSynchronizer synchronizer;
};

}

#include "UART.tpp"
