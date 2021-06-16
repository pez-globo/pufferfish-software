/**
 * Platform.h
 *
 *  Created on: May 24, 2020
 *      Author: Raavi Lagoo & Renji Panicker
 *
 *  Selects the platform for the HAL.
 *  TODO: use a build-time environment variable to select the platform.
 */

#pragma once

#include "Pufferfish/HAL/STM32/Platform.h"

namespace Pufferfish::HAL {

namespace Platform = STM32;

}  // namespace Pufferfish::HAL
