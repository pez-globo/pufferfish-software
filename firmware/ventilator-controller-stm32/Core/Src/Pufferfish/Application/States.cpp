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
  // Refer to States.h for justification of why we are using unions this way
  switch (input_message.type) {
    case static_cast<uint8_t>(MessageTypes::alarms):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      alarms_ = input_message.payload.alarms;
      return true;
    case static_cast<uint8_t>(MessageTypes::sensor_measurements):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      sensor_measurements_ = input_message.payload.sensor_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::cycle_measurements):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      cycle_measurements_ = input_message.payload.cycle_measurements;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      parameters_ = input_message.payload.parameters;
      return true;
    case static_cast<uint8_t>(MessageTypes::parameters_request):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      parameters_request_ = input_message.payload.parameters_request;
      return true;
    case static_cast<uint8_t>(MessageTypes::ping):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      ping_ = input_message.payload.ping;
      return true;
    case static_cast<uint8_t>(MessageTypes::announcement):
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      announcement_ = input_message.payload.announcement;
      return true;
    default:
      return false;
  }
}

void States::set_message(MessageTypes type, Message &output_message) const {
  output_message.type = static_cast<uint8_t>(type);
  // Refer to States.h for justification of why we are using unions this way
  switch (type) {
    case MessageTypes::alarms:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.alarms = alarms_;
      return;
    case MessageTypes::sensor_measurements:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.sensor_measurements = sensor_measurements_;
      return;
    case MessageTypes::cycle_measurements:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.cycle_measurements = cycle_measurements_;
      return;
    case MessageTypes::parameters:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.parameters = parameters_;
      return;
    case MessageTypes::parameters_request:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.parameters_request = parameters_request_;
      return;
    case MessageTypes::ping:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.ping = ping_;
      return;
    case MessageTypes::announcement:
      // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access)
      output_message.payload.announcement = announcement_;
      return;
  }
}

}  // namespace Pufferfish::Application
