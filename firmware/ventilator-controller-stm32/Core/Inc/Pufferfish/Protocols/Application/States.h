/*
 * Application.h
 *
 * Senders for state synchronization.
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

#include "Pufferfish/Util/Containers/EnumMap.h"

namespace Pufferfish::Protocols::Application {

enum class StateOutputStatus { ok = 0, none, invalid_type };

// State Sending

template <typename StateSegment>
class StateSender {
 public:
  virtual StateOutputStatus output(StateSegment &output) = 0;
};

// Indexed State Sending

template <typename Index, typename StateSegment>
class IndexedStateSender {
 public:
  virtual StateOutputStatus output(Index index, StateSegment &output) const = 0;
};

template <typename Index, typename StateSegment, size_t senders_capacity>
class MappedStateSenders : public IndexedStateSender<Index, StateSegment> {
 public:
  // We use raw pointers instead of std::reference_wrapper because std::reference_wrapper
  // lacks a default constructor or default initialization, which we need in order to
  // construct the EnumMap.
  using SendersMap =
      Util::Containers::EnumMap<Index, StateSender<StateSegment> *, senders_capacity>;

  // Note: the lifetimes of the senders pointed to from the initializer list must be
  // externally managed!
  explicit MappedStateSenders(typename SendersMap::InitializerList init) : senders_(init) {}

  StateOutputStatus output(Index index, StateSegment &output) const override;

 private:
  SendersMap senders_;
};

// Scheduled State Sending

// Generates outputs from an IndexedStateSender by rotating through a schedule
// consisting of a fixed sequence of indices.
template <typename Index, typename StateSegment, size_t sched_size>
class SequentialStateSender : public StateSender<StateSegment> {
 public:
  using IndexSequence = std::array<Index, sched_size>;
  using IndexedSender = IndexedStateSender<Index, StateSegment>;

  SequentialStateSender(
      const IndexSequence &index_sequence,
      IndexedSender &indexed_sender,
      bool skip_unavailable = false)
      : skip_unavailable_(skip_unavailable),
        index_sequence_(index_sequence),
        indexed_sender_(indexed_sender) {}

  StateOutputStatus output(StateSegment &output) override;
  Index last_index() const;

 private:
  const bool skip_unavailable_;
  const IndexSequence &index_sequence_;
  IndexedSender &indexed_sender_;
  size_t sequence_cursor_ = 0;
  Index last_index_;

  StateOutputStatus get_next_output(StateSegment &output);
};

}  // namespace Pufferfish::Protocols::Application

#include "States.tpp"
