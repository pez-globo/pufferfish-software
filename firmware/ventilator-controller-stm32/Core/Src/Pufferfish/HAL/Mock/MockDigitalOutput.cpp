/// MockDigitalOutput.cpp
/// This file has methods for mock abstract interfaces for testing Digital
/// Output.

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

#include "Pufferfish/HAL/Mock/MockDigitalOutput.h"

namespace Pufferfish::HAL {

void MockDigitalOutput::write(bool output) {
  /* Update the lastOutput with parameter output value */
  last_output_ = output;
}

bool MockDigitalOutput::get_write() const {
  /* return the write value, written during the write method */
  return last_output_;
}
}  // namespace Pufferfish::HAL
