/* eslint-disable */
import Long from "long";
import _m0 from "protobufjs/minimal";

export const protobufPackage = "";

export enum VentilationMode {
  hfnc = 0,
  pc_ac = 1,
  vc_ac = 2,
  niv_pc = 3,
  niv_ps = 4,
  psv = 5,
  prvc = 6,
  UNRECOGNIZED = -1,
}

export function ventilationModeFromJSON(object: any): VentilationMode {
  switch (object) {
    case 0:
    case "hfnc":
      return VentilationMode.hfnc;
    case 1:
    case "pc_ac":
      return VentilationMode.pc_ac;
    case 2:
    case "vc_ac":
      return VentilationMode.vc_ac;
    case 3:
    case "niv_pc":
      return VentilationMode.niv_pc;
    case 4:
    case "niv_ps":
      return VentilationMode.niv_ps;
    case 5:
    case "psv":
      return VentilationMode.psv;
    case 6:
    case "prvc":
      return VentilationMode.prvc;
    case -1:
    case "UNRECOGNIZED":
    default:
      return VentilationMode.UNRECOGNIZED;
  }
}

export function ventilationModeToJSON(object: VentilationMode): string {
  switch (object) {
    case VentilationMode.hfnc:
      return "hfnc";
    case VentilationMode.pc_ac:
      return "pc_ac";
    case VentilationMode.vc_ac:
      return "vc_ac";
    case VentilationMode.niv_pc:
      return "niv_pc";
    case VentilationMode.niv_ps:
      return "niv_ps";
    case VentilationMode.psv:
      return "psv";
    case VentilationMode.prvc:
      return "prvc";
    default:
      return "UNKNOWN";
  }
}

export enum LogEventCode {
  /** fio2_too_low - Patient alarms */
  fio2_too_low = 0,
  fio2_too_high = 1,
  flow_too_low = 2,
  flow_too_high = 3,
  spo2_too_low = 4,
  spo2_too_high = 5,
  hr_too_low = 6,
  hr_too_high = 7,
  /** ventilation_operation_changed - Control settings */
  ventilation_operation_changed = 64,
  ventilation_mode_changed = 65,
  fio2_setting_changed = 66,
  flow_setting_changed = 67,
  /** fio2_alarm_limits_changed - Alarm limits settings */
  fio2_alarm_limits_changed = 80,
  flow_alarm_limits_changed = 81,
  spo2_alarm_limits_changed = 82,
  hr_alarm_limits_changed = 83,
  /** screen_locked - System settings & alarms */
  screen_locked = 129,
  /** mcu_backend_connection_down - mcu lost backend */
  mcu_backend_connection_down = 130,
  /** backend_mcu_connection_down - backend lost mcu */
  backend_mcu_connection_down = 131,
  /** backend_frontend_connection_down - backend lost frontend */
  backend_frontend_connection_down = 132,
  /**
   * frontend_backend_connection_down - TODO: if the backend can't send data to the frontend but the frontend can send
   * data to the backend, we should make the backend generate a frontend_backend_down
   * log event so that there's some record in the event log of a connection problem.
   * We can achieve this using a FrontendConnections message type which the frontend sends
   * to the backend.
   */
  frontend_backend_connection_down = 133,
  /** mcu_backend_connection_up - mcu detected backend */
  mcu_backend_connection_up = 134,
  /**
   * backend_mcu_connection_up - TODO: if the mcu can't send data to the backend but the backend can send
   * data to the mcu, we should make the backend generate a backend_mcu_up
   * log event once the mcu becomes able to send data again so that the event log
   * has a connection_up event to match the connection_down event.
   */
  backend_mcu_connection_up = 135,
  /** backend_frontend_connection_up - backend detected frontend */
  backend_frontend_connection_up = 136,
  /**
   * frontend_backend_connection_up - TODO: if the backend can't send data to the frontend but the frontend can send
   * data to the backend, we should make the backend generate a frontend_backend_up
   * log event once the frontend detects that the backend is able to send data again,
   * so that the event log has a connection_up event to match the connection_down event.
   * We can achieve this using a FrontendConnections message type which the frontend sends
   * to the backend.
   */
  frontend_backend_connection_up = 137,
  battery_low = 138,
  battery_critical = 139,
  charger_disconnected = 140,
  mcu_started = 141,
  backend_started = 142,
  mcu_shutdown = 143,
  backend_shutdown = 144,
  /** alarms_muted_user_software - Alarm muting/unmuting */
  alarms_muted_user_software = 145,
  alarms_muted_user_hardware = 146,
  /** alarms_muted_unknown - Indicates a software bug; should never occur */
  alarms_muted_unknown = 147,
  alarms_unmuted_user_software = 148,
  alarms_unmuted_user_hardware = 149,
  alarms_unmuted_initialization = 150,
  alarms_unmuted_timeout = 151,
  alarms_unmuted_mcu_backend_loss = 152,
  alarms_unmuted_backend_mcu_loss = 153,
  alarms_unmuted_backend_frontend_loss = 154,
  alarms_unmuted_frontend_backend_loss = 155,
  /** alarms_unmuted_unknown - Indicates a software bug; should never occur */
  alarms_unmuted_unknown = 156,
  /** sfm3019_air_disconnected - Sensor loss */
  sfm3019_air_disconnected = 160,
  sfm3019_o2_disconnected = 161,
  fdo2_disconnected = 162,
  nonin_disconnected = 163,
  nonin_finger_sensor_disconnected = 164,
  nonin_sensor_alarm = 165,
  nonin_out_of_track_measurements = 166,
  UNRECOGNIZED = -1,
}

export function logEventCodeFromJSON(object: any): LogEventCode {
  switch (object) {
    case 0:
    case "fio2_too_low":
      return LogEventCode.fio2_too_low;
    case 1:
    case "fio2_too_high":
      return LogEventCode.fio2_too_high;
    case 2:
    case "flow_too_low":
      return LogEventCode.flow_too_low;
    case 3:
    case "flow_too_high":
      return LogEventCode.flow_too_high;
    case 4:
    case "spo2_too_low":
      return LogEventCode.spo2_too_low;
    case 5:
    case "spo2_too_high":
      return LogEventCode.spo2_too_high;
    case 6:
    case "hr_too_low":
      return LogEventCode.hr_too_low;
    case 7:
    case "hr_too_high":
      return LogEventCode.hr_too_high;
    case 64:
    case "ventilation_operation_changed":
      return LogEventCode.ventilation_operation_changed;
    case 65:
    case "ventilation_mode_changed":
      return LogEventCode.ventilation_mode_changed;
    case 66:
    case "fio2_setting_changed":
      return LogEventCode.fio2_setting_changed;
    case 67:
    case "flow_setting_changed":
      return LogEventCode.flow_setting_changed;
    case 80:
    case "fio2_alarm_limits_changed":
      return LogEventCode.fio2_alarm_limits_changed;
    case 81:
    case "flow_alarm_limits_changed":
      return LogEventCode.flow_alarm_limits_changed;
    case 82:
    case "spo2_alarm_limits_changed":
      return LogEventCode.spo2_alarm_limits_changed;
    case 83:
    case "hr_alarm_limits_changed":
      return LogEventCode.hr_alarm_limits_changed;
    case 129:
    case "screen_locked":
      return LogEventCode.screen_locked;
    case 130:
    case "mcu_backend_connection_down":
      return LogEventCode.mcu_backend_connection_down;
    case 131:
    case "backend_mcu_connection_down":
      return LogEventCode.backend_mcu_connection_down;
    case 132:
    case "backend_frontend_connection_down":
      return LogEventCode.backend_frontend_connection_down;
    case 133:
    case "frontend_backend_connection_down":
      return LogEventCode.frontend_backend_connection_down;
    case 134:
    case "mcu_backend_connection_up":
      return LogEventCode.mcu_backend_connection_up;
    case 135:
    case "backend_mcu_connection_up":
      return LogEventCode.backend_mcu_connection_up;
    case 136:
    case "backend_frontend_connection_up":
      return LogEventCode.backend_frontend_connection_up;
    case 137:
    case "frontend_backend_connection_up":
      return LogEventCode.frontend_backend_connection_up;
    case 138:
    case "battery_low":
      return LogEventCode.battery_low;
    case 139:
    case "battery_critical":
      return LogEventCode.battery_critical;
    case 140:
    case "charger_disconnected":
      return LogEventCode.charger_disconnected;
    case 141:
    case "mcu_started":
      return LogEventCode.mcu_started;
    case 142:
    case "backend_started":
      return LogEventCode.backend_started;
    case 143:
    case "mcu_shutdown":
      return LogEventCode.mcu_shutdown;
    case 144:
    case "backend_shutdown":
      return LogEventCode.backend_shutdown;
    case 145:
    case "alarms_muted_user_software":
      return LogEventCode.alarms_muted_user_software;
    case 146:
    case "alarms_muted_user_hardware":
      return LogEventCode.alarms_muted_user_hardware;
    case 147:
    case "alarms_muted_unknown":
      return LogEventCode.alarms_muted_unknown;
    case 148:
    case "alarms_unmuted_user_software":
      return LogEventCode.alarms_unmuted_user_software;
    case 149:
    case "alarms_unmuted_user_hardware":
      return LogEventCode.alarms_unmuted_user_hardware;
    case 150:
    case "alarms_unmuted_initialization":
      return LogEventCode.alarms_unmuted_initialization;
    case 151:
    case "alarms_unmuted_timeout":
      return LogEventCode.alarms_unmuted_timeout;
    case 152:
    case "alarms_unmuted_mcu_backend_loss":
      return LogEventCode.alarms_unmuted_mcu_backend_loss;
    case 153:
    case "alarms_unmuted_backend_mcu_loss":
      return LogEventCode.alarms_unmuted_backend_mcu_loss;
    case 154:
    case "alarms_unmuted_backend_frontend_loss":
      return LogEventCode.alarms_unmuted_backend_frontend_loss;
    case 155:
    case "alarms_unmuted_frontend_backend_loss":
      return LogEventCode.alarms_unmuted_frontend_backend_loss;
    case 156:
    case "alarms_unmuted_unknown":
      return LogEventCode.alarms_unmuted_unknown;
    case 160:
    case "sfm3019_air_disconnected":
      return LogEventCode.sfm3019_air_disconnected;
    case 161:
    case "sfm3019_o2_disconnected":
      return LogEventCode.sfm3019_o2_disconnected;
    case 162:
    case "fdo2_disconnected":
      return LogEventCode.fdo2_disconnected;
    case 163:
    case "nonin_disconnected":
      return LogEventCode.nonin_disconnected;
    case 164:
    case "nonin_finger_sensor_disconnected":
      return LogEventCode.nonin_finger_sensor_disconnected;
    case 165:
    case "nonin_sensor_alarm":
      return LogEventCode.nonin_sensor_alarm;
    case 166:
    case "nonin_out_of_track_measurements":
      return LogEventCode.nonin_out_of_track_measurements;
    case -1:
    case "UNRECOGNIZED":
    default:
      return LogEventCode.UNRECOGNIZED;
  }
}

