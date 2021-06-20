import { PBMessageType, MessageType, PBMessage, MessageClass, MessageTypes } from '../../types';
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

// Serialization

const makeSerializer = makeMessageSerializer<MessageType, PBMessage>(MessageTypes);
const Serializers = new Map<PBMessageType, MessageSerializer<PBMessage>>([
  [AlarmLimitsRequest, makeSerializer<AlarmLimitsRequest>(AlarmLimitsRequest)],
  [ParametersRequest, makeSerializer<ParametersRequest>(ParametersRequest)],
  [ExpectedLogEvent, makeSerializer<ExpectedLogEvent>(ExpectedLogEvent)],
  [AlarmMuteRequest, makeSerializer<AlarmMuteRequest>(AlarmMuteRequest)],
]);
export const serialize = (pbMessageType: PBMessageType, pbMessage: PBMessage): Uint8Array => {
  const serializer = Serializers.get(pbMessageType);
  if (serializer === undefined) {
    throw new Error(`Backend: missing message serializer for ${pbMessageType}`);
  }

  return serializer(pbMessage);
};

// Deserialization

export const deserialize = makeMessageDeserializer<MessageType, PBMessage>(MessageClass);
