/* eslint-disable */
import { Writer, Reader } from 'protobufjs/minimal';


export interface Alarms {
  time: number;
  alarmOne: boolean;
  alarmTwo: boolean;
}

export interface AlarmLimitsRequest {
  rrMin: number;
  rrMax: number;
  pipMin: number;
  pipMax: number;
  peepMin: number;
  peepMax: number;
  ipAbovePeepMin: number;
  ipAbovePeepMax: number;
  inspTimeMin: number;
  inspTimeMax: number;
  fio2Min: number;
  fio2Max: number;
  pawMin: number;
  pawMax: number;
  mveMin: number;
  mveMax: number;
  tvMin: number;
  tvMax: number;
  etco2Min: number;
  etco2Max: number;
  flowMin: number;
  flowMax: number;
  apneaMin: number;
  apneaMax: number;
  spo2Min: number;
  spo2Max: number;
}

export interface SensorMeasurements {
  time: number;
  cycle: number;
  paw: number;
  flow: number;
  volume: number;
  fio2: number;
  spo2: number;
}

export interface CycleMeasurements {
  time: number;
  vt: number;
  rr: number;
  peep: number;
  pip: number;
  ip: number;
  ve: number;
}

export interface Parameters {
  time: number;
  mode: VentilationMode;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
  fio2: number;
  flow: number;
}

export interface ParametersRequest {
  time: number;
  mode: VentilationMode;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
  fio2: number;
  flow: number;
}

export interface Ping {
  time: number;
  id: number;
}

export interface Announcement {
  time: number;
  announcement: Uint8Array;
}

export interface LogEvent {
  id: number;
  time: number;
  code: LogEventCode;
  oldValue: number;
  newValue: number;
}

export interface ExpectedLogEvent {
  id: number;
}

export interface NextLogEvents {
  nextExpected: number;
  total: number;
  remaining: number;
  elements: LogEvent[];
}

export interface ActiveLogEvents {
  id: number[];
}

const baseAlarms: object = {
  time: 0,
  alarmOne: false,
  alarmTwo: false,
};

const baseAlarmLimitsRequest: object = {
  rrMin: 0,
  rrMax: 0,
  pipMin: 0,
  pipMax: 0,
  peepMin: 0,
  peepMax: 0,
  ipAbovePeepMin: 0,
  ipAbovePeepMax: 0,
  inspTimeMin: 0,
  inspTimeMax: 0,
  fio2Min: 0,
  fio2Max: 0,
  pawMin: 0,
  pawMax: 0,
  mveMin: 0,
  mveMax: 0,
  tvMin: 0,
  tvMax: 0,
  etco2Min: 0,
  etco2Max: 0,
  flowMin: 0,
  flowMax: 0,
  apneaMin: 0,
  apneaMax: 0,
  spo2Min: 0,
  spo2Max: 0,
};

const baseSensorMeasurements: object = {
  time: 0,
  cycle: 0,
  paw: 0,
  flow: 0,
  volume: 0,
  fio2: 0,
  spo2: 0,
};

const baseCycleMeasurements: object = {
  time: 0,
  vt: 0,
  rr: 0,
  peep: 0,
  pip: 0,
  ip: 0,
  ve: 0,
};

const baseParameters: object = {
  time: 0,
  mode: 0,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
  fio2: 0,
  flow: 0,
};

const baseParametersRequest: object = {
  time: 0,
  mode: 0,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
  fio2: 0,
  flow: 0,
};

const basePing: object = {
  time: 0,
  id: 0,
};

const baseAnnouncement: object = {
  time: 0,
  announcement: undefined,
};

const baseLogEvent: object = {
  id: 0,
  time: 0,
  code: 0,
  oldValue: 0,
  newValue: 0,
};

const baseExpectedLogEvent: object = {
  id: 0,
};

const baseNextLogEvents: object = {
  nextExpected: 0,
  total: 0,
  remaining: 0,
  elements: undefined,
};

const baseActiveLogEvents: object = {
  id: 0,
};

export const VentilationMode = {
  pc_ac: 0 as const,
  pc_simv: 1 as const,
  vc_ac: 2 as const,
  vc_simv: 3 as const,
  psv: 4 as const,
  niv: 5 as const,
  hfnc: 6 as const,
  UNRECOGNIZED: -1 as const,
  fromJSON(object: any): VentilationMode {
    switch (object) {
      case 0:
      case "pc_ac":
        return VentilationMode.pc_ac;
      case 1:
      case "pc_simv":
        return VentilationMode.pc_simv;
      case 2:
      case "vc_ac":
        return VentilationMode.vc_ac;
      case 3:
      case "vc_simv":
        return VentilationMode.vc_simv;
      case 4:
      case "psv":
        return VentilationMode.psv;
      case 5:
      case "niv":
        return VentilationMode.niv;
      case 6:
      case "hfnc":
        return VentilationMode.hfnc;
      case -1:
      case "UNRECOGNIZED":
      default:
        return VentilationMode.UNRECOGNIZED;
    }
  },
  toJSON(object: VentilationMode): string {
    switch (object) {
      case VentilationMode.pc_ac:
        return "pc_ac";
      case VentilationMode.pc_simv:
        return "pc_simv";
      case VentilationMode.vc_ac:
        return "vc_ac";
      case VentilationMode.vc_simv:
        return "vc_simv";
      case VentilationMode.psv:
        return "psv";
      case VentilationMode.niv:
        return "niv";
      case VentilationMode.hfnc:
        return "hfnc";
      default:
        return "UNKNOWN";
    }
  },
}

