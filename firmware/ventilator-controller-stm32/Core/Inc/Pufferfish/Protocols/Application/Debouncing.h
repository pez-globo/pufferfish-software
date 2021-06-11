/// Debouncing.h
/// Classes for event debouncing and edge detection

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

#pragma once

#include <cstdint>

#include "Pufferfish/Util/Timeouts.h"

namespace Pufferfish::Protocols::Application {

// Debouncing

class Debouncer {
 public:
  enum class Status { ok = 0, waiting, unstable };

  static const uint8_t default_max_integrator_samples = 100;
  static const uint32_t default_sampling_interval = 10;   // ms
  static const uint32_t default_bouncing_timeout = 2000;  // ms

  Debouncer() = default;
  Debouncer(uint8_t max_integrator_samples, uint32_t sampling_interval)
      : max_integrator_samples_(max_integrator_samples),
        sampling_timer_{sampling_interval},
        bouncing_timer_{default_bouncing_timeout} {}
  Debouncer(
      uint8_t max_integrator_samples, uint32_t sampling_interval, uint32_t allowed_bounce_duration)
      : max_integrator_samples_(max_integrator_samples),
        sampling_timer_{sampling_interval},
        bouncing_timer_{allowed_bounce_duration} {}
  ~Debouncer() = default;
  Debouncer(const Debouncer &other) = default;
  Debouncer(Debouncer &&other) = delete;

  Debouncer &operator=(const Debouncer &other);
  Debouncer &operator=(Debouncer &&other) = delete;

  Status transform(bool input, uint32_t current_time, bool &output);
  // This resets the debouncer
  void transform();

 private:
  // Normally these would be const, but we need to change them for the assignment operator
  uint8_t max_integrator_samples_ = default_max_integrator_samples;

  uint8_t integrator_ = 0;
  bool output_ = false;
  Util::MsTimer sampling_timer_;
  Util::MsTimer bouncing_timer_;
};

// Edge detection

class EdgeDetector {
 public:
  enum class State { no_edge = 0, rising_edge, falling_edge };

  EdgeDetector() = default;

  void transform(bool input, State &output);

 private:
  bool last_state_ = false;
};

}  // namespace Pufferfish::Protocols::Application
