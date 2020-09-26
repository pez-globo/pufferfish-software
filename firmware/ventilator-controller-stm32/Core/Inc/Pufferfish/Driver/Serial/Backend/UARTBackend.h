/*
 * UARTSerial.h
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#ifndef INC_PUFFERFISH_UARTSERIAL_H_
#define INC_PUFFERFISH_UARTSERIAL_H_

#include "Pufferfish/HAL/BufferedUART.h"
#include "Pufferfish/HAL/CRC.h"
#include "Pufferfish/Protocols/Serial.h"
#include "Pufferfish/Protocols/Application.h"

namespace Pufferfish {

template<class BufferedUART>
class UARTSerialReceiver {
public:
  enum class Status {available = 0, waiting, invalid};

  UARTSerialReceiver(
      volatile BufferedUART &uart, Protocols::SerialReceiver &serial
  );

  // The return value indicates whether an output is available and written to outputBuffer
  Status output(Protocols::ApplicationMessage &outputMessage);

protected:
  volatile BufferedUART &uart;
  Protocols::SerialReceiver &serial;

  void input();
};

template<class BufferedUART>
class UARTSerialSender {
public:
  enum class Status {ok = 0, invalid};

  UARTSerialSender(
        volatile BufferedUART &uart, Protocols::SerialSender &serial
    );

  Status input(const Protocols::ApplicationMessage &inputMessage);

private:
  volatile BufferedUART &uart;
  Protocols::SerialSender &serial;
};

template<class BufferedUART>
class UARTSerialDriver {
public:
  using Receiver = UARTSerialReceiver<BufferedUART>;
  using Sender = UARTSerialSender<BufferedUART>;

  UARTSerialDriver(volatile BufferedUART &uart, HAL::CRC32C &crc32c);

  volatile BufferedUART &uart;

  void setupIRQ();
  typename Receiver::Status receive(Protocols::ApplicationMessage &receiveMessage);
  typename Sender::Status send(const Protocols::ApplicationMessage &sendMessage);

protected:
  HAL::CRC32C &crc32c;
  Protocols::SerialReceiver receiverProtocol;
  Protocols::SerialSender senderProtocol;
  UARTSerialReceiver<BufferedUART> receiver;
  UARTSerialSender<BufferedUART> sender;
};

class UARTStateSynchronizer {
public:
  using Driver = UARTSerialDriver<HAL::LargeBufferedUART>;

  UARTStateSynchronizer(
      volatile HAL::LargeBufferedUART &uart, HAL::CRC32C &crc32c,
      Protocols::ApplicationStates &states
  );

  void setupIRQ();
  void receive();
  void updateClock(uint32_t currentTime);
  void send();

protected:
  Driver driver;
  Protocols::ApplicationStates &states;
  Protocols::StateSynchronizer synchronizer;
};

}

#include <Pufferfish/UARTSerial.tpp>

#endif /* INC_PUFFERFISH_UARTSERIAL_H_ */
