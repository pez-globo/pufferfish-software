/*
 * BreathingCircuit.tpp
 *
 *  Created on: June 6, 2020
 *      Author: Ethan Li
 */

#include "Pufferfish/Protocols/Application/SignalSmoothing.h"

#include <cmath>

namespace Pufferfish::Protocols::Application {

// EWMA

void EWMA::transform(float raw, float &filtered) {
  if (std::isnan(average_)) {
    average_ = raw;
  }
  average_ = responsiveness * raw + (1 - responsiveness) * average_;
  filtered = average_;
}

// ConvergenceSmoother

void ConvergenceSmoother::transform(uint32_t current_time, float raw, float &filtered) {
  if (std::isnan(prev_raw_)) {  // Smoother was reset
    started_changing_ = false;
    started_converging_ = false;
    converged_ = float_nan;
    filtered_ = float_nan;
    prev_time_ = current_time;
  }
  const float change_magnitude = std::abs(raw - prev_raw_);
  const bool possibly_converging =
      std::isnan(converged_) && !std::isnan(prev_raw_) && change_magnitude < change_min_magnitude;
  const float residual = std::abs(raw - converged_);
  const bool possibly_converged = !std::isnan(converged_) && residual < change_min_magnitude;
  if (possibly_converging || possibly_converged) {
    // Value may be converging or is already converged
    started_changing_ = false;  // stop the "changing" timer
    if (!started_converging_) {
      // Need to initialize the "converging" timer
      convergence_timer_.reset(current_time);
      started_converging_ = true;
    }
    const bool converged = !convergence_timer_.within_timeout(current_time);
    const bool prev_converged = !convergence_timer_.within_timeout(prev_time_);
    if (converged && !prev_converged) {
      // Value has just converged
      converged_ = raw;
      filtered_ = converged_;
    }
  } else {
    // Value may be starting a transition to a different level
    started_converging_ = false;  // stop the "converging" timer
    if (!started_changing_) {
      // Need to initialize the "changing" timer
      change_timer_.reset(current_time);
      started_changing_ = true;
    }
    if (!change_timer_.within_timeout(current_time)) {
      // Value is transitioning to a different level
      filtered_ = raw;
      converged_ = float_nan;
    }
  }
  filtered = filtered_;
  prev_raw_ = raw;
  prev_time_ = current_time;
}

// DisplaySmoother

DisplaySmoother::Status DisplaySmoother::transform(
    uint32_t current_time, float raw, float &filtered) {
  if (sampling_timer_.within_timeout(current_time)) {
    return Status::waiting;
  }

  sampling_timer_.reset(current_time);
  float ewma_result = float_nan;
  ewma_.transform(raw, ewma_result);
  convergence_.transform(current_time, ewma_result, filtered);
  return Status::ok;
}

}  // namespace Pufferfish::Protocols::Application
