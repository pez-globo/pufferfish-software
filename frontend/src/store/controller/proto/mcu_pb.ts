/* eslint-disable */
import { Writer, Reader } from 'protobufjs/minimal';


export interface Alarms {
  time: number;
  alarmOne: boolean;
  alarmTwo: boolean;
}

export interface SensorMeasurements {
  time: number;
  paw: number;
  flow: number;
  volume: number;
  fio2: number;
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

export interface VentilationMode {
  support: SpontaneousSupport;
  cycling: VentilationCycling;
}

export interface Parameters {
  time: number;
  mode: VentilationMode | undefined;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
  fio2: number;
}

export interface ParametersRequest {
  time: number;
  mode: VentilationMode | undefined;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
  fio2: number;
}

export interface Ping {
  time: number;
  id: number;
}

export interface Announcement {
  time: number;
  announcement: Uint8Array;
}

const baseAlarms: object = {
  time: 0,
  alarmOne: false,
  alarmTwo: false,
};

const baseSensorMeasurements: object = {
  time: 0,
  paw: 0,
  flow: 0,
  volume: 0,
  fio2: 0,
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

const baseVentilationMode: object = {
  support: 0,
  cycling: 0,
};

const baseParameters: object = {
  time: 0,
  mode: undefined,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
  fio2: 0,
};

const baseParametersRequest: object = {
  time: 0,
  mode: undefined,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
  fio2: 0,
};

const basePing: object = {
  time: 0,
  id: 0,
};

const baseAnnouncement: object = {
  time: 0,
  announcement: undefined,
};

export const SpontaneousSupport = {
  ac: 0 as const,
  simv: 1 as const,
  UNRECOGNIZED: -1 as const,
  fromJSON(object: any): SpontaneousSupport {
    switch (object) {
      case 0:
      case "ac":
        return SpontaneousSupport.ac;
      case 1:
      case "simv":
        return SpontaneousSupport.simv;
      case -1:
      case "UNRECOGNIZED":
      default:
        return SpontaneousSupport.UNRECOGNIZED;
    }
  },
  toJSON(object: SpontaneousSupport): string {
    switch (object) {
      case SpontaneousSupport.ac:
        return "ac";
      case SpontaneousSupport.simv:
        return "simv";
      default:
        return "UNKNOWN";
    }
  },
}

export type SpontaneousSupport = 0 | 1 | -1;

export const VentilationCycling = {
  pc: 0 as const,
  vc: 1 as const,
  psv: 2 as const,
  UNRECOGNIZED: -1 as const,
  fromJSON(object: any): VentilationCycling {
    switch (object) {
      case 0:
      case "pc":
        return VentilationCycling.pc;
      case 1:
      case "vc":
        return VentilationCycling.vc;
      case 2:
      case "psv":
        return VentilationCycling.psv;
      case -1:
      case "UNRECOGNIZED":
      default:
        return VentilationCycling.UNRECOGNIZED;
    }
  },
  toJSON(object: VentilationCycling): string {
    switch (object) {
      case VentilationCycling.pc:
        return "pc";
      case VentilationCycling.vc:
        return "vc";
      case VentilationCycling.psv:
        return "psv";
      default:
        return "UNKNOWN";
    }
  },
}

export type VentilationCycling = 0 | 1 | 2 | -1;

export const Alarms = {
  encode(message: Alarms, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).bool(message.alarmOne);
    writer.uint32(24).bool(message.alarmTwo);
    return writer;
  },
  decode(reader: Reader, length?: number): Alarms {
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

export const SensorMeasurements = {
  encode(message: SensorMeasurements, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(21).float(message.paw);
    writer.uint32(29).float(message.flow);
    writer.uint32(37).float(message.volume);
    writer.uint32(45).float(message.fio2);
    return writer;
  },
  decode(reader: Reader, length?: number): SensorMeasurements {
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseSensorMeasurements) as SensorMeasurements;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.paw = reader.float();
          break;
        case 3:
          message.flow = reader.float();
          break;
        case 4:
          message.volume = reader.float();
          break;
        case 5:
          message.fio2 = reader.float();
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
    return message;
  },
  fromPartial(object: DeepPartial<SensorMeasurements>): SensorMeasurements {
    const message = Object.create(baseSensorMeasurements) as SensorMeasurements;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
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
    return message;
  },
  toJSON(message: SensorMeasurements): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.paw = message.paw || 0;
    obj.flow = message.flow || 0;
    obj.volume = message.volume || 0;
    obj.fio2 = message.fio2 || 0;
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
  decode(reader: Reader, length?: number): CycleMeasurements {
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

export const VentilationMode = {
  encode(message: VentilationMode, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).int32(message.support);
    writer.uint32(16).int32(message.cycling);
    return writer;
  },
  decode(reader: Reader, length?: number): VentilationMode {
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseVentilationMode) as VentilationMode;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.support = reader.int32() as any;
          break;
        case 2:
          message.cycling = reader.int32() as any;
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },
  fromJSON(object: any): VentilationMode {
    const message = Object.create(baseVentilationMode) as VentilationMode;
    if (object.support !== undefined && object.support !== null) {
      message.support = SpontaneousSupport.fromJSON(object.support);
    } else {
      message.support = 0;
    }
    if (object.cycling !== undefined && object.cycling !== null) {
      message.cycling = VentilationCycling.fromJSON(object.cycling);
    } else {
      message.cycling = 0;
    }
    return message;
  },
  fromPartial(object: DeepPartial<VentilationMode>): VentilationMode {
    const message = Object.create(baseVentilationMode) as VentilationMode;
    if (object.support !== undefined && object.support !== null) {
      message.support = object.support;
    } else {
      message.support = 0;
    }
    if (object.cycling !== undefined && object.cycling !== null) {
      message.cycling = object.cycling;
    } else {
      message.cycling = 0;
    }
    return message;
  },
  toJSON(message: VentilationMode): unknown {
    const obj: any = {};
    obj.support = SpontaneousSupport.toJSON(message.support);
    obj.cycling = VentilationCycling.toJSON(message.cycling);
    return obj;
  },
};

export const Parameters = {
  encode(message: Parameters, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    if (message.mode !== undefined && message.mode !== undefined) {
      VentilationMode.encode(message.mode, writer.uint32(18).fork()).ldelim();
    }
    writer.uint32(29).float(message.pip);
    writer.uint32(37).float(message.peep);
    writer.uint32(45).float(message.vt);
    writer.uint32(53).float(message.rr);
    writer.uint32(61).float(message.ie);
    writer.uint32(69).float(message.fio2);
    return writer;
  },
  decode(reader: Reader, length?: number): Parameters {
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseParameters) as Parameters;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.mode = VentilationMode.decode(reader, reader.uint32());
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
      message.mode = undefined;
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
      message.mode = VentilationMode.fromPartial(object.mode);
    } else {
      message.mode = undefined;
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
    return message;
  },
  toJSON(message: Parameters): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.mode = message.mode ? VentilationMode.toJSON(message.mode) : undefined;
    obj.pip = message.pip || 0;
    obj.peep = message.peep || 0;
    obj.vt = message.vt || 0;
    obj.rr = message.rr || 0;
    obj.ie = message.ie || 0;
    obj.fio2 = message.fio2 || 0;
    return obj;
  },
};

