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
 * An interface for a hardware that can be tested and reset
 */
class Testable {
 public:
  /**
   * Run test cases and return the test result
   * @return ok on success, error code otherwise
   */
  virtual I2CDeviceStatus test() = 0;

  /**
   * Reset the device
   * @return ok on success, error code otherwise
   */
  virtual I2CDeviceStatus reset() = 0;
};

} /* namespace Driver */
} /* namespace Pufferfish */
