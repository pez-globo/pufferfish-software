/*
 * Lists.h
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <limits>

#include "Pufferfish/Statuses.h"
#include "Pufferfish/Util/RingBuffer.h"

namespace Pufferfish::Protocols {

enum class ListInputStatus { ok = 0, stale_next_expected, oldest_overwritten };

// Synchronize a list with a receiver by sending its elements in chunks.
// The receiver indicates the next element it expects through that element's index;
// if it doesn't increment the index, that's backpressure on the list synchronization.
template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
class ListSender {
 public:
  using Segment = ListSegment;
  using Element = ListElement;

  static const uint32_t max_id = std::numeric_limits<uint32_t>::max();

  // Call this until it returns available, then call output
  ListInputStatus input(uint32_t next_expected);
  ListInputStatus input(const ListElement &new_element, ListElement &overwritten_element);
  void output(ListSegment &segment);

 private:
  Util::RingBuffer<max_buffer_len, ListElement> elements;
  uint32_t next_expected_ = 0;
  uint32_t total_elements_ = 0;
};

}  // namespace Pufferfish::Protocols

#include "Lists.tpp"
