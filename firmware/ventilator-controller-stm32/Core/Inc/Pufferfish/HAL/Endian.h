/**
 * Endian.h
 *
 *  Created on: May 24, 2020
 *      Author: Raavi Lagoo & Renji Panicker
 *
 *  Endianness conversions between host and network order.
 *  STM32 is little-endian, while network order is big-endian, so
 *  the conversion functions reverse the order of bytes..
 */

#pragma once

#include <cstdint>
#include "Pufferfish/HAL/STM32/Endian.h"



namespace Pufferfish {
namespace HAL{



template<typename T>
constexpr T hton(const T& i) {
            return Pufferfish::HAL::STM32::hton(i);
        }
template<typename T>
constexpr T ntoh(const T& i) {
            return Pufferfish::HAL::STM32::ntoh(i);
        }


}// namespace HAL
}// namespace Pufferfish