export function logEventCodeToJSON(object: LogEventCode): string {
  switch (object) {
    case LogEventCode.fio2_too_low:
      return "fio2_too_low";
    case LogEventCode.fio2_too_high:
      return "fio2_too_high";
    case LogEventCode.flow_too_low:
      return "flow_too_low";
    case LogEventCode.flow_too_high:
      return "flow_too_high";
    case LogEventCode.spo2_too_low:
      return "spo2_too_low";
    case LogEventCode.spo2_too_high:
      return "spo2_too_high";
    case LogEventCode.hr_too_low:
      return "hr_too_low";
    case LogEventCode.hr_too_high:
      return "hr_too_high";
    case LogEventCode.ventilation_operation_changed:
      return "ventilation_operation_changed";
    case LogEventCode.ventilation_mode_changed:
      return "ventilation_mode_changed";
    case LogEventCode.fio2_setting_changed:
      return "fio2_setting_changed";
    case LogEventCode.flow_setting_changed:
      return "flow_setting_changed";
    case LogEventCode.fio2_alarm_limits_changed:
      return "fio2_alarm_limits_changed";
    case LogEventCode.flow_alarm_limits_changed:
      return "flow_alarm_limits_changed";
    case LogEventCode.spo2_alarm_limits_changed:
      return "spo2_alarm_limits_changed";
    case LogEventCode.hr_alarm_limits_changed:
      return "hr_alarm_limits_changed";
    case LogEventCode.screen_locked:
      return "screen_locked";
    case LogEventCode.mcu_backend_connection_down:
      return "mcu_backend_connection_down";
    case LogEventCode.backend_mcu_connection_down:
      return "backend_mcu_connection_down";
    case LogEventCode.backend_frontend_connection_down:
      return "backend_frontend_connection_down";
    case LogEventCode.frontend_backend_connection_down:
      return "frontend_backend_connection_down";
    case LogEventCode.mcu_backend_connection_up:
      return "mcu_backend_connection_up";
    case LogEventCode.backend_mcu_connection_up:
      return "backend_mcu_connection_up";
    case LogEventCode.backend_frontend_connection_up:
      return "backend_frontend_connection_up";
    case LogEventCode.frontend_backend_connection_up:
      return "frontend_backend_connection_up";
    case LogEventCode.battery_low:
      return "battery_low";
    case LogEventCode.battery_critical:
      return "battery_critical";
    case LogEventCode.charger_disconnected:
      return "charger_disconnected";
    case LogEventCode.mcu_started:
      return "mcu_started";
    case LogEventCode.backend_started:
      return "backend_started";
    case LogEventCode.mcu_shutdown:
      return "mcu_shutdown";
    case LogEventCode.backend_shutdown:
      return "backend_shutdown";
    case LogEventCode.alarms_muted_user_software:
      return "alarms_muted_user_software";
    case LogEventCode.alarms_muted_user_hardware:
      return "alarms_muted_user_hardware";
    case LogEventCode.alarms_muted_unknown:
      return "alarms_muted_unknown";
    case LogEventCode.alarms_unmuted_user_software:
      return "alarms_unmuted_user_software";
    case LogEventCode.alarms_unmuted_user_hardware:
      return "alarms_unmuted_user_hardware";
    case LogEventCode.alarms_unmuted_initialization:
      return "alarms_unmuted_initialization";
    case LogEventCode.alarms_unmuted_timeout:
      return "alarms_unmuted_timeout";
    case LogEventCode.alarms_unmuted_mcu_backend_loss:
      return "alarms_unmuted_mcu_backend_loss";
    case LogEventCode.alarms_unmuted_backend_mcu_loss:
      return "alarms_unmuted_backend_mcu_loss";
    case LogEventCode.alarms_unmuted_backend_frontend_loss:
      return "alarms_unmuted_backend_frontend_loss";
    case LogEventCode.alarms_unmuted_frontend_backend_loss:
      return "alarms_unmuted_frontend_backend_loss";
    case LogEventCode.alarms_unmuted_unknown:
      return "alarms_unmuted_unknown";
    case LogEventCode.sfm3019_air_disconnected:
      return "sfm3019_air_disconnected";
    case LogEventCode.sfm3019_o2_disconnected:
      return "sfm3019_o2_disconnected";
    case LogEventCode.fdo2_disconnected:
      return "fdo2_disconnected";
    case LogEventCode.nonin_disconnected:
      return "nonin_disconnected";
    case LogEventCode.nonin_finger_sensor_disconnected:
      return "nonin_finger_sensor_disconnected";
    case LogEventCode.nonin_sensor_alarm:
      return "nonin_sensor_alarm";
    case LogEventCode.nonin_out_of_track_measurements:
      return "nonin_out_of_track_measurements";
    default:
      return "UNKNOWN";
  }
}

export enum LogEventType {
  patient = 0,
  control = 1,
  alarm_limits = 2,
  system = 3,
  UNRECOGNIZED = -1,
}

export function logEventTypeFromJSON(object: any): LogEventType {
  switch (object) {
    case 0:
    case "patient":
      return LogEventType.patient;
    case 1:
    case "control":
      return LogEventType.control;
    case 2:
    case "alarm_limits":
      return LogEventType.alarm_limits;
    case 3:
    case "system":
      return LogEventType.system;
    case -1:
    case "UNRECOGNIZED":
    default:
      return LogEventType.UNRECOGNIZED;
  }
}

export function logEventTypeToJSON(object: LogEventType): string {
  switch (object) {
    case LogEventType.patient:
      return "patient";
    case LogEventType.control:
      return "control";
    case LogEventType.alarm_limits:
      return "alarm_limits";
    case LogEventType.system:
      return "system";
    default:
      return "UNKNOWN";
  }
}

export enum AlarmMuteSource {
  /** initialization - Alarm muting was set on initialization */
  initialization = 0,
  /** user_software - User took a software action to change alarm muting */
  user_software = 1,
  /** user_hardware - User took a hardware action to change alarm muting */
  user_hardware = 2,
  /** timeout - Alarm mute automatically timed out */
  timeout = 3,
  /** mcu_backend_loss - Alarm mute was cancelled by mcu due to connection loss */
  mcu_backend_loss = 4,
  /** backend_mcu_loss - Alarm mute was cancelled by backend due to connection loss */
  backend_mcu_loss = 5,
  /** backend_frontend_loss - Alarm mute was cancelled by backend due to connection loss */
  backend_frontend_loss = 6,
  /** frontend_backend_loss - Alarm mute was cancelled by frontend due to connection loss */
  frontend_backend_loss = 7,
  UNRECOGNIZED = -1,
}

export function alarmMuteSourceFromJSON(object: any): AlarmMuteSource {
  switch (object) {
    case 0:
    case "initialization":
      return AlarmMuteSource.initialization;
    case 1:
    case "user_software":
      return AlarmMuteSource.user_software;
    case 2:
    case "user_hardware":
      return AlarmMuteSource.user_hardware;
    case 3:
    case "timeout":
      return AlarmMuteSource.timeout;
    case 4:
    case "mcu_backend_loss":
      return AlarmMuteSource.mcu_backend_loss;
    case 5:
    case "backend_mcu_loss":
      return AlarmMuteSource.backend_mcu_loss;
    case 6:
    case "backend_frontend_loss":
      return AlarmMuteSource.backend_frontend_loss;
    case 7:
    case "frontend_backend_loss":
      return AlarmMuteSource.frontend_backend_loss;
    case -1:
    case "UNRECOGNIZED":
    default:
      return AlarmMuteSource.UNRECOGNIZED;
  }
}

export function alarmMuteSourceToJSON(object: AlarmMuteSource): string {
  switch (object) {
    case AlarmMuteSource.initialization:
      return "initialization";
    case AlarmMuteSource.user_software:
      return "user_software";
    case AlarmMuteSource.user_hardware:
      return "user_hardware";
    case AlarmMuteSource.timeout:
      return "timeout";
    case AlarmMuteSource.mcu_backend_loss:
      return "mcu_backend_loss";
    case AlarmMuteSource.backend_mcu_loss:
      return "backend_mcu_loss";
    case AlarmMuteSource.backend_frontend_loss:
      return "backend_frontend_loss";
    case AlarmMuteSource.frontend_backend_loss:
      return "frontend_backend_loss";
    default:
      return "UNKNOWN";
  }
}

export interface SensorMeasurements {
  /** ms */
  time: number;
  cycle: number;
  fio2: number;
  flow: number;
  spo2: number;
  hr: number;
  paw: number;
  volume: number;
}

export interface CycleMeasurements {
  /** ms */
  time: number;
  vt: number;
  rr: number;
  peep: number;
  pip: number;
  ip: number;
  ve: number;
}

export interface Parameters {
  /** ms */
  time: number;
  ventilating: boolean;
  mode: VentilationMode;
  fio2: number;
  flow: number;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
}

