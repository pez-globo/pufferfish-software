import _ from 'lodash';
import { GeneratorYieldType } from '../sagas';
import {
  SenderYield,
  Sender,
  TaggedStateSegment,
  IndexedSender,
  makeSenderYieldResult,
  sequentialSender,
} from './states';

// Use makeFilteredSender to make an IndexedSender with an overlay to allow suppressing
// indices by excluding them from allowedIndices.
export const makeFilteredSender = <Index, StateSegment>(
  indexedSender: IndexedSender<Index, StateSegment>,
  allowedIndices: Set<Index>,
) => {
  return function* (index: Index): Sender<StateSegment> {
    if (!allowedIndices.has(index)) {
      return makeSenderYieldResult(null);
    }

    let nextInput = null;
    const sender = indexedSender(index);
    while (true) {
      // Service results and effects yielded by the indexed sender
      const yieldValue: SenderYield<StateSegment> = sender.next(nextInput).value;
      switch (yieldValue.type) {
        case GeneratorYieldType.Result:
          nextInput = null;
          return yieldValue;
        case GeneratorYieldType.Effect:
          nextInput = yield yieldValue;
          break;
      }
    }
  };
};

// A notification sender uses a sequence of Indices to yield outputs from an IndexedSender,
// as well as any redux-saga effects yielded from the IndexedSender.
// After a SenderYieldResult is yielded, the next call to the next() method should
// contain a (possibly empty) array of indices to mark as sendable.
// This implementation of the notification sender does not generate any states when no
// states are sendable (i.e. it does not produce any outputs when "idle").
export function* notificationSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  indexedSender: IndexedSender<Index, TaggedStateSegment<Index, StateSegment>>,
): Sender<TaggedStateSegment<Index, StateSegment>> {
  const sendableIndices = new Set<Index>();
  const sendableSender = sequentialSender(
    indexSequence,
    makeFilteredSender(indexedSender, sendableIndices),
    true,
  );

  let nextInput = null;
  while (true) {
    // Service results and effects yielded by the sendable sender,
    // as well as inputs specifying any indices to mark as sendable
    if (sendableIndices.size === 0) {
      const newlySendableIndices = (yield makeSenderYieldResult(null)) as Set<Index>;
      for (const index of newlySendableIndices) {
        sendableIndices.add(index);
      }
    } else {
      const yieldValue: SenderYield<TaggedStateSegment<Index, StateSegment>> = sendableSender.next(
        nextInput,
      ).value;
      switch (yieldValue.type) {
        case GeneratorYieldType.Result: {
          nextInput = null;
          if (yieldValue.value !== null) {
            const { type: index } = yieldValue.value as TaggedStateSegment<Index, StateSegment>;
            sendableIndices.delete(index);
            // console.log(`Event: Sending notification for ${index}`);
          }
          const newlySendableIndices = (yield yieldValue) as Set<Index>;
          for (const index of newlySendableIndices) {
            sendableIndices.add(index);
          }
          break;
        }
        case GeneratorYieldType.Effect:
          nextInput = yield yieldValue;
          break;
      }
    }
  }
}

export interface ConnectionStatus {
  lastConnectionTime: Date | null;
}

// An event sender uses a sequence of Indices to yield outputs from an IndexedSender,
// as well as any redux-saga effects yielded from the IndexedSender.
// This implementation of the changed state sender does not generate any states when no
// states have changed (i.e. it does not produce any outputs when "idle").
export function* changedStateSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  allStates: IndexedSender<Index, TaggedStateSegment<Index, StateSegment>>,
  connectionStatus: ConnectionStatus,
): Sender<TaggedStateSegment<Index, StateSegment>> {
  const sender = notificationSender(indexSequence, allStates);
  const trackableStates = new Set(indexSequence);
  const prevStates = new Map<Index, StateSegment>();
  const prevConnectionStatus = { ...connectionStatus };

  let nextSenderInput: any = new Set<Index>();
  while (true) {
    // Service results and effects yielded by the sender, as well as any reset events
    const senderYieldValue: SenderYield<TaggedStateSegment<Index, StateSegment>> = sender.next(
      nextSenderInput,
    ).value;
    switch (senderYieldValue.type) {
      case GeneratorYieldType.Result: {
        nextSenderInput = new Set<Index>();
        // Prepare to handle connection changes
        const newConnection =
          connectionStatus.lastConnectionTime !== prevConnectionStatus.lastConnectionTime;
        if (newConnection) {
          // We just connected, so prepare to tell the sender to mark all states as sendable
          // console.log('Event: responding to a new connection!');
        }
        prevConnectionStatus.lastConnectionTime = connectionStatus.lastConnectionTime;
        // Prepare to tell the sender to mark changed states as sendable
        for (const index of trackableStates) {
          const stateGetter = allStates(index);
          let nextGetterInput = null;
          let newState = null;
          // Get the new value of the state
          while (true) {
            // Service results and effects yielded by the allStates getter
            const nextGetterYield = stateGetter.next(nextGetterInput);
            switch (nextGetterYield.value.type) {
              case GeneratorYieldType.Result: {
                nextGetterInput = null;
                const getterYieldResult = nextGetterYield.value.value as TaggedStateSegment<
                  Index,
                  StateSegment
                > | null;
                if (getterYieldResult !== null) {
                  newState = getterYieldResult.value;
                }
                break;
              }
              case GeneratorYieldType.Effect:
                nextGetterInput = yield nextGetterYield.value;
                break;
            }
            if (nextGetterYield.done) {
              // The allStates getter has finished working and returned its result,
              // so it no longer has any results or effects to service
              break;
            }
          }
          // Prepare to tell the sender to mark the state as sendable, if it changed
          if (newState !== null && (newConnection || !_.isEqual(newState, prevStates.get(index)))) {
            // console.log(`Event: State ${index} changed!`)
            nextSenderInput.add(index);
            prevStates.set(index, newState);
          }
        }
        yield senderYieldValue;
        break;
      }
      case GeneratorYieldType.Effect:
        nextSenderInput = yield senderYieldValue;
        break;
    }
  }
}
