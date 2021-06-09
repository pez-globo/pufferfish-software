/*
 * SignalSmoothing.h
 *
 *  Signal processing and denoising
 */

#pragma once

#include <cstdint>
#include <limits>

namespace Pufferfish::Protocols::Application {

static constexpr float float_nan = std::numeric_limits<float>::quiet_NaN();

// Basic implementation of an exponentially-weighted moving average
// Note: this does not account for the time interval of sampling, so the responsiveness
// parameter will need to be re-adjusted upon any changes to the time interval of calls to
// the EWMA filter!
class EWMA {
 public:
  explicit EWMA(float responsiveness) : responsiveness(responsiveness) {}

  void transform(float raw, float &filtered);

 private:
  const float responsiveness;
  float average_ = float_nan;
};

// A custom signal filter which tracks when the input has converged on a value
// (even with small noise fluctuations) and locks the output at a constant value.
// The converged value is estimated as the value of the input at the moment that its
// variations have been within change_min_magnitude over the entire convergence_min_duration;
// any further changes within change_min_magnitude around that convergence estimate will be
// filtered out. Thus, the filtered value is "locked" onto a value after the input converges.
// The convergence estimate is maintained as the output until the input has been outside the
// change_min_magnitude window around the convergence estimate over the entire
// change_min_duration, after which the input is considered to be transitioning to a new value.
// While the input is transitioning, it's passed through raw (i.e. without any processing)
// as the output, until it has converged to a new value.
// If the input has converged but is actually slowly drifting in a single direction,
// then the output will change occasionally, whenever the input drifts far enough away from
// the last convergence estimate. Then its new value will become the new convergence estimate.
class ConvergenceSmoother {
 public:
  ConvergenceSmoother(
      // change_min_magnitude specifies the convergence interval - if it's bigger, the smoother
      // will tolerate larger changes of the input as still being within the convergence condition.
      // If convergence_min_duration is higher, the convergence condition will be more conservative
      // - the input value will need to have stopped changing for a longer continuous duration
      // before the smoother decides that the value has converged.
      // If change_min_duration is higher, the convergence estimate will be "stickier" - the input
      // value will need to be far from the convergence estimate for a longer continuous duration
      // before the smoother decides that the value has left the convergence interval.
      float change_min_magnitude,
      uint32_t convergence_min_duration,
      uint32_t change_min_duration)
      : change_min_magnitude(change_min_magnitude),
        convergence_min_duration(convergence_min_duration),
        change_min_duration(change_min_duration) {}

  void transform(uint32_t current_time, float raw, float &filtered);

 private:
  const float change_min_magnitude;
  const uint32_t convergence_min_duration;  // ms
  const uint32_t change_min_duration;       // ms

  bool started_changing_ = false;
  uint32_t change_start_ = 0;  // ms
  bool started_converging_ = false;
  uint32_t convergence_start_ = 0;  // ms
  float converged_ = float_nan;
  float filtered_ = float_nan;

  float prev_raw_ = float_nan;
  uint32_t prev_time_ = 0;  // ms
};

// A class to combine EWMA with ConvergenceSmoother, to smooth & stabilize
// noisy values for user display and alarm condition detection
class DisplaySmoother {
 public:
  enum class Status { ok = 0, waiting };
  struct SmoothingParameters {
    float ewma_responsiveness;
    float change_min_magnitude;
    float convergence_min_duration;
    float change_min_duration;
  };

  DisplaySmoother(
      uint32_t sampling_interval,
      float ewma_responsiveness,
      float change_min_magnitude,
      float convergence_min_duration,
      float change_min_duration)
      : sampling_interval(sampling_interval),
        ewma_(ewma_responsiveness),
        convergence_(change_min_magnitude, convergence_min_duration, change_min_duration) {}
  DisplaySmoother(uint32_t sampling_interval, SmoothingParameters params)
      : DisplaySmoother(
            sampling_interval,
            params.ewma_responsiveness,
            params.change_min_magnitude,
            params.convergence_min_duration,
            params.change_min_duration) {}

  Status transform(uint32_t current_time, float raw, float &filtered);

 private:
  const uint32_t sampling_interval;  // ms

  EWMA ewma_;
  ConvergenceSmoother convergence_;

  uint32_t prev_sample_time_ = 0;  // ms
};

}  // namespace Pufferfish::Protocols::Application
