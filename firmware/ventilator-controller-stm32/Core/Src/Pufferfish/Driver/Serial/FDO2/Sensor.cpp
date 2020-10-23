/*
 * StateMachine.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/Serial/FDO2/Sensor.h"

#include "Pufferfish/Util/Timeouts.h"

// This macro is used to add a checker for the value of a specified request type with an associated
// union field and enum value. We use a macro because it makes the code more maintainable here,
// while allowing us to ensure union tagging.
// clang-format off
// NOLINTNEXTLINE(cppcoreguidelines-macro-usage)
#define RESPONSE_TAGGED_COMPARISON(type, field) \
  inline bool operator==(const Response &input_response, const type &expected_value) {\
    if (input_response.tag != CommandTypes::field) {\
      return false;\
    }\
    return input_response.value.field == expected_value; } // NOLINT(cppcoreguidelines-pro-type-union-access)
// clang-format on

namespace Pufferfish::Driver::Serial::FDO2 {

RESPONSE_TAGGED_COMPARISON(Responses::Vers, vers)
RESPONSE_TAGGED_COMPARISON(Responses::Bcst, bcst)

// Sensor

InitializableState Sensor::setup() {
  if (setup_completed_) {
    return InitializableState::ok;
  }

  if (setup_start_time_ == 0) {
    setup_start_time_ = time_.millis();
  }
  if (!Util::within_timeout(setup_start_time_, setup_timeout, time_.millis())) {
    return InitializableState::failed;
  }

  if (!check_version()) {
    return InitializableState::setup;
  }

  if (!start_broadcast()) {
    return InitializableState::setup;
  }

  setup_completed_ = true;
  return InitializableState::ok;
}

InitializableState Sensor::output(uint32_t &po2) {
  Response response;
  device_.receive(response);
  // This is a tagged union access
  if (response.tag == CommandTypes::mraw) {
    po2 = response.value.mraw.po2;  // NOLINT(cppcoreguidelines-pro-type-union-access)
  }
  return InitializableState::ok;
}

bool Sensor::check_version() {
  device_.request_version();
  request_time_ = time_.millis();
  while (Util::within_timeout(request_time_, setup_request_timeout, time_.millis())) {
    Response response;
    if (device_.receive(response) == Device::Status::ok && response.tag == CommandTypes::vers) {
      return response == Device::expected_vers;
    }
  }
  return false;
}

bool Sensor::start_broadcast() {
  device_.start_broadcast();
  request_time_ = time_.millis();
  while (Util::within_timeout(request_time_, setup_request_timeout, time_.millis())) {
    Response response;
    if (device_.receive(response) == Device::Status::ok && response.tag == CommandTypes::bcst) {
      return response == Device::expected_bcst;
    }
  }
  return false;
}

}  // namespace Pufferfish::Driver::Serial::FDO2
