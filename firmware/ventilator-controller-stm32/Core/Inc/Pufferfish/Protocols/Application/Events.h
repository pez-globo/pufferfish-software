/*
 * Application.h
 *
 * Senders for state synchronization.
 *
 *  Created on: May 26, 2020
 *      Author: Ethan Li
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "Pufferfish/Util/Containers/EnumSet.h"
#include "Pufferfish/Util/Containers/Vector.h"
#include "States.h"

namespace Pufferfish::Protocols::Application {

template <typename Index, typename StateSegment>
class FilteredStateSender : public IndexedStateSender<Index, StateSegment> {
 public:
  using Sender = IndexedStateSender<Index, StateSegment>;
  using Indices = Util::Containers::Set<Index>;

  explicit FilteredStateSender(Sender &sender, const Indices &allowed_indices)
      : sender_(sender), allowed_indices_(allowed_indices) {}

  StateOutputStatus output(Index index, StateSegment &output) const override;

 private:
  Sender &sender_;
  const Indices &allowed_indices_;
};

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
class EventNotificationSender : public StateSender<StateSegment> {
 public:
  using IndexSequence = std::array<Index, sched_size>;
  using IndexedSender = IndexedStateSender<Index, StateSegment>;

  explicit EventNotificationSender(
      const IndexSequence &index_sequence, IndexedSender &indexed_sender, bool output_idle = false)
      : output_idle_(output_idle),
        sendable_indices_{},
        filtered_sender_(indexed_sender, sendable_indices_),
        sendable_sender_(index_sequence, filtered_sender_, true),
        idle_sender_(index_sequence, indexed_sender, true) {}

  void input(Index index);
  StateOutputStatus output(StateSegment &output) override;

 private:
  using AllowedIndices = Util::Containers::EnumSet<Index, allowed_indices_capacity>;
  using FilteredSender = FilteredStateSender<Index, StateSegment>;
  using SequentialSender = SequentialStateSender<Index, StateSegment, sched_size>;

  const bool output_idle_;
  AllowedIndices sendable_indices_;
  FilteredSender filtered_sender_;
  SequentialSender sendable_sender_;
  SequentialSender idle_sender_;

  StateOutputStatus get_next_sendable_output(StateSegment &output);
  StateOutputStatus get_next_idle_output(StateSegment &output);
};

template <typename Index, typename StateSegment, size_t sched_size, size_t allowed_indices_capacity>
class StateChangeEventSender : public StateSender<StateSegment> {
 public:
  using IndexSequence = std::array<Index, sched_size>;
  using IndexedSender = IndexedStateSender<Index, StateSegment>;

  StateChangeEventSender(
      const IndexSequence &index_sequence, IndexedSender &all_states, bool output_idle = false);

  // Calling input will reset the sender to send all states as if they had all changed. This is
  // useful if the sender needs to send all states when a new connection is established, to
  // (re)initialize the other peer.
  void input();
  StateOutputStatus output(StateSegment &output);

 private:
  using NotificationSender =
      EventNotificationSender<Index, StateSegment, sched_size, allowed_indices_capacity>;
  using Store = Util::Containers::EnumMap<Index, StateSegment, allowed_indices_capacity>;
  // trackable_states_ is a subset of index_sequence, so it only needs sched_size capacity
  using TrackableStates = Util::Containers::Vector<Index, sched_size>;

  IndexedSender &all_states_;
  NotificationSender notification_sender_;
  TrackableStates trackable_states_;
  Store prev_states_;
};

}  // namespace Pufferfish::Protocols::Application

#include "Events.tpp"