export type VentilationMode = 0 | 1 | 2 | 3 | 4 | 5 | 6 | -1;

/**  Logged Events
 */
export const LogEventCode = {
  fio2_too_low: 0 as const,
  fio2_too_high: 1 as const,
  spo2_too_low: 2 as const,
  spo2_too_high: 3 as const,
  rr_too_low: 4 as const,
  rr_too_high: 5 as const,
  battery_low: 6 as const,
  screen_locked: 7 as const,
  UNRECOGNIZED: -1 as const,
  fromJSON(object: any): LogEventCode {
    switch (object) {
      case 0:
      case "fio2_too_low":
        return LogEventCode.fio2_too_low;
      case 1:
      case "fio2_too_high":
        return LogEventCode.fio2_too_high;
      case 2:
      case "spo2_too_low":
        return LogEventCode.spo2_too_low;
      case 3:
      case "spo2_too_high":
        return LogEventCode.spo2_too_high;
      case 4:
      case "rr_too_low":
        return LogEventCode.rr_too_low;
      case 5:
      case "rr_too_high":
        return LogEventCode.rr_too_high;
      case 6:
      case "battery_low":
        return LogEventCode.battery_low;
      case 7:
      case "screen_locked":
        return LogEventCode.screen_locked;
      case -1:
      case "UNRECOGNIZED":
      default:
        return LogEventCode.UNRECOGNIZED;
    }
  },
  toJSON(object: LogEventCode): string {
    switch (object) {
      case LogEventCode.fio2_too_low:
        return "fio2_too_low";
      case LogEventCode.fio2_too_high:
        return "fio2_too_high";
      case LogEventCode.spo2_too_low:
        return "spo2_too_low";
      case LogEventCode.spo2_too_high:
        return "spo2_too_high";
      case LogEventCode.rr_too_low:
        return "rr_too_low";
      case LogEventCode.rr_too_high:
        return "rr_too_high";
      case LogEventCode.battery_low:
        return "battery_low";
      case LogEventCode.screen_locked:
        return "screen_locked";
      default:
        return "UNKNOWN";
    }
  },
}

export type LogEventCode = 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | -1;

export const Alarms = {
  encode(message: Alarms, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).bool(message.alarmOne);
    writer.uint32(24).bool(message.alarmTwo);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): Alarms {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseAlarms) as Alarms;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.alarmOne = reader.bool();
          break;
        case 3:
          message.alarmTwo = reader.bool();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): Alarms {
    const message = Object.create(baseAlarms) as Alarms;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.alarmOne !== undefined && object.alarmOne !== null) {
      message.alarmOne = Boolean(object.alarmOne);
    } else {
      message.alarmOne = false;
    }
    if (object.alarmTwo !== undefined && object.alarmTwo !== null) {
      message.alarmTwo = Boolean(object.alarmTwo);
    } else {
      message.alarmTwo = false;
    }
    return message;
  },
  fromPartial(object: DeepPartial<Alarms>): Alarms {
    const message = Object.create(baseAlarms) as Alarms;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.alarmOne !== undefined && object.alarmOne !== null) {
      message.alarmOne = object.alarmOne;
    } else {
      message.alarmOne = false;
    }
    if (object.alarmTwo !== undefined && object.alarmTwo !== null) {
      message.alarmTwo = object.alarmTwo;
    } else {
      message.alarmTwo = false;
    }
    return message;
  },
  toJSON(message: Alarms): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.alarmOne = message.alarmOne || false;
    obj.alarmTwo = message.alarmTwo || false;
    return obj;
  },
};

