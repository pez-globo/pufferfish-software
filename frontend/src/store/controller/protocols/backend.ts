import { PBMessageType } from '../types';
import {
  getParametersRequest,
  getAlarmLimitsRequest,
  getFullExpectedLogEvent,
  getAlarmMuteRequest,
} from '../selectors';
import { MessageSerializer, serializeMessage } from './messages';
import advanceSchedule from './states';
import {
  ParametersRequest,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  AlarmMuteRequest,
} from '../proto/mcu_pb';

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

// Transport

export const MessageSerializers = new Map<PBMessageType, MessageSerializer>([
  [AlarmLimitsRequest, serializeMessage<AlarmLimitsRequest>(AlarmLimitsRequest)],
  [ParametersRequest, serializeMessage<ParametersRequest>(ParametersRequest)],
  [ExpectedLogEvent, serializeMessage<ExpectedLogEvent>(ExpectedLogEvent)],
  [AlarmMuteRequest, serializeMessage<AlarmMuteRequest>(AlarmMuteRequest)],
]);

// Store

// The OutputSelector type is templated and so complicated that it's not clear
// whether we can specify its type in a Map, but for now we'll just delegate the
// responsibility of using types correctly to the calling code.
// eslint-disable @typescript-eslint/no-explicit-any
// eslint-disable-next-line
export const MessageSelectors = new Map<PBMessageType, any>([
  [AlarmLimitsRequest, getAlarmLimitsRequest],
  [ParametersRequest, getParametersRequest],
  [ExpectedLogEvent, getFullExpectedLogEvent],
  [AlarmMuteRequest, getAlarmMuteRequest],
]);

export interface StateProcessor {
  // This "any" is needed because MessageSelectors has a value type of "any".
  // eslint-disable @typescript-eslint/no-explicit-any
  // eslint-disable-next-line
  selector: any;
  serializer: MessageSerializer;
}

export const getStateProcessor = (pbMessageType: PBMessageType): StateProcessor => {
  const selector = MessageSelectors.get(pbMessageType);
  if (selector === undefined) {
    throw new Error(`Backend: missing selector for ${pbMessageType}`);
  }

  const serializer = MessageSerializers.get(pbMessageType);
  if (serializer === undefined) {
    throw new Error(`Backend: missing message serializer for ${pbMessageType}`);
  }

  return { selector, serializer };
};