export interface ParametersRequest {
  /** ms */
  time: number;
  ventilating: boolean;
  mode: VentilationMode;
  fio2: number;
  flow: number;
  pip: number;
  peep: number;
  vt: number;
  rr: number;
  ie: number;
}

export interface Range {
  lower: number;
  upper: number;
}

export interface AlarmLimits {
  /** ms */
  time: number;
  fio2: Range | undefined;
  flow: Range | undefined;
  spo2: Range | undefined;
  hr: Range | undefined;
  rr: Range | undefined;
  pip: Range | undefined;
  peep: Range | undefined;
  ipAbovePeep: Range | undefined;
  inspTime: Range | undefined;
  paw: Range | undefined;
  mve: Range | undefined;
  tv: Range | undefined;
  etco2: Range | undefined;
  apnea: Range | undefined;
}

export interface AlarmLimitsRequest {
  /** ms */
  time: number;
  fio2: Range | undefined;
  flow: Range | undefined;
  spo2: Range | undefined;
  hr: Range | undefined;
  rr: Range | undefined;
  pip: Range | undefined;
  peep: Range | undefined;
  ipAbovePeep: Range | undefined;
  inspTime: Range | undefined;
  paw: Range | undefined;
  mve: Range | undefined;
  tv: Range | undefined;
  etco2: Range | undefined;
  apnea: Range | undefined;
}

export interface LogEvent {
  id: number;
  /** ms */
  time: number;
  code: LogEventCode;
  type: LogEventType;
  alarmLimits: Range | undefined;
  oldFloat: number;
  newFloat: number;
  oldUint32: number;
  newUint32: number;
  oldBool: boolean;
  newBool: boolean;
  oldRange: Range | undefined;
  newRange: Range | undefined;
  oldMode: VentilationMode;
  newMode: VentilationMode;
}

export interface ExpectedLogEvent {
  id: number;
  /**
   * session_id is checked by the NextLogEvents sender if the sender's log is
   * ephemeral; the sender will ignore any ExpectedLogEvent whose session_id
   * doesn't match the sender's session_id, which is announced in NextLogEvents.
   */
  sessionId: number;
}

/**
 * Note: NextLogEvents has a custom equality operator in the firmware which must
 * be updated if you add/remove/modify the fields of the protobuf definition!
 */
export interface NextLogEvents {
  nextExpected: number;
  total: number;
  remaining: number;
  /** used when the sender's log is ephemeral */
  sessionId: number;
  elements: LogEvent[];
}

/**
 * Note: NextLogEvents has a custom equality operator in the firmware which must
 * be updated if you add/remove/modify the fields of the protobuf definition!
 */
export interface ActiveLogEvents {
  id: number[];
}

export interface AlarmMute {
  active: boolean;
  /**
   * seq_num is a logical clock and advances in the firmware after each local action
   * in the firmware (such as a button-press) or the servicing of each external request.
   */
  seqNum: number;
  source: AlarmMuteSource;
  /** ms */
  remaining: number;
}

export interface AlarmMuteRequest {
  active: boolean;
  /**
   * seq_num is a logical clock which also acts as an idempotency key for requests:
   * the firmware only services an AlarmMuteRequest if the request's seq_num is one
   * greater than the seq_num in the firmware's copy of AlarmMute.
   */
  seqNum: number;
  source: AlarmMuteSource;
}

export interface MCUPowerStatus {
  powerLeft: number;
  charging: boolean;
}

export interface BackendConnections {
  hasMcu: boolean;
  hasFrontend: boolean;
}

export interface ScreenStatus {
  lock: boolean;
}

export interface Ping {
  /** ms */
  time: number;
  id: number;
}

export interface Announcement {
  /** ms */
  time: number;
  announcement: Uint8Array;
}

const baseSensorMeasurements: object = {
  time: 0,
  cycle: 0,
  fio2: 0,
  flow: 0,
  spo2: 0,
  hr: 0,
  paw: 0,
  volume: 0,
};

export const SensorMeasurements = {
  encode(
    message: SensorMeasurements,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.cycle !== 0) {
      writer.uint32(16).uint32(message.cycle);
    }
    if (message.fio2 !== 0) {
      writer.uint32(29).float(message.fio2);
    }
    if (message.flow !== 0) {
      writer.uint32(37).float(message.flow);
    }
    if (message.spo2 !== 0) {
      writer.uint32(45).float(message.spo2);
    }
    if (message.hr !== 0) {
      writer.uint32(53).float(message.hr);
    }
    if (message.paw !== 0) {
      writer.uint32(61).float(message.paw);
    }
    if (message.volume !== 0) {
      writer.uint32(69).float(message.volume);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): SensorMeasurements {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseSensorMeasurements } as SensorMeasurements;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 2:
          message.cycle = reader.uint32();
          break;
        case 3:
          message.fio2 = reader.float();
          break;
        case 4:
          message.flow = reader.float();
          break;
        case 5:
          message.spo2 = reader.float();
          break;
        case 6:
          message.hr = reader.float();
          break;
        case 7:
          message.paw = reader.float();
          break;
        case 8:
          message.volume = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): SensorMeasurements {
    const message = { ...baseSensorMeasurements } as SensorMeasurements;
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
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Number(object.spo2);
    } else {
      message.spo2 = 0;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = Number(object.hr);
    } else {
      message.hr = 0;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Number(object.paw);
    } else {
      message.paw = 0;
    }
    if (object.volume !== undefined && object.volume !== null) {
      message.volume = Number(object.volume);
    } else {
      message.volume = 0;
    }
    return message;
  },

  toJSON(message: SensorMeasurements): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.cycle !== undefined && (obj.cycle = message.cycle);
    message.fio2 !== undefined && (obj.fio2 = message.fio2);
    message.flow !== undefined && (obj.flow = message.flow);
    message.spo2 !== undefined && (obj.spo2 = message.spo2);
    message.hr !== undefined && (obj.hr = message.hr);
    message.paw !== undefined && (obj.paw = message.paw);
    message.volume !== undefined && (obj.volume = message.volume);
    return obj;
  },

  fromPartial(object: DeepPartial<SensorMeasurements>): SensorMeasurements {
    const message = { ...baseSensorMeasurements } as SensorMeasurements;
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
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = object.spo2;
    } else {
      message.spo2 = 0;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = object.hr;
    } else {
      message.hr = 0;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = object.paw;
    } else {
      message.paw = 0;
    }
    if (object.volume !== undefined && object.volume !== null) {
      message.volume = object.volume;
    } else {
      message.volume = 0;
    }
    return message;
  },
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

export const CycleMeasurements = {
  encode(
    message: CycleMeasurements,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.vt !== 0) {
      writer.uint32(21).float(message.vt);
    }
    if (message.rr !== 0) {
      writer.uint32(29).float(message.rr);
    }
    if (message.peep !== 0) {
      writer.uint32(37).float(message.peep);
    }
    if (message.pip !== 0) {
      writer.uint32(45).float(message.pip);
    }
    if (message.ip !== 0) {
      writer.uint32(53).float(message.ip);
    }
    if (message.ve !== 0) {
      writer.uint32(61).float(message.ve);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): CycleMeasurements {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseCycleMeasurements } as CycleMeasurements;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
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
    const message = { ...baseCycleMeasurements } as CycleMeasurements;
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

  toJSON(message: CycleMeasurements): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.vt !== undefined && (obj.vt = message.vt);
    message.rr !== undefined && (obj.rr = message.rr);
    message.peep !== undefined && (obj.peep = message.peep);
    message.pip !== undefined && (obj.pip = message.pip);
    message.ip !== undefined && (obj.ip = message.ip);
    message.ve !== undefined && (obj.ve = message.ve);
    return obj;
  },

  fromPartial(object: DeepPartial<CycleMeasurements>): CycleMeasurements {
    const message = { ...baseCycleMeasurements } as CycleMeasurements;
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
};

const baseParameters: object = {
  time: 0,
  ventilating: false,
  mode: 0,
  fio2: 0,
  flow: 0,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
};

export const Parameters = {
  encode(
    message: Parameters,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.ventilating === true) {
      writer.uint32(16).bool(message.ventilating);
    }
    if (message.mode !== 0) {
      writer.uint32(24).int32(message.mode);
    }
    if (message.fio2 !== 0) {
      writer.uint32(37).float(message.fio2);
    }
    if (message.flow !== 0) {
      writer.uint32(45).float(message.flow);
    }
    if (message.pip !== 0) {
      writer.uint32(53).float(message.pip);
    }
    if (message.peep !== 0) {
      writer.uint32(61).float(message.peep);
    }
    if (message.vt !== 0) {
      writer.uint32(69).float(message.vt);
    }
    if (message.rr !== 0) {
      writer.uint32(77).float(message.rr);
    }
    if (message.ie !== 0) {
      writer.uint32(85).float(message.ie);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Parameters {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseParameters } as Parameters;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 2:
          message.ventilating = reader.bool();
          break;
        case 3:
          message.mode = reader.int32() as any;
          break;
        case 4:
          message.fio2 = reader.float();
          break;
        case 5:
          message.flow = reader.float();
          break;
        case 6:
          message.pip = reader.float();
          break;
        case 7:
          message.peep = reader.float();
          break;
        case 8:
          message.vt = reader.float();
          break;
        case 9:
          message.rr = reader.float();
          break;
        case 10:
          message.ie = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): Parameters {
    const message = { ...baseParameters } as Parameters;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.ventilating !== undefined && object.ventilating !== null) {
      message.ventilating = Boolean(object.ventilating);
    } else {
      message.ventilating = false;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = ventilationModeFromJSON(object.mode);
    } else {
      message.mode = 0;
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
    return message;
  },

  toJSON(message: Parameters): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.ventilating !== undefined &&
      (obj.ventilating = message.ventilating);
    message.mode !== undefined &&
      (obj.mode = ventilationModeToJSON(message.mode));
    message.fio2 !== undefined && (obj.fio2 = message.fio2);
    message.flow !== undefined && (obj.flow = message.flow);
    message.pip !== undefined && (obj.pip = message.pip);
    message.peep !== undefined && (obj.peep = message.peep);
    message.vt !== undefined && (obj.vt = message.vt);
    message.rr !== undefined && (obj.rr = message.rr);
    message.ie !== undefined && (obj.ie = message.ie);
    return obj;
  },

  fromPartial(object: DeepPartial<Parameters>): Parameters {
    const message = { ...baseParameters } as Parameters;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.ventilating !== undefined && object.ventilating !== null) {
      message.ventilating = object.ventilating;
    } else {
      message.ventilating = false;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = object.mode;
    } else {
      message.mode = 0;
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
    return message;
  },
};