export const AlarmLimitsRequest = {
  encode(message: AlarmLimitsRequest, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.rrMin);
    writer.uint32(16).uint32(message.rrMax);
    writer.uint32(24).uint32(message.pipMin);
    writer.uint32(32).uint32(message.pipMax);
    writer.uint32(40).uint32(message.peepMin);
    writer.uint32(48).uint32(message.peepMax);
    writer.uint32(56).uint32(message.ipAbovePeepMin);
    writer.uint32(64).uint32(message.ipAbovePeepMax);
    writer.uint32(72).uint32(message.inspTimeMin);
    writer.uint32(80).uint32(message.inspTimeMax);
    writer.uint32(88).uint32(message.fio2Min);
    writer.uint32(96).uint32(message.fio2Max);
    writer.uint32(104).uint32(message.pawMin);
    writer.uint32(112).uint32(message.pawMax);
    writer.uint32(120).uint32(message.mveMin);
    writer.uint32(128).uint32(message.mveMax);
    writer.uint32(136).uint32(message.tvMin);
    writer.uint32(144).uint32(message.tvMax);
    writer.uint32(152).uint32(message.etco2Min);
    writer.uint32(160).uint32(message.etco2Max);
    writer.uint32(168).uint32(message.flowMin);
    writer.uint32(176).uint32(message.flowMax);
    writer.uint32(184).uint32(message.apneaMin);
    writer.uint32(192).uint32(message.apneaMax);
    writer.uint32(200).uint32(message.spo2Min);
    writer.uint32(208).uint32(message.spo2Max);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): AlarmLimitsRequest {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseAlarmLimitsRequest) as AlarmLimitsRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.rrMin = reader.uint32();
          break;
        case 2:
          message.rrMax = reader.uint32();
          break;
        case 3:
          message.pipMin = reader.uint32();
          break;
        case 4:
          message.pipMax = reader.uint32();
          break;
        case 5:
          message.peepMin = reader.uint32();
          break;
        case 6:
          message.peepMax = reader.uint32();
          break;
        case 7:
          message.ipAbovePeepMin = reader.uint32();
          break;
        case 8:
          message.ipAbovePeepMax = reader.uint32();
          break;
        case 9:
          message.inspTimeMin = reader.uint32();
          break;
        case 10:
          message.inspTimeMax = reader.uint32();
          break;
        case 11:
          message.fio2Min = reader.uint32();
          break;
        case 12:
          message.fio2Max = reader.uint32();
          break;
        case 13:
          message.pawMin = reader.uint32();
          break;
        case 14:
          message.pawMax = reader.uint32();
          break;
        case 15:
          message.mveMin = reader.uint32();
          break;
        case 16:
          message.mveMax = reader.uint32();
          break;
        case 17:
          message.tvMin = reader.uint32();
          break;
        case 18:
          message.tvMax = reader.uint32();
          break;
        case 19:
          message.etco2Min = reader.uint32();
          break;
        case 20:
          message.etco2Max = reader.uint32();
          break;
        case 21:
          message.flowMin = reader.uint32();
          break;
        case 22:
          message.flowMax = reader.uint32();
          break;
        case 23:
          message.apneaMin = reader.uint32();
          break;
        case 24:
          message.apneaMax = reader.uint32();
          break;
        case 25:
          message.spo2Min = reader.uint32();
          break;
        case 26:
          message.spo2Max = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): AlarmLimitsRequest {
    const message = Object.create(baseAlarmLimitsRequest) as AlarmLimitsRequest;
    if (object.rrMin !== undefined && object.rrMin !== null) {
      message.rrMin = Number(object.rrMin);
    } else {
      message.rrMin = 0;
    }
    if (object.rrMax !== undefined && object.rrMax !== null) {
      message.rrMax = Number(object.rrMax);
    } else {
      message.rrMax = 0;
    }
    if (object.pipMin !== undefined && object.pipMin !== null) {
      message.pipMin = Number(object.pipMin);
    } else {
      message.pipMin = 0;
    }
    if (object.pipMax !== undefined && object.pipMax !== null) {
      message.pipMax = Number(object.pipMax);
    } else {
      message.pipMax = 0;
    }
    if (object.peepMin !== undefined && object.peepMin !== null) {
      message.peepMin = Number(object.peepMin);
    } else {
      message.peepMin = 0;
    }
    if (object.peepMax !== undefined && object.peepMax !== null) {
      message.peepMax = Number(object.peepMax);
    } else {
      message.peepMax = 0;
    }
    if (object.ipAbovePeepMin !== undefined && object.ipAbovePeepMin !== null) {
      message.ipAbovePeepMin = Number(object.ipAbovePeepMin);
    } else {
      message.ipAbovePeepMin = 0;
    }
    if (object.ipAbovePeepMax !== undefined && object.ipAbovePeepMax !== null) {
      message.ipAbovePeepMax = Number(object.ipAbovePeepMax);
    } else {
      message.ipAbovePeepMax = 0;
    }
    if (object.inspTimeMin !== undefined && object.inspTimeMin !== null) {
      message.inspTimeMin = Number(object.inspTimeMin);
    } else {
      message.inspTimeMin = 0;
    }
    if (object.inspTimeMax !== undefined && object.inspTimeMax !== null) {
      message.inspTimeMax = Number(object.inspTimeMax);
    } else {
      message.inspTimeMax = 0;
    }
    if (object.fio2Min !== undefined && object.fio2Min !== null) {
      message.fio2Min = Number(object.fio2Min);
    } else {
      message.fio2Min = 0;
    }
    if (object.fio2Max !== undefined && object.fio2Max !== null) {
      message.fio2Max = Number(object.fio2Max);
    } else {
      message.fio2Max = 0;
    }
    if (object.pawMin !== undefined && object.pawMin !== null) {
      message.pawMin = Number(object.pawMin);
    } else {
      message.pawMin = 0;
    }
    if (object.pawMax !== undefined && object.pawMax !== null) {
      message.pawMax = Number(object.pawMax);
    } else {
      message.pawMax = 0;
    }
    if (object.mveMin !== undefined && object.mveMin !== null) {
      message.mveMin = Number(object.mveMin);
    } else {
      message.mveMin = 0;
    }
    if (object.mveMax !== undefined && object.mveMax !== null) {
      message.mveMax = Number(object.mveMax);
    } else {
      message.mveMax = 0;
    }
    if (object.tvMin !== undefined && object.tvMin !== null) {
      message.tvMin = Number(object.tvMin);
    } else {
      message.tvMin = 0;
    }
    if (object.tvMax !== undefined && object.tvMax !== null) {
      message.tvMax = Number(object.tvMax);
    } else {
      message.tvMax = 0;
    }
    if (object.etco2Min !== undefined && object.etco2Min !== null) {
      message.etco2Min = Number(object.etco2Min);
    } else {
      message.etco2Min = 0;
    }
    if (object.etco2Max !== undefined && object.etco2Max !== null) {
      message.etco2Max = Number(object.etco2Max);
    } else {
      message.etco2Max = 0;
    }
    if (object.flowMin !== undefined && object.flowMin !== null) {
      message.flowMin = Number(object.flowMin);
    } else {
      message.flowMin = 0;
    }
    if (object.flowMax !== undefined && object.flowMax !== null) {
      message.flowMax = Number(object.flowMax);
    } else {
      message.flowMax = 0;
    }
    if (object.apneaMin !== undefined && object.apneaMin !== null) {
      message.apneaMin = Number(object.apneaMin);
    } else {
      message.apneaMin = 0;
    }
    if (object.apneaMax !== undefined && object.apneaMax !== null) {
      message.apneaMax = Number(object.apneaMax);
    } else {
      message.apneaMax = 0;
    }
    if (object.spo2Min !== undefined && object.spo2Min !== null) {
      message.spo2Min = Number(object.spo2Min);
    } else {
      message.spo2Min = 0;
    }
    if (object.spo2Max !== undefined && object.spo2Max !== null) {
      message.spo2Max = Number(object.spo2Max);
    } else {
      message.spo2Max = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<AlarmLimitsRequest>): AlarmLimitsRequest {
    const message = Object.create(baseAlarmLimitsRequest) as AlarmLimitsRequest;
    if (object.rrMin !== undefined && object.rrMin !== null) {
      message.rrMin = object.rrMin;
    } else {
      message.rrMin = 0;
    }
    if (object.rrMax !== undefined && object.rrMax !== null) {
      message.rrMax = object.rrMax;
    } else {
      message.rrMax = 0;
    }
    if (object.pipMin !== undefined && object.pipMin !== null) {
      message.pipMin = object.pipMin;
    } else {
      message.pipMin = 0;
    }
    if (object.pipMax !== undefined && object.pipMax !== null) {
      message.pipMax = object.pipMax;
    } else {
      message.pipMax = 0;
    }
    if (object.peepMin !== undefined && object.peepMin !== null) {
      message.peepMin = object.peepMin;
    } else {
      message.peepMin = 0;
    }
    if (object.peepMax !== undefined && object.peepMax !== null) {
      message.peepMax = object.peepMax;
    } else {
      message.peepMax = 0;
    }
    if (object.ipAbovePeepMin !== undefined && object.ipAbovePeepMin !== null) {
      message.ipAbovePeepMin = object.ipAbovePeepMin;
    } else {
      message.ipAbovePeepMin = 0;
    }
    if (object.ipAbovePeepMax !== undefined && object.ipAbovePeepMax !== null) {
      message.ipAbovePeepMax = object.ipAbovePeepMax;
    } else {
      message.ipAbovePeepMax = 0;
    }
    if (object.inspTimeMin !== undefined && object.inspTimeMin !== null) {
      message.inspTimeMin = object.inspTimeMin;
    } else {
      message.inspTimeMin = 0;
    }
    if (object.inspTimeMax !== undefined && object.inspTimeMax !== null) {
      message.inspTimeMax = object.inspTimeMax;
    } else {
      message.inspTimeMax = 0;
    }
    if (object.fio2Min !== undefined && object.fio2Min !== null) {
      message.fio2Min = object.fio2Min;
    } else {
      message.fio2Min = 0;
    }
    if (object.fio2Max !== undefined && object.fio2Max !== null) {
      message.fio2Max = object.fio2Max;
    } else {
      message.fio2Max = 0;
    }
    if (object.pawMin !== undefined && object.pawMin !== null) {
      message.pawMin = object.pawMin;
    } else {
      message.pawMin = 0;
    }
    if (object.pawMax !== undefined && object.pawMax !== null) {
      message.pawMax = object.pawMax;
    } else {
      message.pawMax = 0;
    }
    if (object.mveMin !== undefined && object.mveMin !== null) {
      message.mveMin = object.mveMin;
    } else {
      message.mveMin = 0;
    }
    if (object.mveMax !== undefined && object.mveMax !== null) {
      message.mveMax = object.mveMax;
    } else {
      message.mveMax = 0;
    }
    if (object.tvMin !== undefined && object.tvMin !== null) {
      message.tvMin = object.tvMin;
    } else {
      message.tvMin = 0;
    }
    if (object.tvMax !== undefined && object.tvMax !== null) {
      message.tvMax = object.tvMax;
    } else {
      message.tvMax = 0;
    }
    if (object.etco2Min !== undefined && object.etco2Min !== null) {
      message.etco2Min = object.etco2Min;
    } else {
      message.etco2Min = 0;
    }
    if (object.etco2Max !== undefined && object.etco2Max !== null) {
      message.etco2Max = object.etco2Max;
    } else {
      message.etco2Max = 0;
    }
    if (object.flowMin !== undefined && object.flowMin !== null) {
      message.flowMin = object.flowMin;
    } else {
      message.flowMin = 0;
    }
    if (object.flowMax !== undefined && object.flowMax !== null) {
      message.flowMax = object.flowMax;
    } else {
      message.flowMax = 0;
    }
    if (object.apneaMin !== undefined && object.apneaMin !== null) {
      message.apneaMin = object.apneaMin;
    } else {
      message.apneaMin = 0;
    }
    if (object.apneaMax !== undefined && object.apneaMax !== null) {
      message.apneaMax = object.apneaMax;
    } else {
      message.apneaMax = 0;
    }
    if (object.spo2Min !== undefined && object.spo2Min !== null) {
      message.spo2Min = object.spo2Min;
    } else {
      message.spo2Min = 0;
    }
    if (object.spo2Max !== undefined && object.spo2Max !== null) {
      message.spo2Max = object.spo2Max;
    } else {
      message.spo2Max = 0;
    }
    return message;
  },
  toJSON(message: AlarmLimitsRequest): unknown {
    const obj: any = {};
    obj.rrMin = message.rrMin || 0;
    obj.rrMax = message.rrMax || 0;
    obj.pipMin = message.pipMin || 0;
    obj.pipMax = message.pipMax || 0;
    obj.peepMin = message.peepMin || 0;
    obj.peepMax = message.peepMax || 0;
    obj.ipAbovePeepMin = message.ipAbovePeepMin || 0;
    obj.ipAbovePeepMax = message.ipAbovePeepMax || 0;
    obj.inspTimeMin = message.inspTimeMin || 0;
    obj.inspTimeMax = message.inspTimeMax || 0;
    obj.fio2Min = message.fio2Min || 0;
    obj.fio2Max = message.fio2Max || 0;
    obj.pawMin = message.pawMin || 0;
    obj.pawMax = message.pawMax || 0;
    obj.mveMin = message.mveMin || 0;
    obj.mveMax = message.mveMax || 0;
    obj.tvMin = message.tvMin || 0;
    obj.tvMax = message.tvMax || 0;
    obj.etco2Min = message.etco2Min || 0;
    obj.etco2Max = message.etco2Max || 0;
    obj.flowMin = message.flowMin || 0;
    obj.flowMax = message.flowMax || 0;
    obj.apneaMin = message.apneaMin || 0;
    obj.apneaMax = message.apneaMax || 0;
    obj.spo2Min = message.spo2Min || 0;
    obj.spo2Max = message.spo2Max || 0;
    return obj;
  },
};

