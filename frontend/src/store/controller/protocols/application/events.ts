import _ from 'lodash';
import { Sender, TaggedStateSegment, IndexedSender, sequentialSender } from './states';

// Use makeFilteredSender to make an IndexedSender with an overlay to allow suppressing
// indices by excluding them from allowedIndices.
export const makeFilteredSender = <Index, StateSegment>(
  indexedSender: IndexedSender<Index, StateSegment>,
  allowedIndices: Set<Index>,
) => (index: Index): StateSegment | null =>
  allowedIndices.has(index) ? indexedSender(index) : null;

// A notification sender uses a sequence of Indices to yield outputs from an IndexedSender
// based on indices provided as input to the next() method.
// After a yield, the next call to the next() method should contain a (possibly empty) array
// of indices to mark as sendable.
export function* notificationSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  indexedSender: IndexedSender<Index, TaggedStateSegment<Index, StateSegment>>,
  outputIdle = false,
): Sender<TaggedStateSegment<Index, StateSegment>> {
  const sendableIndices = new Set<Index>();
  const sendableSender = sequentialSender(
    indexSequence,
    makeFilteredSender(indexedSender, sendableIndices),
    true,
  );
  const idleSender = sequentialSender(indexSequence, indexedSender, true);

  while (true) {
    let state = null;
    if (sendableIndices.size === 0 && outputIdle) {
      // console.log(`Event: outputting idle!`);
      state = idleSender.next().value;
    } else {
      // console.log(`Event: outputting sendable...`);
      state = sendableSender.next().value;
      if (state === null && outputIdle) {
        // console.log(`Event: actually outputting idle!`);
        state = idleSender.next().value;
      }
    }
    if (state !== null) {
      sendableIndices.delete(state.type);
      // console.log(`Event: sending ${state.type}`);
    }
    const newlySendableIndices = (yield state) as Set<Index>;
    newlySendableIndices.forEach(sendableIndices.add, sendableIndices);
  }
}

export interface ConnectionStatus {
  lastConnectionTime: Date | null;
}

// A changed state sender uses a sequence of Indices to yield outputs from an IndexedSender
// based on changes to states. The next() method takes an optional boolean specifying
// whether to reset all states as if they've changed and thus should be sent as notifications.
export function* changedStateSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  states: IndexedSender<Index, TaggedStateSegment<Index, StateSegment>>,
  connectionStatus: ConnectionStatus,
  outputIdle = false,
): Sender<TaggedStateSegment<Index, StateSegment>> {
  const sender = notificationSender(indexSequence, states, outputIdle);
  const trackableStates = Array.from(new Set(indexSequence));
  const prevStates = new Map<Index, TaggedStateSegment<Index, StateSegment>>();
  const prevConnectionStatus: ConnectionStatus = { lastConnectionTime: null };

  const nextSenderInput = new Set<Index>();
  while (true) {
    const resetAll =
      connectionStatus.lastConnectionTime !== prevConnectionStatus.lastConnectionTime;
    if (resetAll) {
      prevConnectionStatus.lastConnectionTime = connectionStatus.lastConnectionTime;
      // console.log('Event: handling new connection!');
    }
    nextSenderInput.clear();
    trackableStates.forEach((index) => {
      const newState = states(index);
      if (newState !== null && (resetAll || !_.isEqual(newState, prevStates.get(index)))) {
        // console.log(`Event: State ${index} changed!`)
        nextSenderInput.add(index);
        prevStates.set(index, newState);
      }
    });
    yield sender.next(nextSenderInput).value;
  }
}
