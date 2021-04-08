/*
 * Lists.tpp
 *
 *  Created on: May 16, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Lists.h"

namespace Pufferfish::Protocols {

// ListSender

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
ListInputStatus ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::input(
    uint32_t next_expected) {
  next_expected_ = next_expected;
  if (next_expected_ ==
      0) {  // 0 is an init case to mean "receiver has no info, send whatever you have"
    return ListInputStatus::ok;
  }

  ListElement oldest_element;
  if (elements.peek(oldest_element) != BufferStatus::ok || next_expected_ >= oldest_element.id) {
    return ListInputStatus::ok;
  }

  return ListInputStatus::stale_next_expected;
}

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
ListInputStatus ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::input(
    const ListElement &new_element, ListElement &overwritten_element) {
  ++total_elements_;
  if (elements.push(new_element) == BufferStatus::ok) {
    return ListInputStatus::ok;
  }

  elements.pop(overwritten_element);  // after this, elements is guaranteed to have capacity
  elements.push(new_element);
  return ListInputStatus::oldest_overwritten;
}

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
void ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::output(
    ListSegment &segment) {
  ListElement next_element;
  // Pop all elements consumed by the receiver
  while (elements.peek(next_element) == BufferStatus::ok && next_element.id < next_expected_) {
    elements.pop(next_element);
  }
  // Copy remaining elements into output
  for (segment.elements_count = 0; segment.elements_count < max_segment_len;
       ++segment.elements_count) {
    ListElement next_element;
    if (elements.peek(next_element, segment.elements_count) == BufferStatus::empty) {
      break;
    }

    segment.elements[segment.elements_count] = next_element;
  }
  segment.next_expected = next_expected_;
  segment.total = total_elements_;
  segment.remaining = elements.size();
}

}  // namespace Pufferfish::Protocols
