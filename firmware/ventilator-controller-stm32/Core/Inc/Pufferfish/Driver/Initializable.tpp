/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#pragma once

#include <algorithm>

#include "Initializable.h"

namespace Pufferfish::Driver {

template <size_t size>
void Initializables<size>::setup() {
  for (size_t i = 0; i < initializables_.size(); ++i) {
    initialization_states_[i] = initializables_[i].get().setup();
  }
}

template <size_t size>
bool Initializables<size>::setup_failed() const {
  return std::find(
             initialization_states_.cbegin(),
             initialization_states_.cend(),
             InitializableState::failed) != initialization_states_.cend();
}

template <size_t size>
bool Initializables<size>::setup_in_progress() const {
  return std::find(
             initialization_states_.cbegin(),
             initialization_states_.cend(),
             InitializableState::setup) != initialization_states_.cend();
}

}  // namespace Pufferfish::Driver
