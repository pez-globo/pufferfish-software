import { PBMessage, PBMessageType } from '../../types';
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
  selectorSender,
  makeMappedSenders,
  sequentialSender,
} from '../application/states';
import {
  ParametersRequest,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  AlarmMuteRequest,
} from '../../proto/mcu_pb';
import { FrontendDisplaySetting } from '../../proto/frontend_pb';
import { GeneratorYieldType } from '../sagas';

// Store

export type TaggedPBMessage = TaggedStateSegment<PBMessageType, PBMessage>;

const SelectorSenders = new Map<PBMessageType, Sender<TaggedPBMessage>>([
  [AlarmLimitsRequest, selectorSender(AlarmLimitsRequest, getAlarmLimitsRequest)],
  [ParametersRequest, selectorSender(ParametersRequest, getParametersRequest)],
  [ExpectedLogEvent, selectorSender(ExpectedLogEvent, getFullExpectedLogEvent)],
  [AlarmMuteRequest, selectorSender(AlarmMuteRequest, getAlarmMuteRequest)],
  [FrontendDisplaySetting, selectorSender(FrontendDisplaySetting, getFrontendDisplaySetting)],
]);

// State Sending

enum SenderType {
  fastSchedule = 0,
  slowSchedule = 1,
}
const sendRootSchedule = [SenderType.fastSchedule, SenderType.slowSchedule];
const sendFastSchedule = [ExpectedLogEvent];
const sendSlowSchedule = [
  ParametersRequest,
  AlarmLimitsRequest,
  AlarmMuteRequest,
  FrontendDisplaySetting,
];

export const sendInterval = 50; // ms

// The backend state sender yields tagged unions of PBMessageType and PBMessage
// as well as redux-saga effects (for selecting PBMessages from the store).
export function* backendStateSender(): Sender<TaggedPBMessage> {
  const selectorSenders = makeMappedSenders(SelectorSenders);
  const fastSender = sequentialSender<PBMessageType, TaggedPBMessage>(
    sendFastSchedule,
    selectorSenders,
  );
  const slowSender = sequentialSender<PBMessageType, TaggedPBMessage>(
    sendSlowSchedule,
    selectorSenders,
  );
  const childSenders = new Map<SenderType, Sender<TaggedPBMessage>>([
    [SenderType.fastSchedule, fastSender],
    [SenderType.slowSchedule, slowSender],
  ]);
  const rootSender = sequentialSender(sendRootSchedule, makeMappedSenders(childSenders));
  let nextInput = null;
  while (true) {
      const yieldValue: SenderYield<TaggedPBMessage> = rootSender.next(nextInput).value;
      switch (yieldValue.type) {
        case GeneratorYieldType.Result:
          nextInput = null;
          yield yieldValue;
          break;
        case GeneratorYieldType.Effect:
          nextInput = yield yieldValue;
          break;
      }
  }
}
