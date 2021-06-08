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

#include "Pufferfish/Driver/Button/Button.h"

namespace Pufferfish::Driver::Button {

Button::Status Button::read_state(
    bool &debounced_output, Application::EdgeDetector::State &switch_state_changed) {
  bool input = button_input_.read();
  uint32_t ms_time = time_.millis();

  Button::Status status = debounce_.transform(input, ms_time, debounced_output);

  /* Debounce is not success */
  if (status != Button::Status::ok) {
    return status;
  }
  edge_detect_.transform(debounced_output, switch_state_changed);

  return status;
}

}  // namespace Pufferfish::Driver::Button
