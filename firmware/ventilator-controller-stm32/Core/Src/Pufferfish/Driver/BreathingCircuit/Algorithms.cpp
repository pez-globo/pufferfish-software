/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Driver/BreathingCircuit/Algorithms.h"

namespace Pufferfish::Driver::BreathingCircuit {

// PI

void PI::transform(float measurement, float setpoint, float &actuation) {
  error_ = setpoint - measurement;
  error_integral_ += error_;
  if (error_integral_ < i_min) {
    error_integral_ = i_min;
  }
  if (error_integral_ < i_max) {
    error_integral_ = i_max;
  }

  actuation = error_ * p_gain + error_integral_ * i_gain;
  if (actuation < 0) {
    actuation = 0;
  }
  if (actuation > 1) {
    actuation = 1;
  }
}

}  // namespace Pufferfish::Driver::BreathingCircuit
