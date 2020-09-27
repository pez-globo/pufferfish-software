/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Application {

// States

bool States::set_state(const Message &input_message) {
  switch (input_message.type) {
    case static_cast<uint8_t>(MessageTypes::alarms):
      alarms = input_message.payload.alarms;
      return true;
    case static_cast<uint8_t>(MessageTypes::sensor_measurements):
      sensor_measurements = input_message.payload.sensor_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::cycle_measurements):
      cycle_measurements = input_message.payload.cycle_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters):
      parameters = input_message.payload.parameters;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters_request):
      parameters_request = input_message.payload.parameters_request;
      return true;
    case static_cast<uint8_t>(MessageTypes::ping):
      ping = input_message.payload.ping;
      return true;
    case static_cast<uint8_t>(MessageTypes::announcement):
      announcement = input_message.payload.announcement;
      return true;
    default:
      return false;
  }
}

void States::set_message(MessageTypes type, Message &output_message) const {
  output_message.type = static_cast<uint8_t>(type);
  switch (type) {
    case MessageTypes::alarms:
      output_message.payload.alarms = alarms;
      return;
    case MessageTypes::sensor_measurements:
      output_message.payload.sensor_measurements = sensor_measurements;
      return;
    case MessageTypes::cycle_measurements:
      output_message.payload.cycle_measurements = cycle_measurements;
      return;
    case MessageTypes::parameters:
      output_message.payload.parameters = parameters;
      return;
    case MessageTypes::parameters_request:
      output_message.payload.parameters_request = parameters_request;
      return;
    case MessageTypes::ping:
      output_message.payload.ping = ping;
      return;
    case MessageTypes::announcement:
      output_message.payload.announcement = announcement;
      return;
  }
}

}  // namespace Pufferfish::Application