export const SensorMeasurements = {
  encode(message: SensorMeasurements, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).uint32(message.cycle);
    writer.uint32(29).float(message.paw);
    writer.uint32(37).float(message.flow);
    writer.uint32(45).float(message.volume);
    writer.uint32(53).float(message.fio2);
    writer.uint32(61).float(message.spo2);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): SensorMeasurements {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseSensorMeasurements) as SensorMeasurements;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.cycle = reader.uint32();
          break;
        case 3:
          message.paw = reader.float();
          break;
        case 4:
          message.flow = reader.float();
          break;
        case 5:
          message.volume = reader.float();
          break;
        case 6:
          message.fio2 = reader.float();
          break;
        case 7:
          message.spo2 = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): SensorMeasurements {
    const message = Object.create(baseSensorMeasurements) as SensorMeasurements;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.cycle !== undefined && object.cycle !== null) {
      message.cycle = Number(object.cycle);
    } else {
      message.cycle = 0;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Number(object.paw);
    } else {
      message.paw = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Number(object.flow);
    } else {
      message.flow = 0;
    }
    if (object.volume !== undefined && object.volume !== null) {
      message.volume = Number(object.volume);
    } else {
      message.volume = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Number(object.fio2);
    } else {
      message.fio2 = 0;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Number(object.spo2);
    } else {
      message.spo2 = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<SensorMeasurements>): SensorMeasurements {
    const message = Object.create(baseSensorMeasurements) as SensorMeasurements;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.cycle !== undefined && object.cycle !== null) {
      message.cycle = object.cycle;
    } else {
      message.cycle = 0;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = object.paw;
    } else {
      message.paw = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = object.flow;
    } else {
      message.flow = 0;
    }
    if (object.volume !== undefined && object.volume !== null) {
      message.volume = object.volume;
    } else {
      message.volume = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = object.fio2;
    } else {
      message.fio2 = 0;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = object.spo2;
    } else {
      message.spo2 = 0;
    }
    return message;
  },
  toJSON(message: SensorMeasurements): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.cycle = message.cycle || 0;
    obj.paw = message.paw || 0;
    obj.flow = message.flow || 0;
    obj.volume = message.volume || 0;
    obj.fio2 = message.fio2 || 0;
    obj.spo2 = message.spo2 || 0;
    return obj;
  },
};

