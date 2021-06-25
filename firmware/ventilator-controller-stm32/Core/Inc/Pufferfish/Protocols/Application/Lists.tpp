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

namespace Pufferfish::Protocols::Application {

// ListSender

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
void ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::setup(
    uint32_t session_id) {
  session_id_ = session_id;
  if (session_id_ == 0) {
    // 0 is reserved as a sentinel value for an uninitialized receiver
    ++session_id_;
  }
}

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
ListInputStatus ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::input(
    uint32_t next_expected, uint32_t expected_session_id) {
  if (expected_session_id != session_id_) {
    if (next_expected == 0) {
      // The receiver is uninitialized (i.e. it doesn't know the session yet)
      return ListInputStatus::ok;
    }

    return ListInputStatus::stale_session;
  }

  // We only update next_expected_ if the expected session id matches our session id;
  // otherwise, the expected element ID is for a stale session
  next_expected_ = next_expected;
  if (next_expected_ ==
      0) {  // 0 is an init case to mean "receiver has no info, send whatever you have"
    return ListInputStatus::ok;
  }

  ListElement oldest_element{};
  if (elements_.peek(oldest_element) != BufferStatus::ok || next_expected_ >= oldest_element.id) {
    return ListInputStatus::ok;
  }

  return ListInputStatus::stale_next_expected;
}

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
ListInputStatus ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::input(
    const ListElement &new_element, ListElement &overwritten_element) {
  ++total_elements_;
  if (elements_.push(new_element) == BufferStatus::ok) {
    return ListInputStatus::ok;
  }

  if (new_element.id == 0) {
    // Invalidate the previous session ID
    // Note: There is a potential for buggy behavior here because the element ID could roll over
    // (and thus force reset of session ID) in the middle of a ListSegment. Then the elements
    // in the ListSegment before session ID replacement will be orphaned and may screw up
    // ExpectedLogElement.id on the list receiver's end.
    // However, because element ID is uint32_t, rollover will never occur in practice.
    ++session_id_;
    if (session_id_ == 0) {
      // 0 is reserved as a sentinel value for an uninitialized receiver
      ++session_id_;
    }
  }

  elements_.pop(overwritten_element);  // after this, elements is guaranteed to have capacity
  elements_.push(new_element);
  return ListInputStatus::oldest_overwritten;
}

template <typename ListSegment, typename ListElement, size_t max_buffer_len, size_t max_segment_len>
void ListSender<ListSegment, ListElement, max_buffer_len, max_segment_len>::output(
    ListSegment &segment) {
  ListElement next_element{};
  // Pop all elements consumed by the receiver
  while (elements_.peek(next_element) == BufferStatus::ok && next_element.id < next_expected_) {
    elements_.pop(next_element);
  }
  // Copy remaining elements into output
  for (segment.elements_count = 0; segment.elements_count < max_segment_len;
       ++segment.elements_count) {
    if (elements_.peek(next_element, segment.elements_count) == BufferStatus::empty) {
      break;
    }

    segment.elements[segment.elements_count] = next_element;
  }
  segment.next_expected = next_expected_;
  segment.total = total_elements_;
  segment.remaining = elements_.size();
  segment.session_id = session_id_;
}

}  // namespace Pufferfish::Protocols::Application
