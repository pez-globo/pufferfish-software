/*
 * Algorithms.h
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 *
 *  Control algorithms
 */

#pragma once

#include <cstdint>

namespace Pufferfish::Driver::BreathingCircuit {

class PI {
 public:
  void transform(float measurement, float setpoint, float &actuation);

 private:
  static constexpr float p_gain = 0.00001;
  static constexpr float i_gain = 0.0002;
  static constexpr float i_max = 200 * i_gain;
  static constexpr float i_min = 0;

  float error_ = 0;
  float error_integral_ = 0;
};

}  // namespace Pufferfish::Driver::BreathingCircuit