export const CycleMeasurements = {
  encode(message: CycleMeasurements, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(21).float(message.vt);
    writer.uint32(29).float(message.rr);
    writer.uint32(37).float(message.peep);
    writer.uint32(45).float(message.pip);
    writer.uint32(53).float(message.ip);
    writer.uint32(61).float(message.ve);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): CycleMeasurements {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseCycleMeasurements) as CycleMeasurements;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.vt = reader.float();
          break;
        case 3:
          message.rr = reader.float();
          break;
        case 4:
          message.peep = reader.float();
          break;
        case 5:
          message.pip = reader.float();
          break;
        case 6:
          message.ip = reader.float();
          break;
        case 7:
          message.ve = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): CycleMeasurements {
    const message = Object.create(baseCycleMeasurements) as CycleMeasurements;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = Number(object.vt);
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Number(object.rr);
    } else {
      message.rr = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Number(object.peep);
    } else {
      message.peep = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Number(object.pip);
    } else {
      message.pip = 0;
    }
    if (object.ip !== undefined && object.ip !== null) {
      message.ip = Number(object.ip);
    } else {
      message.ip = 0;
    }
    if (object.ve !== undefined && object.ve !== null) {
      message.ve = Number(object.ve);
    } else {
      message.ve = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<CycleMeasurements>): CycleMeasurements {
    const message = Object.create(baseCycleMeasurements) as CycleMeasurements;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = object.vt;
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = object.rr;
    } else {
      message.rr = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = object.peep;
    } else {
      message.peep = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = object.pip;
    } else {
      message.pip = 0;
    }
    if (object.ip !== undefined && object.ip !== null) {
      message.ip = object.ip;
    } else {
      message.ip = 0;
    }
    if (object.ve !== undefined && object.ve !== null) {
      message.ve = object.ve;
    } else {
      message.ve = 0;
    }
    return message;
  },
  toJSON(message: CycleMeasurements): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.vt = message.vt || 0;
    obj.rr = message.rr || 0;
    obj.peep = message.peep || 0;
    obj.pip = message.pip || 0;
    obj.ip = message.ip || 0;
    obj.ve = message.ve || 0;
    return obj;
  },
};

