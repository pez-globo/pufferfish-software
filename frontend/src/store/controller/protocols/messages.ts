import { BufferReader } from 'protobufjs/minimal';
import { MessageClass, MessageTypes, PBMessage, PBMessageType } from '../types';
import { ParametersRequest, AlarmLimitsRequest } from '../proto/mcu_pb';
import { updateState } from '../actions';

export interface MessageParseResults {
  messageType: number;
  pbMessage: PBMessage;
}

export const deserializeMessage = (body: Uint8Array): MessageParseResults | undefined => {
  const messageType = body[0];
  const messageBody = body.slice(1);
  const messageBodyReader = new BufferReader(messageBody);
  const messageClass = MessageClass.get(messageType);
  if (messageClass === undefined) {
    // TODO: raise an exception instead?
    return undefined;
  }

  const pbMessage = messageClass.decode(messageBodyReader);
  return { messageType, pbMessage };
};

export const serializeMessage = <T extends PBMessage>(pbMessageType: PBMessageType) => (
  pbMessage: PBMessage,
): Uint8Array | undefined => {
  const messageType = MessageTypes.get(pbMessageType);
  if (messageType === undefined) {
    // TODO: raise an exception instead?
    return undefined;
  }

  // It's not clear whether there is any reasonable way to make the types work
  // here. A fundamentally different design might make it possible to have
  // specified types for pbMessageType, but for now we'll just delegate the
  // responsibility of using types correctly to the calling code.
  // eslint-disable @typescript-eslint/no-explicit-any
  const messageBody = (pbMessageType as any).encode(pbMessage as T).finish();
  const buffer = new Uint8Array(1 + messageBody.length);
  buffer.set([messageType], 0);
  buffer.set(messageBody, 1);
  return buffer;
};

// Dynamic dispatch
export type MessageSerializer = (pbMessage: PBMessage) => Uint8Array | undefined;
export const MessageSerializers = new Map<PBMessageType, MessageSerializer>([
  [AlarmLimitsRequest, serializeMessage<AlarmLimitsRequest>(AlarmLimitsRequest)],
  [ParametersRequest, serializeMessage<ParametersRequest>(ParametersRequest)],
]);
