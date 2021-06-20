import { OutputSelector } from 'reselect';
import { PBMessage, PBMessageType } from '../../types';
import { StoreState } from '../../../types';
import {
  getParametersRequest,
  getAlarmLimitsRequest,
  getFullExpectedLogEvent,
  getAlarmMuteRequest,
} from '../../selectors';
import advanceSchedule from '../application/states';
import {
  ParametersRequest,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  AlarmMuteRequest,
} from '../../proto/mcu_pb';

// State Sending

export enum Sender {
  fastSchedule = 0,
  slowSchedule = 1,
}
export const sendRootSchedule = [Sender.fastSchedule, Sender.slowSchedule];
export const sendFastSchedule = [ExpectedLogEvent];
export const sendSlowSchedule = [ParametersRequest, AlarmLimitsRequest, AlarmMuteRequest];

export const sendInterval = 50; // ms

// This generator is tagged as returning PBMessageType to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
export function* sequentialStateSender(): Generator<PBMessageType, PBMessageType, unknown> {
  const rootSchedule = Array.from(sendRootSchedule);
  const fastSchedule = Array.from(sendFastSchedule);
  const slowSchedule = Array.from(sendSlowSchedule);
  const schedules = new Map<Sender, Array<PBMessageType>>([
    [Sender.fastSchedule, fastSchedule],
    [Sender.slowSchedule, slowSchedule],
  ]);
  while (true) {
    const sender = advanceSchedule(rootSchedule);
    const schedule = schedules.get(sender);
    if (schedule === undefined) {
      console.error('Invalid sender type', sender);
    } else {
      yield advanceSchedule(schedule);
    }
  }
}

// Store

type StateSelector = OutputSelector<
  StoreState,
  PBMessage | null,
  // We have no way of specifying the input argument type for the selectors,
  // so we have to use any here.
  // eslint-disable @typescript-eslint/no-explicit-any
  // eslint-disable-next-line
  (arg: any) => PBMessage | null
>;

export const MessageSelectors = new Map<PBMessageType, StateSelector>([
  [AlarmLimitsRequest, getAlarmLimitsRequest],
  [ParametersRequest, getParametersRequest],
  [ExpectedLogEvent, getFullExpectedLogEvent],
  [AlarmMuteRequest, getAlarmMuteRequest],
]);

export const getSelector = (pbMessageType: PBMessageType): StateSelector => {
  const selector = MessageSelectors.get(pbMessageType);
  if (selector === undefined) {
    throw new Error(`Backend: missing selector for ${pbMessageType}`);
  }

  return selector;
};