const baseParametersRequest: object = {
  time: 0,
  ventilating: false,
  mode: 0,
  fio2: 0,
  flow: 0,
  pip: 0,
  peep: 0,
  vt: 0,
  rr: 0,
  ie: 0,
};

export const ParametersRequest = {
  encode(
    message: ParametersRequest,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.ventilating === true) {
      writer.uint32(16).bool(message.ventilating);
    }
    if (message.mode !== 0) {
      writer.uint32(24).int32(message.mode);
    }
    if (message.fio2 !== 0) {
      writer.uint32(37).float(message.fio2);
    }
    if (message.flow !== 0) {
      writer.uint32(45).float(message.flow);
    }
    if (message.pip !== 0) {
      writer.uint32(53).float(message.pip);
    }
    if (message.peep !== 0) {
      writer.uint32(61).float(message.peep);
    }
    if (message.vt !== 0) {
      writer.uint32(69).float(message.vt);
    }
    if (message.rr !== 0) {
      writer.uint32(77).float(message.rr);
    }
    if (message.ie !== 0) {
      writer.uint32(85).float(message.ie);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): ParametersRequest {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseParametersRequest } as ParametersRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 2:
          message.ventilating = reader.bool();
          break;
        case 3:
          message.mode = reader.int32() as any;
          break;
        case 4:
          message.fio2 = reader.float();
          break;
        case 5:
          message.flow = reader.float();
          break;
        case 6:
          message.pip = reader.float();
          break;
        case 7:
          message.peep = reader.float();
          break;
        case 8:
          message.vt = reader.float();
          break;
        case 9:
          message.rr = reader.float();
          break;
        case 10:
          message.ie = reader.float();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): ParametersRequest {
    const message = { ...baseParametersRequest } as ParametersRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.ventilating !== undefined && object.ventilating !== null) {
      message.ventilating = Boolean(object.ventilating);
    } else {
      message.ventilating = false;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = ventilationModeFromJSON(object.mode);
    } else {
      message.mode = 0;
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
    return message;
  },

  toJSON(message: ParametersRequest): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.ventilating !== undefined &&
      (obj.ventilating = message.ventilating);
    message.mode !== undefined &&
      (obj.mode = ventilationModeToJSON(message.mode));
    message.fio2 !== undefined && (obj.fio2 = message.fio2);
    message.flow !== undefined && (obj.flow = message.flow);
    message.pip !== undefined && (obj.pip = message.pip);
    message.peep !== undefined && (obj.peep = message.peep);
    message.vt !== undefined && (obj.vt = message.vt);
    message.rr !== undefined && (obj.rr = message.rr);
    message.ie !== undefined && (obj.ie = message.ie);
    return obj;
  },

  fromPartial(object: DeepPartial<ParametersRequest>): ParametersRequest {
    const message = { ...baseParametersRequest } as ParametersRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.ventilating !== undefined && object.ventilating !== null) {
      message.ventilating = object.ventilating;
    } else {
      message.ventilating = false;
    }
    if (object.mode !== undefined && object.mode !== null) {
      message.mode = object.mode;
    } else {
      message.mode = 0;
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
    return message;
  },
};

const baseRange: object = { lower: 0, upper: 0 };

export const Range = {
  encode(message: Range, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.lower !== 0) {
      writer.uint32(8).int32(message.lower);
    }
    if (message.upper !== 0) {
      writer.uint32(16).int32(message.upper);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Range {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseRange } as Range;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.lower = reader.int32();
          break;
        case 2:
          message.upper = reader.int32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): Range {
    const message = { ...baseRange } as Range;
    if (object.lower !== undefined && object.lower !== null) {
      message.lower = Number(object.lower);
    } else {
      message.lower = 0;
    }
    if (object.upper !== undefined && object.upper !== null) {
      message.upper = Number(object.upper);
    } else {
      message.upper = 0;
    }
    return message;
  },

  toJSON(message: Range): unknown {
    const obj: any = {};
    message.lower !== undefined && (obj.lower = message.lower);
    message.upper !== undefined && (obj.upper = message.upper);
    return obj;
  },

  fromPartial(object: DeepPartial<Range>): Range {
    const message = { ...baseRange } as Range;
    if (object.lower !== undefined && object.lower !== null) {
      message.lower = object.lower;
    } else {
      message.lower = 0;
    }
    if (object.upper !== undefined && object.upper !== null) {
      message.upper = object.upper;
    } else {
      message.upper = 0;
    }
    return message;
  },
};

const baseAlarmLimits: object = { time: 0 };

export const AlarmLimits = {
  encode(
    message: AlarmLimits,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.fio2 !== undefined) {
      Range.encode(message.fio2, writer.uint32(18).fork()).ldelim();
    }
    if (message.flow !== undefined) {
      Range.encode(message.flow, writer.uint32(26).fork()).ldelim();
    }
    if (message.spo2 !== undefined) {
      Range.encode(message.spo2, writer.uint32(34).fork()).ldelim();
    }
    if (message.hr !== undefined) {
      Range.encode(message.hr, writer.uint32(42).fork()).ldelim();
    }
    if (message.rr !== undefined) {
      Range.encode(message.rr, writer.uint32(50).fork()).ldelim();
    }
    if (message.pip !== undefined) {
      Range.encode(message.pip, writer.uint32(58).fork()).ldelim();
    }
    if (message.peep !== undefined) {
      Range.encode(message.peep, writer.uint32(66).fork()).ldelim();
    }
    if (message.ipAbovePeep !== undefined) {
      Range.encode(message.ipAbovePeep, writer.uint32(74).fork()).ldelim();
    }
    if (message.inspTime !== undefined) {
      Range.encode(message.inspTime, writer.uint32(82).fork()).ldelim();
    }
    if (message.paw !== undefined) {
      Range.encode(message.paw, writer.uint32(90).fork()).ldelim();
    }
    if (message.mve !== undefined) {
      Range.encode(message.mve, writer.uint32(98).fork()).ldelim();
    }
    if (message.tv !== undefined) {
      Range.encode(message.tv, writer.uint32(106).fork()).ldelim();
    }
    if (message.etco2 !== undefined) {
      Range.encode(message.etco2, writer.uint32(114).fork()).ldelim();
    }
    if (message.apnea !== undefined) {
      Range.encode(message.apnea, writer.uint32(122).fork()).ldelim();
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): AlarmLimits {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseAlarmLimits } as AlarmLimits;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 2:
          message.fio2 = Range.decode(reader, reader.uint32());
          break;
        case 3:
          message.flow = Range.decode(reader, reader.uint32());
          break;
        case 4:
          message.spo2 = Range.decode(reader, reader.uint32());
          break;
        case 5:
          message.hr = Range.decode(reader, reader.uint32());
          break;
        case 6:
          message.rr = Range.decode(reader, reader.uint32());
          break;
        case 7:
          message.pip = Range.decode(reader, reader.uint32());
          break;
        case 8:
          message.peep = Range.decode(reader, reader.uint32());
          break;
        case 9:
          message.ipAbovePeep = Range.decode(reader, reader.uint32());
          break;
        case 10:
          message.inspTime = Range.decode(reader, reader.uint32());
          break;
        case 11:
          message.paw = Range.decode(reader, reader.uint32());
          break;
        case 12:
          message.mve = Range.decode(reader, reader.uint32());
          break;
        case 13:
          message.tv = Range.decode(reader, reader.uint32());
          break;
        case 14:
          message.etco2 = Range.decode(reader, reader.uint32());
          break;
        case 15:
          message.apnea = Range.decode(reader, reader.uint32());
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): AlarmLimits {
    const message = { ...baseAlarmLimits } as AlarmLimits;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Range.fromJSON(object.fio2);
    } else {
      message.fio2 = undefined;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Range.fromJSON(object.flow);
    } else {
      message.flow = undefined;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Range.fromJSON(object.spo2);
    } else {
      message.spo2 = undefined;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = Range.fromJSON(object.hr);
    } else {
      message.hr = undefined;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Range.fromJSON(object.rr);
    } else {
      message.rr = undefined;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Range.fromJSON(object.pip);
    } else {
      message.pip = undefined;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Range.fromJSON(object.peep);
    } else {
      message.peep = undefined;
    }
    if (object.ipAbovePeep !== undefined && object.ipAbovePeep !== null) {
      message.ipAbovePeep = Range.fromJSON(object.ipAbovePeep);
    } else {
      message.ipAbovePeep = undefined;
    }
    if (object.inspTime !== undefined && object.inspTime !== null) {
      message.inspTime = Range.fromJSON(object.inspTime);
    } else {
      message.inspTime = undefined;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Range.fromJSON(object.paw);
    } else {
      message.paw = undefined;
    }
    if (object.mve !== undefined && object.mve !== null) {
      message.mve = Range.fromJSON(object.mve);
    } else {
      message.mve = undefined;
    }
    if (object.tv !== undefined && object.tv !== null) {
      message.tv = Range.fromJSON(object.tv);
    } else {
      message.tv = undefined;
    }
    if (object.etco2 !== undefined && object.etco2 !== null) {
      message.etco2 = Range.fromJSON(object.etco2);
    } else {
      message.etco2 = undefined;
    }
    if (object.apnea !== undefined && object.apnea !== null) {
      message.apnea = Range.fromJSON(object.apnea);
    } else {
      message.apnea = undefined;
    }
    return message;
  },

  toJSON(message: AlarmLimits): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.fio2 !== undefined &&
      (obj.fio2 = message.fio2 ? Range.toJSON(message.fio2) : undefined);
    message.flow !== undefined &&
      (obj.flow = message.flow ? Range.toJSON(message.flow) : undefined);
    message.spo2 !== undefined &&
      (obj.spo2 = message.spo2 ? Range.toJSON(message.spo2) : undefined);
    message.hr !== undefined &&
      (obj.hr = message.hr ? Range.toJSON(message.hr) : undefined);
    message.rr !== undefined &&
      (obj.rr = message.rr ? Range.toJSON(message.rr) : undefined);
    message.pip !== undefined &&
      (obj.pip = message.pip ? Range.toJSON(message.pip) : undefined);
    message.peep !== undefined &&
      (obj.peep = message.peep ? Range.toJSON(message.peep) : undefined);
    message.ipAbovePeep !== undefined &&
      (obj.ipAbovePeep = message.ipAbovePeep
        ? Range.toJSON(message.ipAbovePeep)
        : undefined);
    message.inspTime !== undefined &&
      (obj.inspTime = message.inspTime
        ? Range.toJSON(message.inspTime)
        : undefined);
    message.paw !== undefined &&
      (obj.paw = message.paw ? Range.toJSON(message.paw) : undefined);
    message.mve !== undefined &&
      (obj.mve = message.mve ? Range.toJSON(message.mve) : undefined);
    message.tv !== undefined &&
      (obj.tv = message.tv ? Range.toJSON(message.tv) : undefined);
    message.etco2 !== undefined &&
      (obj.etco2 = message.etco2 ? Range.toJSON(message.etco2) : undefined);
    message.apnea !== undefined &&
      (obj.apnea = message.apnea ? Range.toJSON(message.apnea) : undefined);
    return obj;
  },

  fromPartial(object: DeepPartial<AlarmLimits>): AlarmLimits {
    const message = { ...baseAlarmLimits } as AlarmLimits;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Range.fromPartial(object.fio2);
    } else {
      message.fio2 = undefined;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Range.fromPartial(object.flow);
    } else {
      message.flow = undefined;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Range.fromPartial(object.spo2);
    } else {
      message.spo2 = undefined;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = Range.fromPartial(object.hr);
    } else {
      message.hr = undefined;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Range.fromPartial(object.rr);
    } else {
      message.rr = undefined;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Range.fromPartial(object.pip);
    } else {
      message.pip = undefined;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Range.fromPartial(object.peep);
    } else {
      message.peep = undefined;
    }
    if (object.ipAbovePeep !== undefined && object.ipAbovePeep !== null) {
      message.ipAbovePeep = Range.fromPartial(object.ipAbovePeep);
    } else {
      message.ipAbovePeep = undefined;
    }
    if (object.inspTime !== undefined && object.inspTime !== null) {
      message.inspTime = Range.fromPartial(object.inspTime);
    } else {
      message.inspTime = undefined;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Range.fromPartial(object.paw);
    } else {
      message.paw = undefined;
    }
    if (object.mve !== undefined && object.mve !== null) {
      message.mve = Range.fromPartial(object.mve);
    } else {
      message.mve = undefined;
    }
    if (object.tv !== undefined && object.tv !== null) {
      message.tv = Range.fromPartial(object.tv);
    } else {
      message.tv = undefined;
    }
    if (object.etco2 !== undefined && object.etco2 !== null) {
      message.etco2 = Range.fromPartial(object.etco2);
    } else {
      message.etco2 = undefined;
    }
    if (object.apnea !== undefined && object.apnea !== null) {
      message.apnea = Range.fromPartial(object.apnea);
    } else {
      message.apnea = undefined;
    }
    return message;
  },
};

