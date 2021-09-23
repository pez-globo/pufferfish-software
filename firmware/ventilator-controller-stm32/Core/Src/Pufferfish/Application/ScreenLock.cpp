/*
 * ScreenLock.cpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Hardware-backed 32-bit CRC calculation.
 */

#include "Pufferfish/Application/ScreenLock.h"

namespace Pufferfish::Application {

void ScreenLock::transform(const ScreenStatusRequest &request, ScreenStatus &response) {
  response.lock = request.lock;
}

}  // namespace Pufferfish::Application