export const Parameters = {
  encode(message: Parameters, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).int32(message.mode);
    writer.uint32(29).float(message.pip);
    writer.uint32(37).float(message.peep);
    writer.uint32(45).float(message.vt);
    writer.uint32(53).float(message.rr);
    writer.uint32(61).float(message.ie);
    writer.uint32(69).float(message.fio2);
    writer.uint32(77).float(message.flow);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): Parameters {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseParameters) as Parameters;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.mode = reader.int32() as any;
          break;
        case 3:
          message.pip = reader.float();
          break;
        case 4:
          message.peep = reader.float();
          break;
        case 5:
          message.vt = reader.float();
          break;
        case 6:
          message.rr = reader.float();
          break;
        case 7:
          message.ie = reader.float();
          break;
        case 8:
          message.fio2 = reader.float();
          break;
        case 9:
          message.flow = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): Parameters {
    const message = Object.create(baseParameters) as Parameters;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = VentilationMode.fromJSON(object.mode);
    } else {
      message.mode = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Number(object.pip);
    } else {
      message.pip = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Number(object.peep);
    } else {
      message.peep = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = Number(object.vt);
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Number(object.rr);
    } else {
      message.rr = 0;
    }
    if (object.ie !== undefined && object.ie !== null) {
      message.ie = Number(object.ie);
    } else {
      message.ie = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Number(object.fio2);
    } else {
      message.fio2 = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Number(object.flow);
    } else {
      message.flow = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<Parameters>): Parameters {
    const message = Object.create(baseParameters) as Parameters;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = object.mode;
    } else {
      message.mode = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = object.pip;
    } else {
      message.pip = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = object.peep;
    } else {
      message.peep = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = object.vt;
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = object.rr;
    } else {
      message.rr = 0;
    }
    if (object.ie !== undefined && object.ie !== null) {
      message.ie = object.ie;
    } else {
      message.ie = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = object.fio2;
    } else {
      message.fio2 = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = object.flow;
    } else {
      message.flow = 0;
    }
    return message;
  },
  toJSON(message: Parameters): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.mode = VentilationMode.toJSON(message.mode);
    obj.pip = message.pip || 0;
    obj.peep = message.peep || 0;
    obj.vt = message.vt || 0;
    obj.rr = message.rr || 0;
    obj.ie = message.ie || 0;
    obj.fio2 = message.fio2 || 0;
    obj.flow = message.flow || 0;
    return obj;
  },
};

export const ParametersRequest = {
  encode(message: ParametersRequest, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).int32(message.mode);
    writer.uint32(29).float(message.pip);
    writer.uint32(37).float(message.peep);
    writer.uint32(45).float(message.vt);
    writer.uint32(53).float(message.rr);
    writer.uint32(61).float(message.ie);
    writer.uint32(69).float(message.fio2);
    writer.uint32(77).float(message.flow);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): ParametersRequest {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseParametersRequest) as ParametersRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.mode = reader.int32() as any;
          break;
        case 3:
          message.pip = reader.float();
          break;
        case 4:
          message.peep = reader.float();
          break;
        case 5:
          message.vt = reader.float();
          break;
        case 6:
          message.rr = reader.float();
          break;
        case 7:
          message.ie = reader.float();
          break;
        case 8:
          message.fio2 = reader.float();
          break;
        case 9:
          message.flow = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): ParametersRequest {
    const message = Object.create(baseParametersRequest) as ParametersRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = VentilationMode.fromJSON(object.mode);
    } else {
      message.mode = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Number(object.pip);
    } else {
      message.pip = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Number(object.peep);
    } else {
      message.peep = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = Number(object.vt);
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Number(object.rr);
    } else {
      message.rr = 0;
    }
    if (object.ie !== undefined && object.ie !== null) {
      message.ie = Number(object.ie);
    } else {
      message.ie = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Number(object.fio2);
    } else {
      message.fio2 = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Number(object.flow);
    } else {
      message.flow = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<ParametersRequest>): ParametersRequest {
    const message = Object.create(baseParametersRequest) as ParametersRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = object.mode;
    } else {
      message.mode = 0;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = object.pip;
    } else {
      message.pip = 0;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = object.peep;
    } else {
      message.peep = 0;
    }
    if (object.vt !== undefined && object.vt !== null) {
      message.vt = object.vt;
    } else {
      message.vt = 0;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = object.rr;
    } else {
      message.rr = 0;
    }
    if (object.ie !== undefined && object.ie !== null) {
      message.ie = object.ie;
    } else {
      message.ie = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = object.fio2;
    } else {
      message.fio2 = 0;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = object.flow;
    } else {
      message.flow = 0;
    }
    return message;
  },
  toJSON(message: ParametersRequest): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.mode = VentilationMode.toJSON(message.mode);
    obj.pip = message.pip || 0;
    obj.peep = message.peep || 0;
    obj.vt = message.vt || 0;
    obj.rr = message.rr || 0;
    obj.ie = message.ie || 0;
    obj.fio2 = message.fio2 || 0;
    obj.flow = message.flow || 0;
    return obj;
  },
};