const baseAlarmLimitsRequest: object = { time: 0 };

export const AlarmLimitsRequest = {
  encode(
    message: AlarmLimitsRequest,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.fio2 !== undefined) {
      Range.encode(message.fio2, writer.uint32(18).fork()).ldelim();
    }
    if (message.flow !== undefined) {
      Range.encode(message.flow, writer.uint32(26).fork()).ldelim();
    }
    if (message.spo2 !== undefined) {
      Range.encode(message.spo2, writer.uint32(34).fork()).ldelim();
    }
    if (message.hr !== undefined) {
      Range.encode(message.hr, writer.uint32(42).fork()).ldelim();
    }
    if (message.rr !== undefined) {
      Range.encode(message.rr, writer.uint32(50).fork()).ldelim();
    }
    if (message.pip !== undefined) {
      Range.encode(message.pip, writer.uint32(58).fork()).ldelim();
    }
    if (message.peep !== undefined) {
      Range.encode(message.peep, writer.uint32(66).fork()).ldelim();
    }
    if (message.ipAbovePeep !== undefined) {
      Range.encode(message.ipAbovePeep, writer.uint32(74).fork()).ldelim();
    }
    if (message.inspTime !== undefined) {
      Range.encode(message.inspTime, writer.uint32(82).fork()).ldelim();
    }
    if (message.paw !== undefined) {
      Range.encode(message.paw, writer.uint32(90).fork()).ldelim();
    }
    if (message.mve !== undefined) {
      Range.encode(message.mve, writer.uint32(98).fork()).ldelim();
    }
    if (message.tv !== undefined) {
      Range.encode(message.tv, writer.uint32(106).fork()).ldelim();
    }
    if (message.etco2 !== undefined) {
      Range.encode(message.etco2, writer.uint32(114).fork()).ldelim();
    }
    if (message.apnea !== undefined) {
      Range.encode(message.apnea, writer.uint32(122).fork()).ldelim();
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): AlarmLimitsRequest {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseAlarmLimitsRequest } as AlarmLimitsRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 2:
          message.fio2 = Range.decode(reader, reader.uint32());
          break;
        case 3:
          message.flow = Range.decode(reader, reader.uint32());
          break;
        case 4:
          message.spo2 = Range.decode(reader, reader.uint32());
          break;
        case 5:
          message.hr = Range.decode(reader, reader.uint32());
          break;
        case 6:
          message.rr = Range.decode(reader, reader.uint32());
          break;
        case 7:
          message.pip = Range.decode(reader, reader.uint32());
          break;
        case 8:
          message.peep = Range.decode(reader, reader.uint32());
          break;
        case 9:
          message.ipAbovePeep = Range.decode(reader, reader.uint32());
          break;
        case 10:
          message.inspTime = Range.decode(reader, reader.uint32());
          break;
        case 11:
          message.paw = Range.decode(reader, reader.uint32());
          break;
        case 12:
          message.mve = Range.decode(reader, reader.uint32());
          break;
        case 13:
          message.tv = Range.decode(reader, reader.uint32());
          break;
        case 14:
          message.etco2 = Range.decode(reader, reader.uint32());
          break;
        case 15:
          message.apnea = Range.decode(reader, reader.uint32());
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): AlarmLimitsRequest {
    const message = { ...baseAlarmLimitsRequest } as AlarmLimitsRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = Number(object.time);
    } else {
      message.time = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Range.fromJSON(object.fio2);
    } else {
      message.fio2 = undefined;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Range.fromJSON(object.flow);
    } else {
      message.flow = undefined;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Range.fromJSON(object.spo2);
    } else {
      message.spo2 = undefined;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = Range.fromJSON(object.hr);
    } else {
      message.hr = undefined;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Range.fromJSON(object.rr);
    } else {
      message.rr = undefined;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Range.fromJSON(object.pip);
    } else {
      message.pip = undefined;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Range.fromJSON(object.peep);
    } else {
      message.peep = undefined;
    }
    if (object.ipAbovePeep !== undefined && object.ipAbovePeep !== null) {
      message.ipAbovePeep = Range.fromJSON(object.ipAbovePeep);
    } else {
      message.ipAbovePeep = undefined;
    }
    if (object.inspTime !== undefined && object.inspTime !== null) {
      message.inspTime = Range.fromJSON(object.inspTime);
    } else {
      message.inspTime = undefined;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Range.fromJSON(object.paw);
    } else {
      message.paw = undefined;
    }
    if (object.mve !== undefined && object.mve !== null) {
      message.mve = Range.fromJSON(object.mve);
    } else {
      message.mve = undefined;
    }
    if (object.tv !== undefined && object.tv !== null) {
      message.tv = Range.fromJSON(object.tv);
    } else {
      message.tv = undefined;
    }
    if (object.etco2 !== undefined && object.etco2 !== null) {
      message.etco2 = Range.fromJSON(object.etco2);
    } else {
      message.etco2 = undefined;
    }
    if (object.apnea !== undefined && object.apnea !== null) {
      message.apnea = Range.fromJSON(object.apnea);
    } else {
      message.apnea = undefined;
    }
    return message;
  },

  toJSON(message: AlarmLimitsRequest): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.fio2 !== undefined &&
      (obj.fio2 = message.fio2 ? Range.toJSON(message.fio2) : undefined);
    message.flow !== undefined &&
      (obj.flow = message.flow ? Range.toJSON(message.flow) : undefined);
    message.spo2 !== undefined &&
      (obj.spo2 = message.spo2 ? Range.toJSON(message.spo2) : undefined);
    message.hr !== undefined &&
      (obj.hr = message.hr ? Range.toJSON(message.hr) : undefined);
    message.rr !== undefined &&
      (obj.rr = message.rr ? Range.toJSON(message.rr) : undefined);
    message.pip !== undefined &&
      (obj.pip = message.pip ? Range.toJSON(message.pip) : undefined);
    message.peep !== undefined &&
      (obj.peep = message.peep ? Range.toJSON(message.peep) : undefined);
    message.ipAbovePeep !== undefined &&
      (obj.ipAbovePeep = message.ipAbovePeep
        ? Range.toJSON(message.ipAbovePeep)
        : undefined);
    message.inspTime !== undefined &&
      (obj.inspTime = message.inspTime
        ? Range.toJSON(message.inspTime)
        : undefined);
    message.paw !== undefined &&
      (obj.paw = message.paw ? Range.toJSON(message.paw) : undefined);
    message.mve !== undefined &&
      (obj.mve = message.mve ? Range.toJSON(message.mve) : undefined);
    message.tv !== undefined &&
      (obj.tv = message.tv ? Range.toJSON(message.tv) : undefined);
    message.etco2 !== undefined &&
      (obj.etco2 = message.etco2 ? Range.toJSON(message.etco2) : undefined);
    message.apnea !== undefined &&
      (obj.apnea = message.apnea ? Range.toJSON(message.apnea) : undefined);
    return obj;
  },

  fromPartial(object: DeepPartial<AlarmLimitsRequest>): AlarmLimitsRequest {
    const message = { ...baseAlarmLimitsRequest } as AlarmLimitsRequest;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.fio2 !== undefined && object.fio2 !== null) {
      message.fio2 = Range.fromPartial(object.fio2);
    } else {
      message.fio2 = undefined;
    }
    if (object.flow !== undefined && object.flow !== null) {
      message.flow = Range.fromPartial(object.flow);
    } else {
      message.flow = undefined;
    }
    if (object.spo2 !== undefined && object.spo2 !== null) {
      message.spo2 = Range.fromPartial(object.spo2);
    } else {
      message.spo2 = undefined;
    }
    if (object.hr !== undefined && object.hr !== null) {
      message.hr = Range.fromPartial(object.hr);
    } else {
      message.hr = undefined;
    }
    if (object.rr !== undefined && object.rr !== null) {
      message.rr = Range.fromPartial(object.rr);
    } else {
      message.rr = undefined;
    }
    if (object.pip !== undefined && object.pip !== null) {
      message.pip = Range.fromPartial(object.pip);
    } else {
      message.pip = undefined;
    }
    if (object.peep !== undefined && object.peep !== null) {
      message.peep = Range.fromPartial(object.peep);
    } else {
      message.peep = undefined;
    }
    if (object.ipAbovePeep !== undefined && object.ipAbovePeep !== null) {
      message.ipAbovePeep = Range.fromPartial(object.ipAbovePeep);
    } else {
      message.ipAbovePeep = undefined;
    }
    if (object.inspTime !== undefined && object.inspTime !== null) {
      message.inspTime = Range.fromPartial(object.inspTime);
    } else {
      message.inspTime = undefined;
    }
    if (object.paw !== undefined && object.paw !== null) {
      message.paw = Range.fromPartial(object.paw);
    } else {
      message.paw = undefined;
    }
    if (object.mve !== undefined && object.mve !== null) {
      message.mve = Range.fromPartial(object.mve);
    } else {
      message.mve = undefined;
    }
    if (object.tv !== undefined && object.tv !== null) {
      message.tv = Range.fromPartial(object.tv);
    } else {
      message.tv = undefined;
    }
    if (object.etco2 !== undefined && object.etco2 !== null) {
      message.etco2 = Range.fromPartial(object.etco2);
    } else {
      message.etco2 = undefined;
    }
    if (object.apnea !== undefined && object.apnea !== null) {
      message.apnea = Range.fromPartial(object.apnea);
    } else {
      message.apnea = undefined;
    }
    return message;
  },
};

