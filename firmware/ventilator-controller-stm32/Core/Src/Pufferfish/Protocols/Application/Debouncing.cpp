/// Button.cpp
/// Methods for membaren buttons debounce calculation
/// The low-level driver for the membrane buttons needs a way to
/// detect when each button has been pressed
/// (consisting of a push down + a release).

// Copyright (c) 2020 Pez-Globo and the Pufferfish project contributors
// SPDX-License-Identifier: Apache-2.0
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
// either express or implied.
//
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Pufferfish/Protocols/Application/Debouncing.h"

namespace Pufferfish::Protocols::Application {

// Debouncer

Debouncer &Debouncer::operator=(const Debouncer &other) {
  max_integrator_samples_ = other.max_integrator_samples_;
  integrator_ = other.integrator_;
  output_ = other.output_;
  sampling_timer_ = other.sampling_timer_;
  bouncing_timer_ = other.bouncing_timer_;
  return *this;
}

Debouncer::Status Debouncer::transform(bool input, uint32_t current_time, bool &output) {
  if (sampling_timer_.within_timeout(current_time)) {
    output = output_;
    return Status::waiting;
  }

  sampling_timer_.reset(current_time);

  // Update the integrator based on the input signal
  if (!input && integrator_ > 0) {
    --integrator_;
  } else if (input && integrator_ < max_integrator_samples_) {
    ++integrator_;
  }
  // Update the output based on the integrator
  if (integrator_ == 0) {
    output_ = false;
    bouncing_timer_.reset(current_time);
  } else if (integrator_ >= max_integrator_samples_) {
    output_ = true;
    bouncing_timer_.reset(current_time);
    integrator_ = max_integrator_samples_;  // defensive code if integrator got corrupted
  }
  output = output_;
  // Report fault if the input has been bouncing for too long
  if (!bouncing_timer_.within_timeout(current_time)) {
    return Status::unstable;
  }

  return Status::ok;
}

void Debouncer::transform() {
  integrator_ = 0;
  output_ = false;
  sampling_timer_.reset(0);
  bouncing_timer_.reset(0);
}

// EdgeDetector

void EdgeDetector::transform(bool input, State &output) {
  if (input == last_state_) {
    output = State::no_edge;
    return;
  }

  last_state_ = input;
  if (input) {
    output = State::rising_edge;
  } else {
    output = State::falling_edge;
  }
}

}  // namespace Pufferfish::Protocols::Application
