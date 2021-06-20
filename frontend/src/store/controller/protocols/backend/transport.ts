import { PBMessageType, MessageType, PBMessage, MessageClass, MessageTypes } from '../../types';
import {
  MessageDeserializer,
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

export type Serializer = MessageSerializer<PBMessage>;
const makeSerializer = makeMessageSerializer<MessageType, PBMessage>(MessageTypes);
export const Serializers = new Map<PBMessageType, Serializer>([
  [AlarmLimitsRequest, makeSerializer<AlarmLimitsRequest>(AlarmLimitsRequest)],
  [ParametersRequest, makeSerializer<ParametersRequest>(ParametersRequest)],
  [ExpectedLogEvent, makeSerializer<ExpectedLogEvent>(ExpectedLogEvent)],
  [AlarmMuteRequest, makeSerializer<AlarmMuteRequest>(AlarmMuteRequest)],
]);

// Deserialization

export type Deserializer = MessageDeserializer<PBMessage>;
export const deserialize = makeMessageDeserializer<MessageType, PBMessage>(MessageClass);
