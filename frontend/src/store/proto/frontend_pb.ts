/* eslint-disable */
import Long from "long";
import _m0 from "protobufjs/minimal";

export const protobufPackage = "";

/** TODO: should metric be 0? (i.e. the default) */
export enum Unit {
  imperial = 0,
  metric = 1,
  UNRECOGNIZED = -1,
}

export function unitFromJSON(object: any): Unit {
  switch (object) {
    case 0:
    case "imperial":
      return Unit.imperial;
    case 1:
    case "metric":
      return Unit.metric;
    case -1:
    case "UNRECOGNIZED":
    default:
      return Unit.UNRECOGNIZED;
  }
}

export function unitToJSON(object: Unit): string {
  switch (object) {
    case Unit.imperial:
      return "imperial";
    case Unit.metric:
      return "metric";
    default:
      return "UNKNOWN";
  }
}

export enum ThemeVariant {
  dark = 0,
  light = 1,
  UNRECOGNIZED = -1,
}

export function themeVariantFromJSON(object: any): ThemeVariant {
  switch (object) {
    case 0:
    case "dark":
      return ThemeVariant.dark;
    case 1:
    case "light":
      return ThemeVariant.light;
    case -1:
    case "UNRECOGNIZED":
    default:
      return ThemeVariant.UNRECOGNIZED;
  }
}

export function themeVariantToJSON(object: ThemeVariant): string {
  switch (object) {
    case ThemeVariant.dark:
      return "dark";
    case ThemeVariant.light:
      return "light";
    default:
      return "UNKNOWN";
  }
}

export interface RotaryEncoder {
  step: number;
  lastStepChange: number;
  buttonPressed: boolean;
  lastButtonDown: number;
  lastButtonUp: number;
}

/**
 * TODO: we also need a request version of this message, FrontendDisplaySettingsRequest
 * TODO: rename this to FrontendDisplaySettings (with the "s" at the end)
 */
export interface FrontendDisplaySetting {
  theme: ThemeVariant;
  unit: Unit;
}

export interface SystemSettings {
  /**
   * We use a double and units of sec because otherwise we'd use uint64 and
   * units of ms, but then mcu_pb.LogEvent needs to have oldUint64 and newUint64
   * instead of oldUint32 and newUint32, and the frontend runs into problems
   * because it can only have 53 bits of precision in the number type (in which
   * case the frontend would then need to use BigInt), which prevents the frontend
   * from working correctly on other things which require LogEvent.oldUint32/newUint32.
   */
  date: number;
  /** TODO: move display_brightness into FrontendDisplaySetting */
  displayBrightness: number;
  seqNum: number;
}

export interface SystemSettingsRequest {
  /** Unix timestamp in units of sec */
  date: number;
  displayBrightness: number;
  seqNum: number;
}

const baseRotaryEncoder: object = {
  step: 0,
  lastStepChange: 0,
  buttonPressed: false,
  lastButtonDown: 0,
  lastButtonUp: 0,
};

