/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#pragma once

#include <utility>

#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/Containers/Array.h"

namespace Pufferfish::Driver {

/**
 * An interface for a hardware which needs to be initialized during setup
 */
class Initializable {
 public:
  /**
   * Run a step of setup and return the result
   * @return ok when setup has completed, setup when it is in progress, failed when an error
   * occurred
   */
  virtual InitializableState setup() = 0;
};

using InitializableRef = std::reference_wrapper<Initializable>;

template <size_t size>
class Initializables {
 public:
  explicit Initializables(const std::array<InitializableRef, size> &initializables)
      : initializables_(initializables) {}

  void setup();
  [[nodiscard]] bool setup_failed() const;
  [[nodiscard]] bool setup_in_progress() const;

 private:
  std::array<InitializableRef, size> initializables_;
  std::array<InitializableState, size> initialization_states_;
};

template <typename... Arg>
constexpr auto make_initializables(Arg &&... arg) noexcept {
  return Initializables<sizeof...(Arg)>(Util::Containers::make_array<InitializableRef>(arg...));
}

}  // namespace Pufferfish::Driver

#include "Initializable.tpp"
