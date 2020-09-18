/*
 * Copyright 2020, the Pez Globo team and the Pufferfish project contributors
 *
 *      Author: March Boonyapaluk
 *
 * A common hardware-abstraction-layer header file
 */

#pragma once

#include "Types.h"
#include "CRC.h"

// Interfaces

#include "Interfaces/DigitalOutput.h"
#include "Interfaces/I2CDevice.h"
#include "Interfaces/SPIDevice.h"

// STM32 HAL

#include "STM32/AnalogInput.h"
#include "STM32/DigitalInput.h"
#include "STM32/Endian.h"
#include "STM32/HALDigitalOutput.h"
#include "STM32/HALI2CDevice.h"
#include "STM32/HALSPIDevice.h"
#include "STM32/PWM.h"
#include "STM32/Time.h"