export const ParametersRequest = {
  encode(message: ParametersRequest, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    if (message.mode !== undefined && message.mode !== undefined) {
      VentilationMode.encode(message.mode, writer.uint32(18).fork()).ldelim();
    }
    writer.uint32(29).float(message.pip);
    writer.uint32(37).float(message.peep);
    writer.uint32(45).float(message.vt);
    writer.uint32(53).float(message.rr);
    writer.uint32(61).float(message.ie);
    writer.uint32(69).float(message.fio2);
    return writer;
  },
  decode(reader: Reader, length?: number): ParametersRequest {
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = Object.create(baseParametersRequest) as ParametersRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = reader.uint32();
          break;
        case 2:
          message.mode = VentilationMode.decode(reader, reader.uint32());
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
      message.mode = undefined;
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
      message.mode = VentilationMode.fromPartial(object.mode);
    } else {
      message.mode = undefined;
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
    return message;
  },
  toJSON(message: ParametersRequest): unknown {
    const obj: any = {};
    obj.time = message.time || 0;
    obj.mode = message.mode ? VentilationMode.toJSON(message.mode) : undefined;
    obj.pip = message.pip || 0;
    obj.peep = message.peep || 0;
    obj.vt = message.vt || 0;
    obj.rr = message.rr || 0;
    obj.ie = message.ie || 0;
    obj.fio2 = message.fio2 || 0;
    return obj;
  },
};

export const Ping = {
  encode(message: Ping, writer: Writer = Writer.create()): Writer {
    writer.uint32(8).uint32(message.time);
    writer.uint32(16).uint32(message.id);
    return writer;
  },
  decode(reader: Reader, length?: number): Ping {
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
  decode(reader: Reader, length?: number): Announcement {
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