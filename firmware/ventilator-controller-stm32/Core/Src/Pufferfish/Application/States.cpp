/*
 * Application.tpp
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Application {

// States

bool States::setState(const Message &inputMessage) {
  switch (inputMessage.type) {
  case static_cast<uint8_t>(MessageTypes::alarms):
    alarms = inputMessage.payload.alarms;
    return true;
  case static_cast<uint8_t>(MessageTypes::sensor_measurements):
    sensor_measurements = inputMessage.payload.sensor_measurements;
    return true;
  case static_cast<uint8_t>(MessageTypes::cycle_measurements):
    cycle_measurements = inputMessage.payload.cycle_measurements;
    return true;
  case static_cast<uint8_t>(MessageTypes::parameters):
    parameters = inputMessage.payload.parameters;
    return true;
  case static_cast<uint8_t>(MessageTypes::parameters_request):
    parameters_request = inputMessage.payload.parameters_request;
    return true;
  case static_cast<uint8_t>(MessageTypes::ping):
    ping = inputMessage.payload.ping;
    return true;
  case static_cast<uint8_t>(MessageTypes::announcement):
    announcement = inputMessage.payload.announcement;
    return true;
  default:
    return false;
  }
}

void States::setMessage(
    MessageTypes type, Message &outputMessage) const {
  outputMessage.type = static_cast<uint8_t>(type);
  switch (type) {
  case MessageTypes::alarms:
    outputMessage.payload.alarms = alarms;
    return;
  case MessageTypes::sensor_measurements:
    outputMessage.payload.sensor_measurements = sensor_measurements;
    return;
  case MessageTypes::cycle_measurements:
    outputMessage.payload.cycle_measurements = cycle_measurements;
    return;
  case MessageTypes::parameters:
    outputMessage.payload.parameters = parameters;
    return;
  case MessageTypes::parameters_request:
    outputMessage.payload.parameters_request = parameters_request;
    return;
  case MessageTypes::ping:
    outputMessage.payload.ping = ping;
    return;
  case MessageTypes::announcement:
    outputMessage.payload.announcement = announcement;
    return;
  }
}

}
