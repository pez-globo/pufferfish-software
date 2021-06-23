import { OutputSelector } from 'reselect';
import { PBMessage, PBMessageType } from '../../types';
import { StoreState } from '../../../types';
import {
  getParametersRequest,
  getAlarmLimitsRequest,
  getFullExpectedLogEvent,
  getAlarmMuteRequest,
} from '../../selectors';
import {
  Sender,
  passthroughSender,
  makeMappedSenders,
  sequentialSender,
} from '../application/states';
import {
  ParametersRequest,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  AlarmMuteRequest,
} from '../../proto/mcu_pb';

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

const MessageSelectors = new Map<PBMessageType, StateSelector>([
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

// State Sending

enum SenderType {
  fastSchedule = 0,
  slowSchedule = 1,
}
const sendRootSchedule = [SenderType.fastSchedule, SenderType.slowSchedule];
const sendFastSchedule = [ExpectedLogEvent];
const sendSlowSchedule = [ParametersRequest, AlarmLimitsRequest, AlarmMuteRequest];

export const sendInterval = 50; // ms

// This generator is tagged as returning PBMessageType to make typescript eslinting
// behave nicely, but it actually never returns (it only yields).
export function* backendSender(): Generator<PBMessageType, PBMessageType, unknown> {
  const fastSender = sequentialSender<PBMessageType, PBMessageType>(
    sendFastSchedule,
    passthroughSender,
  );
  const slowSender = sequentialSender<PBMessageType, PBMessageType>(
    sendSlowSchedule,
    passthroughSender,
  );
  const childSenders = new Map<SenderType, Sender<PBMessageType>>([
    [SenderType.fastSchedule, fastSender],
    [SenderType.slowSchedule, slowSender],
  ]);
  const rootSender = sequentialSender(sendRootSchedule, makeMappedSenders(childSenders));
  while (true) {
    yield rootSender.next().value;
  }
}
