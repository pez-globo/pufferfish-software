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

namespace Pufferfish::Protocols::Application {

// Debouncing

class Debouncer {
 public:
  enum class Status { ok = 0, waiting, unstable };

  Debouncer() = default;
  ~Debouncer() = default;
  Debouncer(const Debouncer &other)
      : integrator_(other.integrator_),
        output_(other.output_),
        prev_sample_time_(other.prev_sample_time_),
        prev_stable_time_(other.prev_stable_time_) {}
  Debouncer(Debouncer &&other) = delete;

  Debouncer &operator=(const Debouncer &other);
  Debouncer &operator=(Debouncer &&other) = delete;

  Status transform(bool input, uint32_t current_time, bool &output);

 private:
  const uint8_t max_integrator_samples = 100;
  const uint32_t sampling_interval = 10;          // ms
  const uint32_t allowed_bounce_duration = 2000;  // ms

  uint8_t integrator_ = 0;
  bool output_ = false;
  uint32_t prev_sample_time_ = 0;  // ms
  uint32_t prev_stable_time_ = 0;  // ms
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