export const Ping = {
  encode(message: Ping, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).uint32(message.id);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): Ping {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(basePing) as Ping;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.id = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): Ping {
    const message = Object.create(basePing) as Ping;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.id !== undefined && object.id !== null) {
      message.id = Number(object.id);
    } else {
      message.id = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<Ping>): Ping {
    const message = Object.create(basePing) as Ping;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.id !== undefined && object.id !== null) {
      message.id = object.id;
    } else {
      message.id = 0;
    }
    return message;
  },
  toJSON(message: Ping): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.id = message.id || 0;
    return obj;
  },
};

export const Announcement = {
  encode(message: Announcement, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(18).bytes(message.announcement);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): Announcement {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseAnnouncement) as Announcement;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.announcement = reader.bytes();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): Announcement {
    const message = Object.create(baseAnnouncement) as Announcement;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.announcement !== undefined && object.announcement !== null) {
      message.announcement = bytesFromBase64(object.announcement);
    }
    return message;
  },
  fromPartial(object: DeepPartial<Announcement>): Announcement {
    const message = Object.create(baseAnnouncement) as Announcement;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.announcement !== undefined && object.announcement !== null) {
      message.announcement = object.announcement;
    }
    return message;
  },
  toJSON(message: Announcement): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.announcement = message.announcement !== undefined ? base64FromBytes(message.announcement) : undefined;
    return obj;
  },
};

export const LogEvent = {
  encode(message: LogEvent, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.id);
    writer.uint32(16).uint32(message.time);
    writer.uint32(24).int32(message.code);
    writer.uint32(37).float(message.oldValue);
    writer.uint32(45).float(message.newValue);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): LogEvent {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseLogEvent) as LogEvent;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.id = reader.uint32();
          break;
        case 2:
          message.time = reader.uint32();
          break;
        case 3:
          message.code = reader.int32() as any;
          break;
        case 4:
          message.oldValue = reader.float();
          break;
        case 5:
          message.newValue = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): LogEvent {
    const message = Object.create(baseLogEvent) as LogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = Number(object.id);
    } else {
      message.id = 0;
    }
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.code !== undefined && object.code !== null) {
      message.code = LogEventCode.fromJSON(object.code);
    } else {
      message.code = 0;
    }
    if (object.oldValue !== undefined && object.oldValue !== null) {
      message.oldValue = Number(object.oldValue);
    } else {
      message.oldValue = 0;
    }
    if (object.newValue !== undefined && object.newValue !== null) {
      message.newValue = Number(object.newValue);
    } else {
      message.newValue = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<LogEvent>): LogEvent {
    const message = Object.create(baseLogEvent) as LogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = object.id;
    } else {
      message.id = 0;
    }
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.code !== undefined && object.code !== null) {
      message.code = object.code;
    } else {
      message.code = 0;
    }
    if (object.oldValue !== undefined && object.oldValue !== null) {
      message.oldValue = object.oldValue;
    } else {
      message.oldValue = 0;
    }
    if (object.newValue !== undefined && object.newValue !== null) {
      message.newValue = object.newValue;
    } else {
      message.newValue = 0;
    }
    return message;
  },
  toJSON(message: LogEvent): unknown {
    const obj: any = {};
    obj.id = message.id || 0;
    obj.time = message.time || 0;
    obj.code = LogEventCode.toJSON(message.code);
    obj.oldValue = message.oldValue || 0;
    obj.newValue = message.newValue || 0;
    return obj;
  },
};

export const ExpectedLogEvent = {
  encode(message: ExpectedLogEvent, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.id);
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): ExpectedLogEvent {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseExpectedLogEvent) as ExpectedLogEvent;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.id = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): ExpectedLogEvent {
    const message = Object.create(baseExpectedLogEvent) as ExpectedLogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = Number(object.id);
    } else {
      message.id = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<ExpectedLogEvent>): ExpectedLogEvent {
    const message = Object.create(baseExpectedLogEvent) as ExpectedLogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = object.id;
    } else {
      message.id = 0;
    }
    return message;
  },
  toJSON(message: ExpectedLogEvent): unknown {
    const obj: any = {};
    obj.id = message.id || 0;
    return obj;
  },
};