export const RotaryEncoder = {
  encode(
    message: RotaryEncoder,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.step !== 0) {
      writer.uint32(8).int32(message.step);
    }
    if (message.lastStepChange !== 0) {
      writer.uint32(21).float(message.lastStepChange);
    }
    if (message.buttonPressed === true) {
      writer.uint32(24).bool(message.buttonPressed);
    }
    if (message.lastButtonDown !== 0) {
      writer.uint32(37).float(message.lastButtonDown);
    }
    if (message.lastButtonUp !== 0) {
      writer.uint32(45).float(message.lastButtonUp);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): RotaryEncoder {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseRotaryEncoder } as RotaryEncoder;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.step = reader.int32();
          break;
        case 2:
          message.lastStepChange = reader.float();
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
    const message = { ...baseRotaryEncoder } as RotaryEncoder;
    if (object.step !== undefined && object.step !== null) {
      message.step = Number(object.step);
    } else {
      message.step = 0;
    }
    if (object.lastStepChange !== undefined && object.lastStepChange !== null) {
      message.lastStepChange = Number(object.lastStepChange);
    } else {
      message.lastStepChange = 0;
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

  toJSON(message: RotaryEncoder): unknown {
    const obj: any = {};
    message.step !== undefined && (obj.step = message.step);
    message.lastStepChange !== undefined &&
      (obj.lastStepChange = message.lastStepChange);
    message.buttonPressed !== undefined &&
      (obj.buttonPressed = message.buttonPressed);
    message.lastButtonDown !== undefined &&
      (obj.lastButtonDown = message.lastButtonDown);
    message.lastButtonUp !== undefined &&
      (obj.lastButtonUp = message.lastButtonUp);
    return obj;
  },

  fromPartial(object: DeepPartial<RotaryEncoder>): RotaryEncoder {
    const message = { ...baseRotaryEncoder } as RotaryEncoder;
    if (object.step !== undefined && object.step !== null) {
      message.step = object.step;
    } else {
      message.step = 0;
    }
    if (object.lastStepChange !== undefined && object.lastStepChange !== null) {
      message.lastStepChange = object.lastStepChange;
    } else {
      message.lastStepChange = 0;
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
};

const baseFrontendDisplaySetting: object = { theme: 0, unit: 0 };

export const FrontendDisplaySetting = {
  encode(
    message: FrontendDisplaySetting,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.theme !== 0) {
      writer.uint32(8).int32(message.theme);
    }
    if (message.unit !== 0) {
      writer.uint32(16).int32(message.unit);
    }
    return writer;
  },

  decode(
    input: _m0.Reader | Uint8Array,
    length?: number
  ): FrontendDisplaySetting {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseFrontendDisplaySetting } as FrontendDisplaySetting;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.theme = reader.int32() as any;
          break;
        case 2:
          message.unit = reader.int32() as any;
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): FrontendDisplaySetting {
    const message = { ...baseFrontendDisplaySetting } as FrontendDisplaySetting;
    if (object.theme !== undefined && object.theme !== null) {
      message.theme = themeVariantFromJSON(object.theme);
    } else {
      message.theme = 0;
    }
    if (object.unit !== undefined && object.unit !== null) {
      message.unit = unitFromJSON(object.unit);
    } else {
      message.unit = 0;
    }
    return message;
  },

  toJSON(message: FrontendDisplaySetting): unknown {
    const obj: any = {};
    message.theme !== undefined &&
      (obj.theme = themeVariantToJSON(message.theme));
    message.unit !== undefined && (obj.unit = unitToJSON(message.unit));
    return obj;
  },

  fromPartial(
    object: DeepPartial<FrontendDisplaySetting>
  ): FrontendDisplaySetting {
    const message = { ...baseFrontendDisplaySetting } as FrontendDisplaySetting;
    if (object.theme !== undefined && object.theme !== null) {
      message.theme = object.theme;
    } else {
      message.theme = 0;
    }
    if (object.unit !== undefined && object.unit !== null) {
      message.unit = object.unit;
    } else {
      message.unit = 0;
    }
    return message;
  },
};

const baseSystemSettings: object = { date: 0, displayBrightness: 0, seqNum: 0 };

export const SystemSettings = {
  encode(
    message: SystemSettings,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.date !== 0) {
      writer.uint32(9).double(message.date);
    }
    if (message.displayBrightness !== 0) {
      writer.uint32(16).uint32(message.displayBrightness);
    }
    if (message.seqNum !== 0) {
      writer.uint32(24).uint32(message.seqNum);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): SystemSettings {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseSystemSettings } as SystemSettings;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.date = reader.double();
          break;
        case 2:
          message.displayBrightness = reader.uint32();
          break;
        case 3:
          message.seqNum = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): SystemSettings {
    const message = { ...baseSystemSettings } as SystemSettings;
    if (object.date !== undefined && object.date !== null) {
      message.date = Number(object.date);
    } else {
      message.date = 0;
    }
    if (
      object.displayBrightness !== undefined &&
      object.displayBrightness !== null
    ) {
      message.displayBrightness = Number(object.displayBrightness);
    } else {
      message.displayBrightness = 0;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = Number(object.seqNum);
    } else {
      message.seqNum = 0;
    }
    return message;
  },

  toJSON(message: SystemSettings): unknown {
    const obj: any = {};
    message.date !== undefined && (obj.date = message.date);
    message.displayBrightness !== undefined &&
      (obj.displayBrightness = message.displayBrightness);
    message.seqNum !== undefined && (obj.seqNum = message.seqNum);
    return obj;
  },

  fromPartial(object: DeepPartial<SystemSettings>): SystemSettings {
    const message = { ...baseSystemSettings } as SystemSettings;
    if (object.date !== undefined && object.date !== null) {
      message.date = object.date;
    } else {
      message.date = 0;
    }
    if (
      object.displayBrightness !== undefined &&
      object.displayBrightness !== null
    ) {
      message.displayBrightness = object.displayBrightness;
    } else {
      message.displayBrightness = 0;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = object.seqNum;
    } else {
      message.seqNum = 0;
    }
    return message;
  },
};

