import { OutputSelector } from 'reselect';
import { SelectEffect, select } from 'redux-saga/effects';
import { GeneratorYieldType, GeneratorYield, makeYieldResult, makeYieldEffect } from '../sagas';

export type SenderYieldEffect = SelectEffect;
type SenderYieldResult<StateSegment> = StateSegment | null;
export type SenderYield<StateSegment> = GeneratorYield<
  SenderYieldResult<StateSegment>,
  SenderYieldEffect
>;
const makeSenderYieldResult = <StateSegment>(result: SenderYieldResult<StateSegment>) =>
  makeYieldResult<SenderYieldResult<StateSegment>, SenderYieldEffect>(result);
const makeSenderYieldEffect = <StateSegment>(effect: SenderYieldEffect) =>
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
    const stateSegment = yield makeSenderYieldEffect<
      TaggedStateSegment<StateSegmentType, StateSegment>
    >(select(selector));
    if (stateSegment === null) {
      yield makeSenderYieldResult<TaggedStateSegment<StateSegmentType, StateSegment>>(null);
    } else {
      yield makeSenderYieldResult<TaggedStateSegment<StateSegmentType, StateSegment>>({
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

// Sequential send scheduling

function* sequentialScheduler<T>(sequence: Array<T>): Generator<T, T, void> {
  while (true) {
    yield* sequence;
  }
}

// A sequential sender uses a sequence of Indices to yield outputs from an IndexedSender,
// as well as any redux-saga effects yielded from the IndexedSender.
export function* sequentialSender<Index, StateSegment>(
  sequence: Array<Index>,
  indexedSender: IndexedSender<Index, StateSegment>,
  skipUnavailable = false
): Sender<StateSegment> {
  const schedule = sequentialScheduler<Index>(sequence);
  while (true) {
    const index = schedule.next().value;
    const sender = indexedSender(index);
    let nextInput = null;
    let skipped = 0;
    while (true) {
      const nextYield = sender.next(nextInput);
      switch (nextYield.value.type) {
        case GeneratorYieldType.Result:
          nextInput = null;
          if (nextYield.value.value !== null || !skipUnavailable || skipped >= sequence.length) {
            skipped = 0;
            yield nextYield.value;
          } else {
            skipped += 1;
          }
          break;
        case GeneratorYieldType.Effect:
          nextInput = yield nextYield.value;
          break;
      }
      if (nextYield.done) {
        break;
      }
    }
  }
}
