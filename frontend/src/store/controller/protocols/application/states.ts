// A sender should yield a sequence of optional StateSegments.
export type Sender<StateSegment> = Generator<StateSegment | null, StateSegment | null, unknown>;

// Indexed sending

// An IndexedSender should return an optional StateSegment produced for the given index.
export interface IndexedSender<Index, StateSegment> {
  (index: Index): StateSegment | null;
}

export interface TaggedStateSegment<StateSegmentType, StateSegment> {
  type: StateSegmentType;
  value: StateSegment;
}

// Use makeStateMapSender to make an IndexedSender of TaggedStateSegments from a
// Map of Indices to StateSegments.
export const makeStateMapSender = <Index, StateSegment>(
  states: Map<Index, StateSegment | null>,
) => (index: Index): TaggedStateSegment<Index, StateSegment> | null => {
  const state = states.get(index);
  if (state === undefined) {
    throw new Error(`Backend: invalid state index`);
  }

  if (state === null) {
    return null;
  }

  return {
    type: index,
    value: state,
  };
};

// Use makeMappedSenders to make an IndexedSender from a Map of Indices to Senders.
export const makeMappedSenders = <Index, StateSegment>(
  senders: Map<Index, Sender<StateSegment>>,
) => (index: Index): StateSegment | null => {
  const sender = senders.get(index);
  if (sender === undefined) {
    throw new Error(`Backend: invalid sender index`);
  }

  return sender.next().value;
};

// Sequential send scheduling

function* sequentialScheduler<T>(sequence: Array<T>): Generator<T, T, undefined> {
  while (true) {
    yield* sequence;
  }
}

// A sequential sender uses a sequence of Indices to yield outputs from an IndexedSender.
export function* sequentialSender<Index, StateSegment>(
  indexSequence: Array<Index>,
  indexedSender: IndexedSender<Index, StateSegment>,
  skipUnavailable = false,
): Sender<StateSegment> {
  const schedule = sequentialScheduler(indexSequence);

  let skipped = 0;
  while (true) {
    const index = schedule.next().value;
    const state = indexedSender(index);
    if (state !== null || !skipUnavailable || skipped >= indexSequence.length) {
      skipped = 0;
      yield state;
    } else {
      skipped += 1;
    }
  }
}