const baseSystemSettingsRequest: object = {
  date: 0,
  displayBrightness: 0,
  seqNum: 0,
};

export const SystemSettingsRequest = {
  encode(
    message: SystemSettingsRequest,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.date !== 0) {
      writer.uint32(9).double(message.date);
    }
    if (message.displayBrightness !== 0) {
      writer.uint32(16).uint32(message.displayBrightness);
    }
    if (message.seqNum !== 0) {
      writer.uint32(24).uint32(message.seqNum);
    }
    return writer;
  },

  decode(
    input: _m0.Reader | Uint8Array,
    length?: number
  ): SystemSettingsRequest {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseSystemSettingsRequest } as SystemSettingsRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.date = reader.double();
          break;
        case 2:
          message.displayBrightness = reader.uint32();
          break;
        case 3:
          message.seqNum = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): SystemSettingsRequest {
    const message = { ...baseSystemSettingsRequest } as SystemSettingsRequest;
    if (object.date !== undefined && object.date !== null) {
      message.date = Number(object.date);
    } else {
      message.date = 0;
    }
    if (
      object.displayBrightness !== undefined &&
      object.displayBrightness !== null
    ) {
      message.displayBrightness = Number(object.displayBrightness);
    } else {
      message.displayBrightness = 0;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = Number(object.seqNum);
    } else {
      message.seqNum = 0;
    }
    return message;
  },

  toJSON(message: SystemSettingsRequest): unknown {
    const obj: any = {};
    message.date !== undefined && (obj.date = message.date);
    message.displayBrightness !== undefined &&
      (obj.displayBrightness = message.displayBrightness);
    message.seqNum !== undefined && (obj.seqNum = message.seqNum);
    return obj;
  },

  fromPartial(
    object: DeepPartial<SystemSettingsRequest>
  ): SystemSettingsRequest {
    const message = { ...baseSystemSettingsRequest } as SystemSettingsRequest;
    if (object.date !== undefined && object.date !== null) {
      message.date = object.date;
    } else {
      message.date = 0;
    }
    if (
      object.displayBrightness !== undefined &&
      object.displayBrightness !== null
    ) {
      message.displayBrightness = object.displayBrightness;
    } else {
      message.displayBrightness = 0;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = object.seqNum;
    } else {
      message.seqNum = 0;
    }
    return message;
  },
};

type Builtin = Date | Function | Uint8Array | string | number | undefined;
export type DeepPartial<T> = T extends Builtin
  ? T
  : T extends Array<infer U>
  ? Array<DeepPartial<U>>
  : T extends ReadonlyArray<infer U>
  ? ReadonlyArray<DeepPartial<U>>
  : T extends {}
  ? { [K in keyof T]?: DeepPartial<T[K]> }
  : Partial<T>;

if (_m0.util.Long !== Long) {
  _m0.util.Long = Long as any;
  _m0.configure();
}