const baseLogEvent: object = {
  id: 0,
  time: 0,
  code: 0,
  type: 0,
  oldFloat: 0,
  newFloat: 0,
  oldUint32: 0,
  newUint32: 0,
  oldBool: false,
  newBool: false,
  oldMode: 0,
  newMode: 0,
};

export const LogEvent = {
  encode(
    message: LogEvent,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.id !== 0) {
      writer.uint32(8).uint32(message.id);
    }
    if (message.time !== 0) {
      writer.uint32(16).uint64(message.time);
    }
    if (message.code !== 0) {
      writer.uint32(24).int32(message.code);
    }
    if (message.type !== 0) {
      writer.uint32(32).int32(message.type);
    }
    if (message.alarmLimits !== undefined) {
      Range.encode(message.alarmLimits, writer.uint32(42).fork()).ldelim();
    }
    if (message.oldFloat !== 0) {
      writer.uint32(53).float(message.oldFloat);
    }
    if (message.newFloat !== 0) {
      writer.uint32(61).float(message.newFloat);
    }
    if (message.oldUint32 !== 0) {
      writer.uint32(64).uint32(message.oldUint32);
    }
    if (message.newUint32 !== 0) {
      writer.uint32(72).uint32(message.newUint32);
    }
    if (message.oldBool === true) {
      writer.uint32(80).bool(message.oldBool);
    }
    if (message.newBool === true) {
      writer.uint32(88).bool(message.newBool);
    }
    if (message.oldRange !== undefined) {
      Range.encode(message.oldRange, writer.uint32(98).fork()).ldelim();
    }
    if (message.newRange !== undefined) {
      Range.encode(message.newRange, writer.uint32(106).fork()).ldelim();
    }
    if (message.oldMode !== 0) {
      writer.uint32(112).int32(message.oldMode);
    }
    if (message.newMode !== 0) {
      writer.uint32(120).int32(message.newMode);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): LogEvent {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseLogEvent } as LogEvent;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.id = reader.uint32();
          break;
        case 2:
          message.time = longToNumber(reader.uint64() as Long);
          break;
        case 3:
          message.code = reader.int32() as any;
          break;
        case 4:
          message.type = reader.int32() as any;
          break;
        case 5:
          message.alarmLimits = Range.decode(reader, reader.uint32());
          break;
        case 6:
          message.oldFloat = reader.float();
          break;
        case 7:
          message.newFloat = reader.float();
          break;
        case 8:
          message.oldUint32 = reader.uint32();
          break;
        case 9:
          message.newUint32 = reader.uint32();
          break;
        case 10:
          message.oldBool = reader.bool();
          break;
        case 11:
          message.newBool = reader.bool();
          break;
        case 12:
          message.oldRange = Range.decode(reader, reader.uint32());
          break;
        case 13:
          message.newRange = Range.decode(reader, reader.uint32());
          break;
        case 14:
          message.oldMode = reader.int32() as any;
          break;
        case 15:
          message.newMode = reader.int32() as any;
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): LogEvent {
    const message = { ...baseLogEvent } as LogEvent;
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
      message.code = logEventCodeFromJSON(object.code);
    } else {
      message.code = 0;
    }
    if (object.type !== undefined && object.type !== null) {
      message.type = logEventTypeFromJSON(object.type);
    } else {
      message.type = 0;
    }
    if (object.alarmLimits !== undefined && object.alarmLimits !== null) {
      message.alarmLimits = Range.fromJSON(object.alarmLimits);
    } else {
      message.alarmLimits = undefined;
    }
    if (object.oldFloat !== undefined && object.oldFloat !== null) {
      message.oldFloat = Number(object.oldFloat);
    } else {
      message.oldFloat = 0;
    }
    if (object.newFloat !== undefined && object.newFloat !== null) {
      message.newFloat = Number(object.newFloat);
    } else {
      message.newFloat = 0;
    }
    if (object.oldUint32 !== undefined && object.oldUint32 !== null) {
      message.oldUint32 = Number(object.oldUint32);
    } else {
      message.oldUint32 = 0;
    }
    if (object.newUint32 !== undefined && object.newUint32 !== null) {
      message.newUint32 = Number(object.newUint32);
    } else {
      message.newUint32 = 0;
    }
    if (object.oldBool !== undefined && object.oldBool !== null) {
      message.oldBool = Boolean(object.oldBool);
    } else {
      message.oldBool = false;
    }
    if (object.newBool !== undefined && object.newBool !== null) {
      message.newBool = Boolean(object.newBool);
    } else {
      message.newBool = false;
    }
    if (object.oldRange !== undefined && object.oldRange !== null) {
      message.oldRange = Range.fromJSON(object.oldRange);
    } else {
      message.oldRange = undefined;
    }
    if (object.newRange !== undefined && object.newRange !== null) {
      message.newRange = Range.fromJSON(object.newRange);
    } else {
      message.newRange = undefined;
    }
    if (object.oldMode !== undefined && object.oldMode !== null) {
      message.oldMode = ventilationModeFromJSON(object.oldMode);
    } else {
      message.oldMode = 0;
    }
    if (object.newMode !== undefined && object.newMode !== null) {
      message.newMode = ventilationModeFromJSON(object.newMode);
    } else {
      message.newMode = 0;
    }
    return message;
  },

  toJSON(message: LogEvent): unknown {
    const obj: any = {};
    message.id !== undefined && (obj.id = message.id);
    message.time !== undefined && (obj.time = message.time);
    message.code !== undefined && (obj.code = logEventCodeToJSON(message.code));
    message.type !== undefined && (obj.type = logEventTypeToJSON(message.type));
    message.alarmLimits !== undefined &&
      (obj.alarmLimits = message.alarmLimits
        ? Range.toJSON(message.alarmLimits)
        : undefined);
    message.oldFloat !== undefined && (obj.oldFloat = message.oldFloat);
    message.newFloat !== undefined && (obj.newFloat = message.newFloat);
    message.oldUint32 !== undefined && (obj.oldUint32 = message.oldUint32);
    message.newUint32 !== undefined && (obj.newUint32 = message.newUint32);
    message.oldBool !== undefined && (obj.oldBool = message.oldBool);
    message.newBool !== undefined && (obj.newBool = message.newBool);
    message.oldRange !== undefined &&
      (obj.oldRange = message.oldRange
        ? Range.toJSON(message.oldRange)
        : undefined);
    message.newRange !== undefined &&
      (obj.newRange = message.newRange
        ? Range.toJSON(message.newRange)
        : undefined);
    message.oldMode !== undefined &&
      (obj.oldMode = ventilationModeToJSON(message.oldMode));
    message.newMode !== undefined &&
      (obj.newMode = ventilationModeToJSON(message.newMode));
    return obj;
  },

  fromPartial(object: DeepPartial<LogEvent>): LogEvent {
    const message = { ...baseLogEvent } as LogEvent;
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
    if (object.type !== undefined && object.type !== null) {
      message.type = object.type;
    } else {
      message.type = 0;
    }
    if (object.alarmLimits !== undefined && object.alarmLimits !== null) {
      message.alarmLimits = Range.fromPartial(object.alarmLimits);
    } else {
      message.alarmLimits = undefined;
    }
    if (object.oldFloat !== undefined && object.oldFloat !== null) {
      message.oldFloat = object.oldFloat;
    } else {
      message.oldFloat = 0;
    }
    if (object.newFloat !== undefined && object.newFloat !== null) {
      message.newFloat = object.newFloat;
    } else {
      message.newFloat = 0;
    }
    if (object.oldUint32 !== undefined && object.oldUint32 !== null) {
      message.oldUint32 = object.oldUint32;
    } else {
      message.oldUint32 = 0;
    }
    if (object.newUint32 !== undefined && object.newUint32 !== null) {
      message.newUint32 = object.newUint32;
    } else {
      message.newUint32 = 0;
    }
    if (object.oldBool !== undefined && object.oldBool !== null) {
      message.oldBool = object.oldBool;
    } else {
      message.oldBool = false;
    }
    if (object.newBool !== undefined && object.newBool !== null) {
      message.newBool = object.newBool;
    } else {
      message.newBool = false;
    }
    if (object.oldRange !== undefined && object.oldRange !== null) {
      message.oldRange = Range.fromPartial(object.oldRange);
    } else {
      message.oldRange = undefined;
    }
    if (object.newRange !== undefined && object.newRange !== null) {
      message.newRange = Range.fromPartial(object.newRange);
    } else {
      message.newRange = undefined;
    }
    if (object.oldMode !== undefined && object.oldMode !== null) {
      message.oldMode = object.oldMode;
    } else {
      message.oldMode = 0;
    }
    if (object.newMode !== undefined && object.newMode !== null) {
      message.newMode = object.newMode;
    } else {
      message.newMode = 0;
    }
    return message;
  },
};

