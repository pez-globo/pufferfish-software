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

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Protocols::Application {

// Debouncer

Debouncer::Status Debouncer::transform(bool input, uint32_t current_time, bool &output) {
  if (Util::within_timeout(prev_sample_time_, sampling_interval, current_time)) {
    output = output_;
    return Status::waiting;
  }

  prev_sample_time_ = current_time;

  // Update the integrator based on the input signal
  if (!input && integrator_ > 0) {
    --integrator_;
  } else if (input && integrator_ < max_integrator_samples) {
    ++integrator_;
  }
  // Update the output based on the integrator
  if (integrator_ == 0) {
    output_ = false;
    prev_stable_time_ = current_time;
  } else if (integrator_ >= max_integrator_samples) {
    output_ = true;
    prev_stable_time_ = current_time;
    integrator_ = max_integrator_samples;  // defensive code if integrator got corrupted
  }
  output = output_;
  // Report fault if the input has been bouncing for too long
  if (!Util::within_timeout(prev_stable_time_, allowed_bounce_duration, current_time)) {
    return Status::unstable;
  }

  return Status::ok;
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
