/* eslint-disable */
import { Writer, Reader } from 'protobufjs/minimal';


export interface RotaryEncoder {
  angle: number;
  lastAngleChange: number;
  buttonPressed: boolean;
  lastButtonDown: number;
  lastButtonUp: number;
}

const baseRotaryEncoder: object = {
  angle: 0,
  lastAngleChange: 0,
  buttonPressed: false,
  lastButtonDown: 0,
  lastButtonUp: 0,
};

export const RotaryEncoder = {
  encode(message: RotaryEncoder, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.angle);
    writer.uint32(21).float(message.lastAngleChange);
    writer.uint32(24).bool(message.buttonPressed);
    writer.uint32(37).float(message.lastButtonDown);
    writer.uint32(45).float(message.lastButtonUp);
    return writer;
  },
  decode(reader: Reader, length?: number): RotaryEncoder {
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseRotaryEncoder) as RotaryEncoder;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.angle = reader.uint32();
          break;
        case 2:
          message.lastAngleChange = reader.float();
          break;
        case 3:
          message.buttonPressed = reader.bool();
          break;
        case 4:
          message.lastButtonDown = reader.float();
          break;
        case 5:
          message.lastButtonUp = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): RotaryEncoder {
    const message = Object.create(baseRotaryEncoder) as RotaryEncoder;
    if (object.angle !== undefined && object.angle !== null) {
      message.angle = Number(object.angle);
    } else {
      message.angle = 0;
    }
    if (object.lastAngleChange !== undefined && object.lastAngleChange !== null) {
      message.lastAngleChange = Number(object.lastAngleChange);
    } else {
      message.lastAngleChange = 0;
    }
    if (object.buttonPressed !== undefined && object.buttonPressed !== null) {
      message.buttonPressed = Boolean(object.buttonPressed);
    } else {
      message.buttonPressed = false;
    }
    if (object.lastButtonDown !== undefined && object.lastButtonDown !== null) {
      message.lastButtonDown = Number(object.lastButtonDown);
    } else {
      message.lastButtonDown = 0;
    }
    if (object.lastButtonUp !== undefined && object.lastButtonUp !== null) {
      message.lastButtonUp = Number(object.lastButtonUp);
    } else {
      message.lastButtonUp = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<RotaryEncoder>): RotaryEncoder {
    const message = Object.create(baseRotaryEncoder) as RotaryEncoder;
    if (object.angle !== undefined && object.angle !== null) {
      message.angle = object.angle;
    } else {
      message.angle = 0;
    }
    if (object.lastAngleChange !== undefined && object.lastAngleChange !== null) {
      message.lastAngleChange = object.lastAngleChange;
    } else {
      message.lastAngleChange = 0;
    }
    if (object.buttonPressed !== undefined && object.buttonPressed !== null) {
      message.buttonPressed = object.buttonPressed;
    } else {
      message.buttonPressed = false;
    }
    if (object.lastButtonDown !== undefined && object.lastButtonDown !== null) {
      message.lastButtonDown = object.lastButtonDown;
    } else {
      message.lastButtonDown = 0;
    }
    if (object.lastButtonUp !== undefined && object.lastButtonUp !== null) {
      message.lastButtonUp = object.lastButtonUp;
    } else {
      message.lastButtonUp = 0;
    }
    return message;
  },
  toJSON(message: RotaryEncoder): unknown {
    const obj: any = {};
    obj.angle = message.angle || 0;
    obj.lastAngleChange = message.lastAngleChange || 0;
    obj.buttonPressed = message.buttonPressed || false;
    obj.lastButtonDown = message.lastButtonDown || 0;
    obj.lastButtonUp = message.lastButtonUp || 0;
    return obj;
  },
};

type Builtin = Date | Function | Uint8Array | string | number | undefined;
type DeepPartial<T> = T extends Builtin
  ? T
  : T extends Array<infer U>
  ? Array<DeepPartial<U>>
  : T extends ReadonlyArray<infer U>
  ? ReadonlyArray<DeepPartial<U>>
  : T extends {}
  ? { [K in keyof T]?: DeepPartial<T[K]> }
  : Partial<T>;