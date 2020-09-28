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
      alarms_ = input_message.payload.alarms;
      return true;
    case static_cast<uint8_t>(MessageTypes::sensor_measurements):
      sensor_measurements_ = input_message.payload.sensor_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::cycle_measurements):
      cycle_measurements_ = input_message.payload.cycle_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters):
      parameters_ = input_message.payload.parameters;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters_request):
      parameters_request_ = input_message.payload.parameters_request;
      return true;
    case static_cast<uint8_t>(MessageTypes::ping):
      ping_ = input_message.payload.ping;
      return true;
    case static_cast<uint8_t>(MessageTypes::announcement):
      announcement_ = input_message.payload.announcement;
      return true;
    default:
      return false;
  }
}

void States::set_message(MessageTypes type, Message &output_message) const {
  output_message.type = static_cast<uint8_t>(type);
  switch (type) {
    case MessageTypes::alarms:
      output_message.payload.alarms = alarms_;
      return;
    case MessageTypes::sensor_measurements:
      output_message.payload.sensor_measurements = sensor_measurements_;
      return;
    case MessageTypes::cycle_measurements:
      output_message.payload.cycle_measurements = cycle_measurements_;
      return;
    case MessageTypes::parameters:
      output_message.payload.parameters = parameters_;
      return;
    case MessageTypes::parameters_request:
      output_message.payload.parameters_request = parameters_request_;
      return;
    case MessageTypes::ping:
      output_message.payload.ping = ping_;
      return;
    case MessageTypes::announcement:
      output_message.payload.announcement = announcement_;
      return;
  }
}

}  // namespace Pufferfish::Application