const baseExpectedLogEvent: object = { id: 0, sessionId: 0 };

export const ExpectedLogEvent = {
  encode(
    message: ExpectedLogEvent,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.id !== 0) {
      writer.uint32(8).uint32(message.id);
    }
    if (message.sessionId !== 0) {
      writer.uint32(16).uint32(message.sessionId);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): ExpectedLogEvent {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseExpectedLogEvent } as ExpectedLogEvent;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.id = reader.uint32();
          break;
        case 2:
          message.sessionId = reader.uint32();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): ExpectedLogEvent {
    const message = { ...baseExpectedLogEvent } as ExpectedLogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = Number(object.id);
    } else {
      message.id = 0;
    }
    if (object.sessionId !== undefined && object.sessionId !== null) {
      message.sessionId = Number(object.sessionId);
    } else {
      message.sessionId = 0;
    }
    return message;
  },

  toJSON(message: ExpectedLogEvent): unknown {
    const obj: any = {};
    message.id !== undefined && (obj.id = message.id);
    message.sessionId !== undefined && (obj.sessionId = message.sessionId);
    return obj;
  },

  fromPartial(object: DeepPartial<ExpectedLogEvent>): ExpectedLogEvent {
    const message = { ...baseExpectedLogEvent } as ExpectedLogEvent;
    if (object.id !== undefined && object.id !== null) {
      message.id = object.id;
    } else {
      message.id = 0;
    }
    if (object.sessionId !== undefined && object.sessionId !== null) {
      message.sessionId = object.sessionId;
    } else {
      message.sessionId = 0;
    }
    return message;
  },
};

const baseNextLogEvents: object = {
  nextExpected: 0,
  total: 0,
  remaining: 0,
  sessionId: 0,
};

export const NextLogEvents = {
  encode(
    message: NextLogEvents,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.nextExpected !== 0) {
      writer.uint32(8).uint32(message.nextExpected);
    }
    if (message.total !== 0) {
      writer.uint32(16).uint32(message.total);
    }
    if (message.remaining !== 0) {
      writer.uint32(24).uint32(message.remaining);
    }
    if (message.sessionId !== 0) {
      writer.uint32(32).uint32(message.sessionId);
    }
    for (const v of message.elements) {
      LogEvent.encode(v!, writer.uint32(42).fork()).ldelim();
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): NextLogEvents {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseNextLogEvents } as NextLogEvents;
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
          message.sessionId = reader.uint32();
          break;
        case 5:
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
    const message = { ...baseNextLogEvents } as NextLogEvents;
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
    if (object.sessionId !== undefined && object.sessionId !== null) {
      message.sessionId = Number(object.sessionId);
    } else {
      message.sessionId = 0;
    }
    if (object.elements !== undefined && object.elements !== null) {
      for (const e of object.elements) {
        message.elements.push(LogEvent.fromJSON(e));
      }
    }
    return message;
  },

  toJSON(message: NextLogEvents): unknown {
    const obj: any = {};
    message.nextExpected !== undefined &&
      (obj.nextExpected = message.nextExpected);
    message.total !== undefined && (obj.total = message.total);
    message.remaining !== undefined && (obj.remaining = message.remaining);
    message.sessionId !== undefined && (obj.sessionId = message.sessionId);
    if (message.elements) {
      obj.elements = message.elements.map((e) =>
        e ? LogEvent.toJSON(e) : undefined
      );
    } else {
      obj.elements = [];
    }
    return obj;
  },

  fromPartial(object: DeepPartial<NextLogEvents>): NextLogEvents {
    const message = { ...baseNextLogEvents } as NextLogEvents;
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
    if (object.sessionId !== undefined && object.sessionId !== null) {
      message.sessionId = object.sessionId;
    } else {
      message.sessionId = 0;
    }
    if (object.elements !== undefined && object.elements !== null) {
      for (const e of object.elements) {
        message.elements.push(LogEvent.fromPartial(e));
      }
    }
    return message;
  },
};

const baseActiveLogEvents: object = { id: 0 };

export const ActiveLogEvents = {
  encode(
    message: ActiveLogEvents,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    writer.uint32(10).fork();
    for (const v of message.id) {
      writer.uint32(v);
    }
    writer.ldelim();
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): ActiveLogEvents {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseActiveLogEvents } as ActiveLogEvents;
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
    const message = { ...baseActiveLogEvents } as ActiveLogEvents;
    message.id = [];
    if (object.id !== undefined && object.id !== null) {
      for (const e of object.id) {
        message.id.push(Number(e));
      }
    }
    return message;
  },

  toJSON(message: ActiveLogEvents): unknown {
    const obj: any = {};
    if (message.id) {
      obj.id = message.id.map((e) => e);
    } else {
      obj.id = [];
    }
    return obj;
  },

  fromPartial(object: DeepPartial<ActiveLogEvents>): ActiveLogEvents {
    const message = { ...baseActiveLogEvents } as ActiveLogEvents;
    message.id = [];
    if (object.id !== undefined && object.id !== null) {
      for (const e of object.id) {
        message.id.push(e);
      }
    }
    return message;
  },
};

const baseAlarmMute: object = {
  active: false,
  seqNum: 0,
  source: 0,
  remaining: 0,
};

export const AlarmMute = {
  encode(
    message: AlarmMute,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.active === true) {
      writer.uint32(8).bool(message.active);
    }
    if (message.seqNum !== 0) {
      writer.uint32(16).uint32(message.seqNum);
    }
    if (message.source !== 0) {
      writer.uint32(24).int32(message.source);
    }
    if (message.remaining !== 0) {
      writer.uint32(32).uint64(message.remaining);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): AlarmMute {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseAlarmMute } as AlarmMute;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.active = reader.bool();
          break;
        case 2:
          message.seqNum = reader.uint32();
          break;
        case 3:
          message.source = reader.int32() as any;
          break;
        case 4:
          message.remaining = longToNumber(reader.uint64() as Long);
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): AlarmMute {
    const message = { ...baseAlarmMute } as AlarmMute;
    if (object.active !== undefined && object.active !== null) {
      message.active = Boolean(object.active);
    } else {
      message.active = false;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = Number(object.seqNum);
    } else {
      message.seqNum = 0;
    }
    if (object.source !== undefined && object.source !== null) {
      message.source = alarmMuteSourceFromJSON(object.source);
    } else {
      message.source = 0;
    }
    if (object.remaining !== undefined && object.remaining !== null) {
      message.remaining = Number(object.remaining);
    } else {
      message.remaining = 0;
    }
    return message;
  },

  toJSON(message: AlarmMute): unknown {
    const obj: any = {};
    message.active !== undefined && (obj.active = message.active);
    message.seqNum !== undefined && (obj.seqNum = message.seqNum);
    message.source !== undefined &&
      (obj.source = alarmMuteSourceToJSON(message.source));
    message.remaining !== undefined && (obj.remaining = message.remaining);
    return obj;
  },

  fromPartial(object: DeepPartial<AlarmMute>): AlarmMute {
    const message = { ...baseAlarmMute } as AlarmMute;
    if (object.active !== undefined && object.active !== null) {
      message.active = object.active;
    } else {
      message.active = false;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = object.seqNum;
    } else {
      message.seqNum = 0;
    }
    if (object.source !== undefined && object.source !== null) {
      message.source = object.source;
    } else {
      message.source = 0;
    }
    if (object.remaining !== undefined && object.remaining !== null) {
      message.remaining = object.remaining;
    } else {
      message.remaining = 0;
    }
    return message;
  },
};

const baseAlarmMuteRequest: object = { active: false, seqNum: 0, source: 0 };

