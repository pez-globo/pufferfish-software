import { BufferReader } from 'protobufjs/minimal';

interface PBMessageType<PBMessage> {
  decode: (input: Uint8Array | BufferReader, length?: number | undefined) => PBMessage;
}

// Serialization

export interface MessageSerializer<PBMessage> {
  (pbMessage: PBMessage): Uint8Array;
}

export const makeMessageSerializer = <MessageType extends number, PBMessage>(
  messageTypes: Map<PBMessageType<PBMessage>, MessageType>,
) => <T extends PBMessage>(pbMessageType: PBMessageType<PBMessage>) => (
  pbMessage: PBMessage,
): Uint8Array => {
  const messageType = messageTypes.get(pbMessageType);
  if (messageType === undefined) {
    throw new Error(`Messages: missing message type for ${pbMessageType}`);
  }

  // It's not clear whether there is any reasonable way to make the types work
  // here. A fundamentally different design might make it possible to have
  // specified types for pbMessageType, but for now we'll just delegate the
  // responsibility of using types correctly to the calling code.
  // eslint-disable @typescript-eslint/no-explicit-any
  // eslint-disable-next-line
  const messageBody = (pbMessageType as any).encode(pbMessage as T).finish();
  const buffer = new Uint8Array(1 + messageBody.length);
  buffer.set([messageType], 0);
  buffer.set(messageBody, 1);
  return buffer;
};

// Deserialization

interface MessageParseResults<PBMessage> {
  messageType: number;
  pbMessage: PBMessage;
}

export const makeMessageDeserializer = <MessageType extends number, PBMessage>(
  messageClasses: Map<MessageType | number, PBMessageType<PBMessage>>,
) => (body: Uint8Array): MessageParseResults<PBMessage> => {
  const messageType = body[0];
  const messageBody = body.slice(1);
  const messageBodyReader = new BufferReader(messageBody);
  const messageClass = messageClasses.get(messageType);
  if (messageClass === undefined) {
    throw new Error(`Messages: missing message class for type ${messageType}`);
  }

  const pbMessage = messageClass.decode(messageBodyReader);
  return { messageType, pbMessage };
};
