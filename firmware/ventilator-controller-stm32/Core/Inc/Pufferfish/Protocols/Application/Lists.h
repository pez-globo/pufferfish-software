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

namespace Pufferfish::Protocols::Application {

enum class ListInputStatus { ok = 0, stale_next_expected, oldest_overwritten, stale_session };

// Synchronize an ephemeral list with a receiver by sending its elements in chunks.
// The receiver indicates the next element it expects through that element's index;
// if it doesn't increment the index, that's backpressure on the list synchronization.
template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
class ListSender {
 public:
  using Segment = ListSegment;
  using Element = ListElement;

  static const uint32_t max_id = std::numeric_limits<uint32_t>::max();

  void setup(uint32_t session_id);

  ListInputStatus input(uint32_t next_expected, uint32_t expected_session_id);
  ListInputStatus input(const ListElement &new_element, ListElement &overwritten_element);
  void output(ListSegment &segment);

 private:
  Util::RingBuffer<max_buffer_len, ListElement> elements_;
  uint32_t next_expected_ = 0;
  uint32_t total_elements_ = 0;
  uint32_t session_id_ = 0;
};

}  // namespace Pufferfish::Protocols::Application

#include "Lists.tpp"
