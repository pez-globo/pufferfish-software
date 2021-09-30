/// \file
/// \brief Convenience functions for safely checking timeouts
///
/// Functions for uint rollover-safe checking of timeouts

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

#include "Timeouts.h"
#include "iostream"

namespace Pufferfish::Util {

template <typename T>
inline bool within_timeout(T start_time, T timeout, T test_time) {
  T duration = test_time - start_time;
  return duration < timeout;
}

// Timer

template <typename T>
Timer<T> &Timer<T>::operator=(const Timer<T> &other) {
  timeout_ = other.timeout_;
  start_time_ = other.start_time_;
  return *this;
}

template <typename T>
void Timer<T>::set_timeout(T timeout) {
  timeout_ = timeout;
}

template <typename T>
void Timer<T>::reset(T current_time) {
  start_time_ = current_time;
}

template <typename T>
bool Timer<T>::within_timeout(T current_time) const {
  return Util::within_timeout(start_time_, timeout_, current_time);
}

}  // namespace Pufferfish::Util