export const NextLogEvents = {
  encode(message: NextLogEvents, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.nextExpected);
    writer.uint32(16).uint32(message.total);
    writer.uint32(24).uint32(message.remaining);
    for (const v of message.elements) {
      LogEvent.encode(v!, writer.uint32(34).fork()).ldelim();
    }
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): NextLogEvents {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseNextLogEvents) as NextLogEvents;
    message.elements = [];
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.nextExpected = reader.uint32();
          break;
        case 2:
          message.total = reader.uint32();
          break;
        case 3:
          message.remaining = reader.uint32();
          break;
        case 4:
          message.elements.push(LogEvent.decode(reader, reader.uint32()));
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): NextLogEvents {
    const message = Object.create(baseNextLogEvents) as NextLogEvents;
    message.elements = [];
    if (object.nextExpected !== undefined && object.nextExpected !== null) {
      message.nextExpected = Number(object.nextExpected);
    } else {
      message.nextExpected = 0;
    }
    if (object.total !== undefined && object.total !== null) {
      message.total = Number(object.total);
    } else {
      message.total = 0;
    }
    if (object.remaining !== undefined && object.remaining !== null) {
      message.remaining = Number(object.remaining);
    } else {
      message.remaining = 0;
    }
    if (object.elements !== undefined && object.elements !== null) {
      for (const e of object.elements) {
        message.elements.push(LogEvent.fromJSON(e));
      }
    }
    return message;
  },
  fromPartial(object: DeepPartial<NextLogEvents>): NextLogEvents {
    const message = Object.create(baseNextLogEvents) as NextLogEvents;
    message.elements = [];
    if (object.nextExpected !== undefined && object.nextExpected !== null) {
      message.nextExpected = object.nextExpected;
    } else {
      message.nextExpected = 0;
    }
    if (object.total !== undefined && object.total !== null) {
      message.total = object.total;
    } else {
      message.total = 0;
    }
    if (object.remaining !== undefined && object.remaining !== null) {
      message.remaining = object.remaining;
    } else {
      message.remaining = 0;
    }
    if (object.elements !== undefined && object.elements !== null) {
      for (const e of object.elements) {
        message.elements.push(LogEvent.fromPartial(e));
      }
    }
    return message;
  },
  toJSON(message: NextLogEvents): unknown {
    const obj: any = {};
    obj.nextExpected = message.nextExpected || 0;
    obj.total = message.total || 0;
    obj.remaining = message.remaining || 0;
    if (message.elements) {
      obj.elements = message.elements.map(e => e ? LogEvent.toJSON(e) : undefined);
    } else {
      obj.elements = [];
    }
    return obj;
  },
};

export const ActiveLogEvents = {
  encode(message: ActiveLogEvents, writer: Writer = Writer.create()): Writer {
    writer.uint32(10).fork();
    for (const v of message.id) {
      writer.uint32(v);
    }
    writer.ldelim();
    return writer;
  },
  decode(input: Uint8Array | Reader, length?: number): ActiveLogEvents {
    const reader = input instanceof Uint8Array ? new Reader(input) : input;
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseActiveLogEvents) as ActiveLogEvents;
    message.id = [];
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          if ((tag & 7) === 2) {
            const end2 = reader.uint32() + reader.pos;
            while (reader.pos < end2) {
              message.id.push(reader.uint32());
            }
          } else {
            message.id.push(reader.uint32());
          }
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): ActiveLogEvents {
    const message = Object.create(baseActiveLogEvents) as ActiveLogEvents;
    message.id = [];
    if (object.id !== undefined && object.id !== null) {
      for (const e of object.id) {
        message.id.push(Number(e));
      }
    }
    return message;
  },
  fromPartial(object: DeepPartial<ActiveLogEvents>): ActiveLogEvents {
    const message = Object.create(baseActiveLogEvents) as ActiveLogEvents;
    message.id = [];
    if (object.id !== undefined && object.id !== null) {
      for (const e of object.id) {
        message.id.push(e);
      }
    }
    return message;
  },
  toJSON(message: ActiveLogEvents): unknown {
    const obj: any = {};
    if (message.id) {
      obj.id = message.id.map(e => e || 0);
    } else {
      obj.id = [];
    }
    return obj;
  },
};

interface WindowBase64 {
  atob(b64: string): string;
  btoa(bin: string): string;
}

const windowBase64 = (globalThis as unknown as WindowBase64);
const atob = windowBase64.atob || ((b64: string) => Buffer.from(b64, 'base64').toString('binary'));
const btoa = windowBase64.btoa || ((bin: string) => Buffer.from(bin, 'binary').toString('base64'));

function bytesFromBase64(b64: string): Uint8Array {
  const bin = atob(b64);
  const arr = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; ++i) {
      arr[i] = bin.charCodeAt(i);
  }
  return arr;
}

function base64FromBytes(arr: Uint8Array): string {
  const bin: string[] = [];
  for (let i = 0; i < arr.byteLength; ++i) {
    bin.push(String.fromCharCode(arr[i]));
  }
  return btoa(bin.join(''));
}
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