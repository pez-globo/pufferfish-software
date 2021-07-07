import { select } from 'redux-saga/effects';
import { getLastBackendConnectionTime } from '../../../app/selectors';
import { PBMessage, MessageType } from '../../types';
import {
  getParametersRequest,
  getAlarmLimitsRequest,
  getFullExpectedLogEvent,
  getAlarmMuteRequest,
  getFrontendDisplaySetting,
} from '../../selectors';
import {
  SenderYield,
  Sender,
  TaggedStateSegment,
  makeSenderYieldEffect,
  selectorSender,
  makeMappedSenders,
  sequentialSender,
} from '../application/states';
import { ConnectionStatus, changedStateSender } from '../application/events';
import { GeneratorYieldType } from '../sagas';

// Store

export type TaggedPBMessage = TaggedStateSegment<MessageType, PBMessage>;

const SelectorSenders = new Map<MessageType, Sender<TaggedPBMessage>>([
  [
    MessageType.AlarmLimitsRequest,
    selectorSender(MessageType.AlarmLimitsRequest, getAlarmLimitsRequest),
  ],
  [
    MessageType.ParametersRequest,
    selectorSender(MessageType.ParametersRequest, getParametersRequest),
  ],
  [
    MessageType.ExpectedLogEvent,
    selectorSender(MessageType.ExpectedLogEvent, getFullExpectedLogEvent),
  ],
  [MessageType.AlarmMuteRequest, selectorSender(MessageType.AlarmMuteRequest, getAlarmMuteRequest)],
  [
    MessageType.FrontendDisplaySetting,
    selectorSender(MessageType.FrontendDisplaySetting, getFrontendDisplaySetting),
  ],
]);

// State Sending

enum SenderType {
  eventSchedule = 0,
  mainSchedule = 1,
}
export const sendMinInterval = 20; // ms
export const sendMaxInterval = 500; // ms
const sendRootSchedule = [SenderType.mainSchedule].concat(
  Array(sendMaxInterval / sendMinInterval - 1).fill(SenderType.eventSchedule),
);
const sendSchedule = [
  MessageType.ExpectedLogEvent,
  MessageType.ParametersRequest,
  MessageType.AlarmLimitsRequest,
  MessageType.AlarmMuteRequest,
  MessageType.FrontendDisplaySetting,
];

// The backend state sender yields tagged unions of MessageType and PBMessage
// as well as redux-saga effects (for selecting PBMessages from the store).
export function* backendStateSender(): Sender<TaggedPBMessage> {
  const selectorSenders = makeMappedSenders(SelectorSenders);
  const connectionStatus: ConnectionStatus = { lastConnectionTime: null };
  const eventSender = changedStateSender(sendSchedule, selectorSenders, connectionStatus);
  const mainSender = sequentialSender(sendSchedule, selectorSenders);
  const childSenders = new Map<SenderType, Sender<TaggedPBMessage>>([
    [SenderType.eventSchedule, eventSender],
    [SenderType.mainSchedule, mainSender],
  ]);
  const rootSender = sequentialSender(sendRootSchedule, makeMappedSenders(childSenders), false);
  let nextInput = null;
  while (true) {
    // Service results and effects yielded by the root sender
    const yieldValue: SenderYield<TaggedPBMessage> = rootSender.next(nextInput).value;
    switch (yieldValue.type) {
      case GeneratorYieldType.Result:
        // Handle connection changes
        connectionStatus.lastConnectionTime = (yield makeSenderYieldEffect(
          select(getLastBackendConnectionTime),
        )) as Date | null;
        // Generate the next output from the root sender
        nextInput = null;
        yield yieldValue;
        break;
      case GeneratorYieldType.Effect:
        nextInput = yield yieldValue;
        break;
    }
  }
}
