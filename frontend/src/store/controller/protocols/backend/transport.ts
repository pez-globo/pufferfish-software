import { MessageType, PBMessage, MessageClass, MessageTypes } from '../../types';
import {
  MessageSerializer,
  makeMessageSerializer,
  makeMessageDeserializer,
} from '../transport/messages';
import {
  ParametersRequest,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  AlarmMuteRequest,
} from '../../proto/mcu_pb';
import { FrontendDisplaySetting } from '../../proto/frontend_pb';

// Serialization

const makeSerializer = makeMessageSerializer<MessageType, PBMessage>(MessageTypes);
const Serializers = new Map<MessageType, MessageSerializer<PBMessage>>([
  [MessageType.AlarmLimitsRequest, makeSerializer<AlarmLimitsRequest>(AlarmLimitsRequest)],
  [MessageType.ParametersRequest, makeSerializer<ParametersRequest>(ParametersRequest)],
  [MessageType.ExpectedLogEvent, makeSerializer<ExpectedLogEvent>(ExpectedLogEvent)],
  [MessageType.AlarmMuteRequest, makeSerializer<AlarmMuteRequest>(AlarmMuteRequest)],
  [MessageType.FrontendDisplaySetting, makeSerializer<FrontendDisplaySetting>(FrontendDisplaySetting)],
]);
export const serialize = (messageType: MessageType, pbMessage: PBMessage): Uint8Array => {
  const serializer = Serializers.get(messageType);
  if (serializer === undefined) {
    throw new Error(`Backend: missing message serializer for ${MessageType[messageType]}`);
  }

  return serializer(pbMessage);
};

// Deserialization

export const deserialize = makeMessageDeserializer<MessageType, PBMessage>(MessageClass);
