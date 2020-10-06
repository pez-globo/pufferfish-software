/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 */

#pragma once

#include "Pufferfish/Statuses.h"

namespace Pufferfish {
namespace Driver {

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

} /* namespace Driver */
} /* namespace Pufferfish */
