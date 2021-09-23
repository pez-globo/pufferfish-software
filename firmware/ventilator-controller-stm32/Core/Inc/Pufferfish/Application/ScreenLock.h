/*
 * ScreenLock.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#pragma once

#include "Pufferfish/Application/States.h"

namespace Pufferfish::Application {

using Pufferfish::Application::ScreenStatus;
using Pufferfish::Application::ScreenStatusRequest;

class ScreenLock {
 public:
  static void transform(const ScreenStatusRequest &request, ScreenStatus &response);
};

}  // namespace Pufferfish::Application