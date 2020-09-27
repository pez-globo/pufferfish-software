/*
 * UARTBackend.tpp
 *
 *  Created on: May 20, 2020
 *      Author: Ethan Li
 */

#pragma once

#include "UART.h"

namespace Pufferfish::Driver::Serial::Backend {

// UARTBackendReceiver

template <typename BufferedUART>
typename UARTBackendReceiver<BufferedUART>::Status UARTBackendReceiver<BufferedUART>::output(
    Application::Message &outputMessage) {
  input();
  switch (serial_.output(outputMessage)) {
    case BackendReceiver::OutputStatus::waiting:
      return Status::waiting;
    case BackendReceiver::OutputStatus::invalid_frame_chunk_length:
    case BackendReceiver::OutputStatus::invalid_frame_cobs_length:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
    case BackendReceiver::OutputStatus::invalid_datagram_parse:
    case BackendReceiver::OutputStatus::invalid_datagram_crc:
    case BackendReceiver::OutputStatus::invalid_datagram_length:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
    case BackendReceiver::OutputStatus::invalid_datagram_sequence:
      // TODO(lietk12): handle warning case first
      break;
    case BackendReceiver::OutputStatus::invalid_message_length:
    case BackendReceiver::OutputStatus::invalid_message_type:
    case BackendReceiver::OutputStatus::invalid_message_encoding:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
    case BackendReceiver::OutputStatus::available:
      break;
  }
  return Status::available;
}

template <typename BufferedUART>
void UARTBackendReceiver<BufferedUART>::input() {
  while (true) {  // repeat until UART read buffer is empty or output is available
    uint8_t receive = 0;

    // UART
    switch (uart_.read(receive)) {
      case BufferStatus::empty:
        return;
      case BufferStatus::ok:
        break;
      default:
        break;
    }

    // BackendReceiver
    switch (serial_.input(receive)) {
      case BackendReceiver::InputStatus::output_ready:
        return;
      case BackendReceiver::InputStatus::invalid_frame_chunk_length:
        // TODO(lietk12): handle error case first
        return;
      case BackendReceiver::InputStatus::input_ready:
        break;
    }
  }
}

// UARTBackendSender

template <typename BufferedUART>
typename UARTBackendSender<BufferedUART>::Status UARTBackendSender<BufferedUART>::input(
    const Application::Message &inputMessage) {
  ChunkBuffer send_output;
  switch (serial_.transform(inputMessage, send_output)) {
    case BackendSender::Status::invalid_message_length:
    case BackendSender::Status::invalid_message_type:
    case BackendSender::Status::invalid_message_encoding:
    case BackendSender::Status::invalid_datagram_length:
    case BackendSender::Status::invalid_frame_cobs_length:
    case BackendSender::Status::invalid_frame_chunk_length:
      // TODO(lietk12): handle error cases first
      return Status::invalid;
    case BackendSender::Status::ok:  // ready to write to UART
      break;
  }
  // blocks until everything is written to TX buffer
  for (size_t i = 0; i < send_output.size(); ++i) {
    uart_.write_block(send_output.buffer[i], 100000);
  }
  return Status::ok;
}

// UARTBackendDriver

template <typename BufferedUART>
void UARTBackendDriver<BufferedUART>::setup_irq() {
  uart.setup_irq();
}

template <typename BufferedUART>
typename UARTBackendDriver<BufferedUART>::Receiver::Status UARTBackendDriver<BufferedUART>::receive(
    Application::Message &receiveMessage) {
  return receiver_.output(receiveMessage);
}

template <typename BufferedUART>
typename UARTBackendDriver<BufferedUART>::Sender::Status UARTBackendDriver<BufferedUART>::send(
    const Application::Message &sendMessage) {
  return sender_.input(sendMessage);
}

// UARTBackend

void UARTBackend::setup_irq() {
  driver_.setup_irq();
}

void UARTBackend::receive() {
  Application::Message message;
  switch (driver_.receive(message)) {
    case Driver::Receiver::Status::waiting:  // nothing to do
    case Driver::Receiver::Status::invalid:  // errors handled by driver
      break;
    case Driver::Receiver::Status::available:
      switch (synchronizer_.input(message)) {
        case StateSynchronizer::InputStatus::invalid_type:
          // TODO(lietk12): handle error case
          break;
        case StateSynchronizer::InputStatus::ok:
          break;
      }
      break;
    default:
      break;
  }
}

void UARTBackend::update_clock(uint32_t current_time) {
  synchronizer_.input(current_time);
}

void UARTBackend::send() {
  Application::Message message;
  switch (synchronizer_.output(message)) {
    case StateSynchronizer::OutputStatus::waiting:
      break;
    case StateSynchronizer::OutputStatus::available:
      switch (driver_.send(message)) {
        case Driver::Sender::Status::invalid:  // errors handled by driver
          break;
        case Driver::Sender::Status::ok:
          break;
      }
      break;
  }
}

}  // namespace Pufferfish::Driver::Serial::Backend
