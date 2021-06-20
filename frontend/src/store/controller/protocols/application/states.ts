// Due to the way redux-saga's select works for getting StateSegments from the store,
// these senders are used to produce PBMessageTypes specifying StateSegments to select
// from the store, rather than producing PBMessage objects which are the actual contents
// of StateSegments.

export type Sender<StateSegment> = Generator<StateSegment, StateSegment, unknown>;

// Indexed sending

export interface IndexedSender<Index, StateSegment> {
  (index: Index): StateSegment | undefined;
}

export const passthroughSender = <Index extends StateSegment, StateSegment>(
  index: Index,
): StateSegment => index;

export const makeMappedSenders = <Index, StateSegment>(
  senders: Map<Index, Sender<StateSegment>>,
): IndexedSender<Index, StateSegment> => (index: Index): StateSegment | undefined => {
  const sender = senders.get(index);
  if (sender === undefined) {
    console.error('Invalid sender type', sender);
    return undefined;
  }

  return sender.next().value;
};

// Sequential send scheduling

function* sequentialScheduler<T>(sequence: Array<T>): Generator<T, T, unknown> {
  while (true) {
    yield* sequence;
  }
}

// This generator is tagged as returning StateSegment to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
export function* sequentialSender<Index, StateSegment>(
  sequence: Array<Index>,
  indexedSender: IndexedSender<Index, StateSegment>,
): Sender<StateSegment> {
  const schedule = sequentialScheduler<Index>(sequence);
  while (true) {
    const index = schedule.next().value;
    const stateSegment = indexedSender(index);
    if (stateSegment !== undefined) {
      yield stateSegment;
    }
  }
}
