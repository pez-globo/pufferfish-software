import { OutputSelector } from 'reselect';
import { SelectEffect, select } from 'redux-saga/effects';
import { GeneratorYieldType, GeneratorYield, makeYieldResult, makeYieldEffect } from '../sagas';

export type SenderYieldEffect = SelectEffect;
type SenderYieldResult<StateSegment> = StateSegment | null;
export type SenderYield<StateSegment> = GeneratorYield<
  SenderYieldResult<StateSegment>,
  SenderYieldEffect
>;
export const makeSenderYieldResult = <StateSegment>(
  result: SenderYieldResult<StateSegment>,
): SenderYield<StateSegment> =>
  makeYieldResult<SenderYieldResult<StateSegment>, SenderYieldEffect>(result);
export const makeSenderYieldEffect = <StateSegment>(
  effect: SenderYieldEffect,
): SenderYield<StateSegment> =>
  makeYieldEffect<SenderYieldResult<StateSegment>, SenderYieldEffect>(effect);

// A sender should yield a sequence of optional StateSegments and/or redux-saga effects.
export type Sender<StateSegment> = Generator<
  SenderYield<StateSegment>,
  SenderYield<StateSegment>,
  unknown
>;

// Selector-based sending

type StateSelector<StoreState, StateSegment> = OutputSelector<
  StoreState,
  StateSegment | null,
  // We have no way of specifying the input argument type for the selectors,
  // so we have to use any here.
  // eslint-disable @typescript-eslint/no-explicit-any
  // eslint-disable-next-line
  (arg: any) => StateSegment | null
>;

export interface TaggedStateSegment<StateSegmentType, StateSegment> {
  type: StateSegmentType;
  value: StateSegment;
}

// Use selectorSender to make a Sender from a selector. It yields redux-saga effects
// and TaggedStateSegments (which is a tagged union for StateSegment).
export function* selectorSender<StoreState, StateSegmentType, StateSegment>(
  stateSegmentType: StateSegmentType,
  selector: StateSelector<StoreState, StateSegment>,
): Sender<TaggedStateSegment<StateSegmentType, StateSegment>> {
  while (true) {
    const stateSegment = yield makeSenderYieldEffect(select(selector));
    if (stateSegment === null) {
      yield makeSenderYieldResult(null);
    } else {
      yield makeSenderYieldResult({
        type: stateSegmentType,
        value: stateSegment as StateSegment,
      });
    }
  }
}

// Indexed sending

// An IndexedSender should return a SenderYieldResult holding the optional StateSegment produced
// for the given index. Before it returns, it may yield SenderYieldEffects for redux-saga
// effects.
export interface IndexedSender<Index, StateSegment> {
  (index: Index): Sender<StateSegment>;
}

// Use makeMappedSenders to make an IndexedSender from a Map of Indices to Senders.
export const makeMappedSenders = <Index, StateSegment>(senders: Map<Index, Sender<StateSegment>>) =>
  function* (index: Index): Sender<StateSegment> {
    const sender = senders.get(index);
    if (sender === undefined) {
      throw new Error(`Backend: invalid sender index`);
    }

    let nextInput = null;
    while (true) {
      // Service results and effects yielded by the sender
      const yieldValue: SenderYield<StateSegment> = sender.next(nextInput).value;
      switch (yieldValue.type) {
        case GeneratorYieldType.Result:
          nextInput = null;
          return yieldValue;
        case GeneratorYieldType.Effect:
          nextInput = yield yieldValue;
          break;
        default:
          throw new Error('Unhandled generator yield type!');
          break;
      }
    }
  };

// Sequential send scheduling

function* sequentialScheduler<T>(sequence: Array<T>): Generator<T, T, undefined> {
  while (true) {
    yield* sequence;
  }
}

// A sequential sender uses a sequence of Indices to yield outputs from an IndexedSender,
// as well as any redux-saga effects yielded from the IndexedSender.
export function* sequentialSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  indexedSender: IndexedSender<Index, StateSegment>,
  skipUnavailable = false,
): Sender<StateSegment> {
  const schedule = sequentialScheduler(indexSequence);
  while (true) {
    const index = schedule.next().value;
    const sender = indexedSender(index);
    let nextInput = null;
    let skipped = 0;
    while (true) {
      // Service results and effects yielded by the indexed sender
      const nextYield = sender.next(nextInput);
      switch (nextYield.value.type) {
        case GeneratorYieldType.Result:
          nextInput = null;
          if (
            nextYield.value.value !== null ||
            !skipUnavailable ||
            skipped >= indexSequence.length
          ) {
            skipped = 0;
            yield nextYield.value;
          } else {
            skipped += 1;
          }
          break;
        case GeneratorYieldType.Effect:
          nextInput = yield nextYield.value;
          break;
        default:
          throw new Error('Unhandled generator yield type!');
          break;
      }
      if (nextYield.done) {
        // The indexed sender has finished working and returned its result,
        // so it no longer has any results or effects to service.
        break;
      }
    }
  }
}
