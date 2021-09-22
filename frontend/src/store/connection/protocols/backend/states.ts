import { OutputSelector } from 'reselect';
import { SelectEffect, select } from 'redux-saga/effects';
import { StoreState } from '../../../types';
import { PBMessage, MessageType } from '../../../proto/types';
import { getParametersRequest } from '../../../controller/selectors/measurements';
import {
  getAlarmLimitsRequest,
  getFullExpectedLogEvent,
  getAlarmMuteRequest,
  getScreenStatusRequest,
} from '../../../controller/selectors';
import {
  getSystemSettingsRequest,
  getFrontendDisplaySetting,
} from '../../../controller/selectors/frontend_pb';
import { getLastBackendConnectionTime } from '../../selectors';
import {
  TaggedStateSegment,
  makeStateMapSender,
  makeMappedSenders,
  sequentialSender,
} from '../application/states';
import { ConnectionStatus, changedStateSender } from '../application/events';
import { GeneratorYield, makeYieldResult, makeYieldEffect } from '../sagas';

// Store Access

type StateSelector = OutputSelector<
  StoreState,
  PBMessage | null,
  // We have no way of specifying the input argument type for the selectors, so we must use any.
  // eslint-disable @typescript-eslint/no-explicit-any
  // eslint-disable-next-line
  (arg: any) => PBMessage | null
>;
const selectors = new Map<MessageType, StateSelector>([
  [MessageType.AlarmLimitsRequest, getAlarmLimitsRequest],
  [MessageType.ParametersRequest, getParametersRequest],
  [MessageType.ExpectedLogEvent, getFullExpectedLogEvent],
  [MessageType.AlarmMuteRequest, getAlarmMuteRequest],
  [MessageType.ScreenStatusRequest, getScreenStatusRequest],
  [MessageType.SystemSettingsRequest, getSystemSettingsRequest],
  [MessageType.FrontendDisplaySetting, getFrontendDisplaySetting],
]);

// State Sending

enum SenderType {
  eventSchedule = 0,
  mainSchedule = 1,
}
export const sendMinInterval = 50; // ms
export const sendMaxInterval = 100; // ms
const sendRootSchedule = [SenderType.mainSchedule].concat(
  Array(sendMaxInterval / sendMinInterval - 1).fill(SenderType.eventSchedule),
);
const sendSchedule = [
  MessageType.ExpectedLogEvent,
  MessageType.ParametersRequest,
  MessageType.AlarmLimitsRequest,
  MessageType.AlarmMuteRequest,
  MessageType.ScreenStatusRequest,
  MessageType.SystemSettingsRequest,
  MessageType.FrontendDisplaySetting,
];

export type TaggedPBMessage = TaggedStateSegment<MessageType, PBMessage>;
export type SenderYieldResult = TaggedPBMessage | null;
export type SenderYieldEffect = SelectEffect;
export type SenderYield = GeneratorYield<SenderYieldResult, SenderYieldEffect>;
export type SenderInputs = PBMessage | Date | null;
// SagaSender is tagged as returning SenderYield to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
type SagaSender = Generator<SenderYield, SenderYield, SenderInputs>;

// This generator yields tagged unions of MessageType and PBMessage,
// as well as redux-saga effects (for selecting PBMessages from the store, and
// which require an input into the generator's subsequent next() method call).
export function* backendStateSender(): SagaSender {
  const states = new Map<MessageType, PBMessage | null>();
  const stateMapSender = makeStateMapSender(states);
  const connectionStatus: ConnectionStatus = { lastConnectionTime: null };
  const eventSender = changedStateSender(sendSchedule, stateMapSender, connectionStatus, false);
  const mainSender = sequentialSender(sendSchedule, stateMapSender);
  const childSenders = new Map([
    [SenderType.eventSchedule, eventSender],
    [SenderType.mainSchedule, mainSender],
  ]);
  const rootSender = sequentialSender(sendRootSchedule, makeMappedSenders(childSenders), false);
  while (true) {
    // Update connection status
    connectionStatus.lastConnectionTime = (yield makeYieldEffect(
      select(getLastBackendConnectionTime),
    )) as Date | null;
    // Update states for the mappedStateSender
    // We must use for(... of ...) here because we need to yield inside the loop:
    // eslint-disable no-restricted-syntax
    // eslint-disable-next-line
    for (const [index, selector] of selectors) {
      const newState = (yield makeYieldEffect(select(selector))) as PBMessage | null;
      states.set(index, newState);
    }
    // Generate the next output from the root sender
    yield makeYieldResult(rootSender.next().value);
  }
}
