import { ActiveLogEvents, SensorMeasurements } from '../proto/mcu_pb';
import {
  MessageType,
  PBMessage,
  StateUpdateAction,
  STATE_UPDATED,
  WaveformHistory,
  PVHistory,
  commitAction,
  BACKEND_CONNECTION_LOST,
  SmoothingData,
} from '../types';

export const sensorMeasurementSmoothingReducer = (
  averageResponsiveness: number,
  changeValueThreshold: number,
  convergenceDurationThreshold: number,
  changeDurationThreshold: number,
  valueAccessor: (measurements: SensorMeasurements) => number,
) => (
  state: SmoothingData = { raw: NaN, average: NaN, converged: NaN, smoothed: NaN },
  action: StateUpdateAction,
): SmoothingData => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === MessageType.SensorMeasurements) {
        const newState = action.state as SensorMeasurements;
        const { time } = newState;
        const raw = valueAccessor(newState);
        const oldAverage = Number.isNaN(state.average) ? raw : state.average;
        const average = averageResponsiveness * raw + (1 - averageResponsiveness) * oldAverage;
        const previousTime = state.time || time;
        let { converged, smoothed, convergenceStartTime, changeStartTime } = state;
        if (
          (Number.isNaN(converged) && Math.abs(average - oldAverage) < changeValueThreshold) ||
          Math.abs(average - converged) < changeValueThreshold
        ) {
          // Value may be converging or converged
          changeStartTime = undefined;
          if (convergenceStartTime === undefined) {
            convergenceStartTime = time;
          }
          if (
            time - convergenceStartTime >= convergenceDurationThreshold &&
            previousTime - convergenceStartTime < convergenceDurationThreshold
          ) {
            // Value has just converged
            converged = average;
            smoothed = converged;
          }
        } else {
          // Value may be starting a transition to a different level
          convergenceStartTime = undefined;
          if (changeStartTime === undefined) {
            changeStartTime = time;
          }
          if (time - changeStartTime >= changeDurationThreshold) {
            // Value is transitioning to a different level
            smoothed = average;
            converged = NaN;
          }
        }
        return {
          raw,
          average,
          converged,
          smoothed,
          time,
          convergenceStartTime,
          changeStartTime,
        };
      }
      return state;
    default:
      return state;
  }
};

export const waveformHistoryReducer = <T extends PBMessage>(
  messageType: MessageType,
  getTime: (values: T) => number,
  getValue: (values: T) => number,
  bufferDuration = 60,
  segmentUpdateOffset = 0,
  maxDuration = 10000,
  gapDuration = 500,
  maxSegmentDuration = 2500,
) => (
  state: WaveformHistory = {
    waveformOld: {
      full: [],
    },
    waveformNew: {
      full: [],
      buffer: [],
      segmented: [[]],
    },
    waveformNewStart: 0,
  },
  action: StateUpdateAction,
): WaveformHistory => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === messageType) {
        const sampleTime = getTime(action.state as T);
        const lastTime =
          state.waveformNew.full.length === 0
            ? state.waveformNewStart
            : state.waveformNew.full[state.waveformNew.full.length - 1].date;
        if (
          sampleTime > state.waveformNewStart + maxDuration ||
          sampleTime < lastTime ||
          sampleTime - gapDuration - state.waveformNewStart > lastTime
        ) {
          // make waveformNew start over
          const newPoint = {
            date: 0,
            value: getValue(action.state as T),
          };
          return {
            waveformOld: {
              full: state.waveformNew.full,
            },
            waveformNew: {
              full: [],
              buffer: [newPoint],
              segmented: [[]],
            },
            waveformNewStart: sampleTime,
          };
        }

        // update buffer
        let buffered = [...state.waveformNew.buffer];
        const newPointTime = sampleTime - state.waveformNewStart;
        const newPoint = {
          date: newPointTime,
          value: getValue(action.state as T),
        };
        buffered = buffered.concat([newPoint]);

        // apply segment update offset
        let segments = [...state.waveformNew.segmented];
        const bufferedStart = buffered[0].date;
        const bufferedEnd = buffered[buffered.length - 1].date;
        if (segments.length === 1 && bufferedEnd - bufferedStart < segmentUpdateOffset) {
          return {
            ...state,
            waveformNew: {
              ...state.waveformNew,
              buffer: buffered,
            },
          };
        }

        // update segmented
        const lastSegment = segments[segments.length - 1];
        if (lastSegment.length === 0) {
          segments[segments.length - 1] = buffered;
          buffered = [];
        } else {
          const lastSegmentStart = lastSegment[0].date;
          const lastSegmentEnd = lastSegment[lastSegment.length - 1].date;
          if (newPointTime - lastSegmentEnd >= bufferDuration) {
            const lastSegmentDuration = newPointTime - lastSegmentStart;
            if (lastSegmentDuration >= maxSegmentDuration) {
              // start a new segment, but add an overlap of points
              segments[segments.length - 1] = lastSegment.concat([buffered[0]]);
              segments = segments.concat([buffered]);
            } else {
              segments[segments.length - 1] = lastSegment.concat(buffered);
            }
            buffered = [];
          }
        }

        return {
          ...state,
          waveformNew: {
            full: state.waveformNew.full.concat([newPoint]),
            buffer: buffered,
            segmented: segments,
          },
        };
      }
      return state;
    default:
      return state;
  }
};

export const pvHistoryReducer = (
  state: PVHistory = {
    loop: [],
    loopOrigin: {
      pressure: 0,
      volume: 0,
    },
    cycle: 0,
  },
  action: StateUpdateAction,
): PVHistory => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === MessageType.SensorMeasurements) {
        const newState = action.state as SensorMeasurements;
        const { cycle, paw, volume } = newState;
        if (cycle !== state.cycle) {
          // make loop start over
          return {
            loop: [{ pressure: 0, volume: 0 }],
            loopOrigin: { pressure: paw, volume },
            cycle,
          };
        }

        // add point to loop
        const newPoint = {
          pressure: paw - state.loopOrigin.pressure,
          volume: volume - state.loopOrigin.volume,
        };
        return {
          ...state,
          loop: state.loop.concat([newPoint]),
        };
      }
      return state;
    default:
      return state;
  }
};

export const activeLogEventsReducer = (
  state: ActiveLogEvents = ActiveLogEvents.fromJSON({
    id: [],
  }),
  action: commitAction | StateUpdateAction,
): ActiveLogEvents => {
  switch (action.type) {
    case STATE_UPDATED: {
      const actionClone = { ...(action as StateUpdateAction) };
      if (actionClone.messageType === MessageType.ActiveLogEvents) {
        return actionClone.state as ActiveLogEvents;
      }
      return state;
    }
    case BACKEND_CONNECTION_LOST:
      state.id.push((action.update.id as unknown) as number);
      return { ...state, id: state.id };
    default:
      return state;
  }
};