export const AlarmMuteRequest = {
  encode(
    message: AlarmMuteRequest,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.active === true) {
      writer.uint32(8).bool(message.active);
    }
    if (message.seqNum !== 0) {
      writer.uint32(16).uint32(message.seqNum);
    }
    if (message.source !== 0) {
      writer.uint32(24).int32(message.source);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): AlarmMuteRequest {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseAlarmMuteRequest } as AlarmMuteRequest;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.active = reader.bool();
          break;
        case 2:
          message.seqNum = reader.uint32();
          break;
        case 3:
          message.source = reader.int32() as any;
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): AlarmMuteRequest {
    const message = { ...baseAlarmMuteRequest } as AlarmMuteRequest;
    if (object.active !== undefined && object.active !== null) {
      message.active = Boolean(object.active);
    } else {
      message.active = false;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = Number(object.seqNum);
    } else {
      message.seqNum = 0;
    }
    if (object.source !== undefined && object.source !== null) {
      message.source = alarmMuteSourceFromJSON(object.source);
    } else {
      message.source = 0;
    }
    return message;
  },

  toJSON(message: AlarmMuteRequest): unknown {
    const obj: any = {};
    message.active !== undefined && (obj.active = message.active);
    message.seqNum !== undefined && (obj.seqNum = message.seqNum);
    message.source !== undefined &&
      (obj.source = alarmMuteSourceToJSON(message.source));
    return obj;
  },

  fromPartial(object: DeepPartial<AlarmMuteRequest>): AlarmMuteRequest {
    const message = { ...baseAlarmMuteRequest } as AlarmMuteRequest;
    if (object.active !== undefined && object.active !== null) {
      message.active = object.active;
    } else {
      message.active = false;
    }
    if (object.seqNum !== undefined && object.seqNum !== null) {
      message.seqNum = object.seqNum;
    } else {
      message.seqNum = 0;
    }
    if (object.source !== undefined && object.source !== null) {
      message.source = object.source;
    } else {
      message.source = 0;
    }
    return message;
  },
};

const baseMCUPowerStatus: object = { powerLeft: 0, charging: false };

export const MCUPowerStatus = {
  encode(
    message: MCUPowerStatus,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.powerLeft !== 0) {
      writer.uint32(13).float(message.powerLeft);
    }
    if (message.charging === true) {
      writer.uint32(16).bool(message.charging);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): MCUPowerStatus {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseMCUPowerStatus } as MCUPowerStatus;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.powerLeft = reader.float();
          break;
        case 2:
          message.charging = reader.bool();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): MCUPowerStatus {
    const message = { ...baseMCUPowerStatus } as MCUPowerStatus;
    if (object.powerLeft !== undefined && object.powerLeft !== null) {
      message.powerLeft = Number(object.powerLeft);
    } else {
      message.powerLeft = 0;
    }
    if (object.charging !== undefined && object.charging !== null) {
      message.charging = Boolean(object.charging);
    } else {
      message.charging = false;
    }
    return message;
  },

  toJSON(message: MCUPowerStatus): unknown {
    const obj: any = {};
    message.powerLeft !== undefined && (obj.powerLeft = message.powerLeft);
    message.charging !== undefined && (obj.charging = message.charging);
    return obj;
  },

  fromPartial(object: DeepPartial<MCUPowerStatus>): MCUPowerStatus {
    const message = { ...baseMCUPowerStatus } as MCUPowerStatus;
    if (object.powerLeft !== undefined && object.powerLeft !== null) {
      message.powerLeft = object.powerLeft;
    } else {
      message.powerLeft = 0;
    }
    if (object.charging !== undefined && object.charging !== null) {
      message.charging = object.charging;
    } else {
      message.charging = false;
    }
    return message;
  },
};

const baseBackendConnections: object = { hasMcu: false, hasFrontend: false };

export const BackendConnections = {
  encode(
    message: BackendConnections,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.hasMcu === true) {
      writer.uint32(8).bool(message.hasMcu);
    }
    if (message.hasFrontend === true) {
      writer.uint32(16).bool(message.hasFrontend);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): BackendConnections {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseBackendConnections } as BackendConnections;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.hasMcu = reader.bool();
          break;
        case 2:
          message.hasFrontend = reader.bool();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): BackendConnections {
    const message = { ...baseBackendConnections } as BackendConnections;
    if (object.hasMcu !== undefined && object.hasMcu !== null) {
      message.hasMcu = Boolean(object.hasMcu);
    } else {
      message.hasMcu = false;
    }
    if (object.hasFrontend !== undefined && object.hasFrontend !== null) {
      message.hasFrontend = Boolean(object.hasFrontend);
    } else {
      message.hasFrontend = false;
    }
    return message;
  },

  toJSON(message: BackendConnections): unknown {
    const obj: any = {};
    message.hasMcu !== undefined && (obj.hasMcu = message.hasMcu);
    message.hasFrontend !== undefined &&
      (obj.hasFrontend = message.hasFrontend);
    return obj;
  },

  fromPartial(object: DeepPartial<BackendConnections>): BackendConnections {
    const message = { ...baseBackendConnections } as BackendConnections;
    if (object.hasMcu !== undefined && object.hasMcu !== null) {
      message.hasMcu = object.hasMcu;
    } else {
      message.hasMcu = false;
    }
    if (object.hasFrontend !== undefined && object.hasFrontend !== null) {
      message.hasFrontend = object.hasFrontend;
    } else {
      message.hasFrontend = false;
    }
    return message;
  },
};

const baseScreenStatus: object = { lock: false };

export const ScreenStatus = {
  encode(
    message: ScreenStatus,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.lock === true) {
      writer.uint32(8).bool(message.lock);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): ScreenStatus {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseScreenStatus } as ScreenStatus;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.lock = reader.bool();
          break;
        default:
          reader.skipType(tag & 7);
          break;
      }
    }
    return message;
  },

  fromJSON(object: any): ScreenStatus {
    const message = { ...baseScreenStatus } as ScreenStatus;
    if (object.lock !== undefined && object.lock !== null) {
      message.lock = Boolean(object.lock);
    } else {
      message.lock = false;
    }
    return message;
  },

  toJSON(message: ScreenStatus): unknown {
    const obj: any = {};
    message.lock !== undefined && (obj.lock = message.lock);
    return obj;
  },

  fromPartial(object: DeepPartial<ScreenStatus>): ScreenStatus {
    const message = { ...baseScreenStatus } as ScreenStatus;
    if (object.lock !== undefined && object.lock !== null) {
      message.lock = object.lock;
    } else {
      message.lock = false;
    }
    return message;
  },
};

const basePing: object = { time: 0, id: 0 };

export const Ping = {
  encode(message: Ping, writer: _m0.Writer = _m0.Writer.create()): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.id !== 0) {
      writer.uint32(16).uint32(message.id);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Ping {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...basePing } as Ping;
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
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
    const message = { ...basePing } as Ping;
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

  toJSON(message: Ping): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.id !== undefined && (obj.id = message.id);
    return obj;
  },

  fromPartial(object: DeepPartial<Ping>): Ping {
    const message = { ...basePing } as Ping;
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
};

const baseAnnouncement: object = { time: 0 };

export const Announcement = {
  encode(
    message: Announcement,
    writer: _m0.Writer = _m0.Writer.create()
  ): _m0.Writer {
    if (message.time !== 0) {
      writer.uint32(8).uint64(message.time);
    }
    if (message.announcement.length !== 0) {
      writer.uint32(18).bytes(message.announcement);
    }
    return writer;
  },

  decode(input: _m0.Reader | Uint8Array, length?: number): Announcement {
    const reader = input instanceof _m0.Reader ? input : new _m0.Reader(input);
    let end = length === undefined ? reader.len : reader.pos + length;
    const message = { ...baseAnnouncement } as Announcement;
    message.announcement = new Uint8Array();
    while (reader.pos < end) {
      const tag = reader.uint32();
      switch (tag >>> 3) {
        case 1:
          message.time = longToNumber(reader.uint64() as Long);
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
    const message = { ...baseAnnouncement } as Announcement;
    message.announcement = new Uint8Array();
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

  toJSON(message: Announcement): unknown {
    const obj: any = {};
    message.time !== undefined && (obj.time = message.time);
    message.announcement !== undefined &&
      (obj.announcement = base64FromBytes(
        message.announcement !== undefined
          ? message.announcement
          : new Uint8Array()
      ));
    return obj;
  },

  fromPartial(object: DeepPartial<Announcement>): Announcement {
    const message = { ...baseAnnouncement } as Announcement;
    if (object.time !== undefined && object.time !== null) {
      message.time = object.time;
    } else {
      message.time = 0;
    }
    if (object.announcement !== undefined && object.announcement !== null) {
      message.announcement = object.announcement;
    } else {
      message.announcement = new Uint8Array();
    }
    return message;
  },
};

declare var self: any | undefined;
declare var window: any | undefined;
var globalThis: any = (() => {
  if (typeof globalThis !== "undefined") return globalThis;
  if (typeof self !== "undefined") return self;
  if (typeof window !== "undefined") return window;
  if (typeof global !== "undefined") return global;
  throw "Unable to locate global object";
})();

const atob: (b64: string) => string =
  globalThis.atob ||
  ((b64) => globalThis.Buffer.from(b64, "base64").toString("binary"));
function bytesFromBase64(b64: string): Uint8Array {
  const bin = atob(b64);
  const arr = new Uint8Array(bin.length);
  for (let i = 0; i < bin.length; ++i) {
    arr[i] = bin.charCodeAt(i);
  }
  return arr;
}

const btoa: (bin: string) => string =
  globalThis.btoa ||
  ((bin) => globalThis.Buffer.from(bin, "binary").toString("base64"));
function base64FromBytes(arr: Uint8Array): string {
  const bin: string[] = [];
  for (let i = 0; i < arr.byteLength; ++i) {
    bin.push(String.fromCharCode(arr[i]));
  }
  return btoa(bin.join(""));
}

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

function longToNumber(long: Long): number {
  if (long.gt(Number.MAX_SAFE_INTEGER)) {
    throw new globalThis.Error("Value is larger than Number.MAX_SAFE_INTEGER");
  }
  return long.toNumber();
}

if (_m0.util.Long !== Long) {
  _m0.util.Long = Long as any;
  _m0.configure();
}
