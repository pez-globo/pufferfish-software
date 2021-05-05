import {
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  AlarmLimits,
  AlarmLimitsRequest,
  ActiveLogEvents,
  ExpectedLogEvent,
  NextLogEvents,
  BatteryPower,
  ScreenStatus,
  AlarmMuteRequest,
  AlarmMute,
} from './proto/mcu_pb';
import { SystemSettingRequest, FrontendDisplaySetting, RotaryEncoder } from './proto/frontend_pb';

// MESSAGES

export type PBMessage =
  // mcu_pb
  | SensorMeasurements
  | CycleMeasurements
  | Parameters
  | ParametersRequest
  | AlarmLimits
  | AlarmLimitsRequest
  | ExpectedLogEvent
  | NextLogEvents
  | ActiveLogEvents
  | AlarmMuteRequest
  | AlarmMute
  // frontend_pb
  | BatteryPower
  | ScreenStatus
  | SystemSettingRequest
  | FrontendDisplaySetting
  | RotaryEncoder;

export type PBMessageType =
  // mcu_pb
  | typeof SensorMeasurements
  | typeof CycleMeasurements
  | typeof Parameters
  | typeof ParametersRequest
  | typeof AlarmLimits
  | typeof AlarmLimitsRequest
  | typeof ExpectedLogEvent
  | typeof NextLogEvents
  | typeof ActiveLogEvents
  | typeof AlarmMuteRequest
  | typeof AlarmMute
  // frontend_pb
  | typeof BatteryPower
  | typeof ScreenStatus
  | typeof SystemSettingRequest
  | typeof FrontendDisplaySetting
  | typeof RotaryEncoder;

export enum MessageType {
  // mcu_pb
  SensorMeasurements = 2,
  CycleMeasurements = 3,
  Parameters = 4,
  ParametersRequest = 5,
  AlarmLimits = 6,
  AlarmLimitsRequest = 7,
  ExpectedLogEvent = 8,
  NextLogEvents = 9,
  ActiveLogEvents = 10,
  AlarmMuteRequest = 11,
  AlarmMute = 12,
  BatteryPower = 64,
  // frontend_pb
  ScreenStatus = 65,
  RotaryEncoder = 128,
  SystemSettingRequest = 129,
  FrontendDisplaySetting = 130,
}

export const MessageClass = new Map<MessageType, PBMessageType>([
  // mcu_pb
  [MessageType.SensorMeasurements, SensorMeasurements],
  [MessageType.CycleMeasurements, CycleMeasurements],
  [MessageType.Parameters, Parameters],
  [MessageType.ParametersRequest, ParametersRequest],
  [MessageType.AlarmLimits, AlarmLimits],
  [MessageType.AlarmLimitsRequest, AlarmLimitsRequest],
  [MessageType.ExpectedLogEvent, ExpectedLogEvent],
  [MessageType.NextLogEvents, NextLogEvents],
  [MessageType.ActiveLogEvents, ActiveLogEvents],
  [MessageType.AlarmMuteRequest, AlarmMuteRequest],
  [MessageType.AlarmMute, AlarmMute],
  // frontend_pb
  [MessageType.BatteryPower, BatteryPower],
  [MessageType.ScreenStatus, ScreenStatus],
  [MessageType.SystemSettingRequest, SystemSettingRequest],
  [MessageType.FrontendDisplaySetting, FrontendDisplaySetting],
  [MessageType.RotaryEncoder, RotaryEncoder],
]);

export const MessageTypes = new Map<PBMessageType, MessageType>([
  // mcu_pb
  [SensorMeasurements, MessageType.SensorMeasurements],
  [CycleMeasurements, MessageType.CycleMeasurements],
  [Parameters, MessageType.Parameters],
  [ParametersRequest, MessageType.ParametersRequest],
  [AlarmLimits, MessageType.AlarmLimits],
  [AlarmLimitsRequest, MessageType.AlarmLimitsRequest],
  [ExpectedLogEvent, MessageType.ExpectedLogEvent],
  [NextLogEvents, MessageType.NextLogEvents],
  [ActiveLogEvents, MessageType.ActiveLogEvents],
  [AlarmMuteRequest, MessageType.AlarmMuteRequest],
  [AlarmMute, MessageType.AlarmMute],
  // frontend_pb
  [BatteryPower, MessageType.BatteryPower],
  [ScreenStatus, MessageType.ScreenStatus],
  [SystemSettingRequest, MessageType.SystemSettingRequest],
  [FrontendDisplaySetting, MessageType.FrontendDisplaySetting],
  [RotaryEncoder, MessageType.RotaryEncoder],
]);

// STATES

export interface Measurements {
  sensor: SensorMeasurements | null;
  cycle: CycleMeasurements | null;
}
export interface ParametersRequestResponse {
  current: Parameters | null;
  request: ParametersRequest | null;
  // standby is a scratchpad for modifying individual fields of ParametersRequest
  // across different screens in standby mode, before committing the final values
  // of all fields to ParametersRequest to send to the backend
  standby: ParametersRequest | null;
}
export interface AlarmLimitsRequestResponse {
  current: AlarmLimits | null;
  request: AlarmLimitsRequest | null;
  // standby is a scratchpad for modifying individual fields of AlarmLimitsRequest
  // across different screens in standby mode, before committing the final values
  // of all fields to AlarmLimitsRequest to send to the backend
  standby: AlarmLimitsRequest | null;
}
export interface EventLog {
  expectedLogEvent: ExpectedLogEvent;
  nextLogEvents: NextLogEvents;
  activeLogEvents: ActiveLogEvents;
  ephemeralLogEvents: {
    id: number[];
  };
}

export interface RotaryEncoderParameter {
  step: number;
  lastStepChange: number;
  buttonPressed: boolean;
  lastButtonDown: number;
  lastButtonUp: number;
  stepDiff: number; // this is a derived value not in RotaryEncoder
}

// Smoothed measurements

export interface SmoothingData {
  raw: number;
  average: number;
  converged: number;
  smoothed: number;
  time?: number;
  convergenceStartTime?: number;
  changeStartTime?: number;
}
export interface SmoothedMeasurements {
  fio2: SmoothingData;
  flow: SmoothingData;
  spo2: SmoothingData;
  hr: SmoothingData;
}

// Plots

// Waveform histories
export interface WaveformPoint {
  date: number;
  value: number;
}
export interface WaveformHistory {
  waveformOld: {
    full: WaveformPoint[];
  };
  waveformNew: {
    full: WaveformPoint[];
    buffer: WaveformPoint[];
    segmented: WaveformPoint[][];
  };
  waveformNewStart: number;
}
export interface WaveformHistories {
  paw: WaveformHistory;
  flow: WaveformHistory;
  volume: WaveformHistory;
}
// P-V loops
export interface PVPoint {
  pressure: number;
  volume: number;
}
export interface PVHistory {
  loop: PVPoint[];
  loopOrigin: PVPoint;
  cycle: number;
}
// All plots
export interface Plots {
  waveforms: WaveformHistories;
  pvLoop: PVHistory;
}

// All states

export interface ControllerStates {
  // Message states from mcu_pb
  measurements: Measurements;
  parameters: ParametersRequestResponse;
  alarmLimits: AlarmLimitsRequestResponse;
  eventLog: EventLog;
  alarmMuteRequest: AlarmMuteRequest | null;
  alarmMute: AlarmMute | null;
  systemSettingRequest: SystemSettingRequest | null;
  frontendDisplaySetting: FrontendDisplaySetting | null;
  batteryPower: BatteryPower | null;
  screenStatus: ScreenStatus | null;
  heartbeatBackend: { time: Date };

  // Message states from frontend_pb
  rotaryEncoder: RotaryEncoderParameter | null;

  // Derived states
  smoothedMeasurements: SmoothedMeasurements;
  plots: Plots;
}

// ACTIONS

// Action Types

// TODO: rename STATE_UPDATED to STATE_MESSAGE_RECEIVED
export const STATE_UPDATED = '@controller/STATE_UPDATED';
export const REQUEST_COMMITTED = '@controller/REQUEST_COMMITTED';
export const STANDBY_REQUEST_COMMITTED = '@controller/STANDBY_COMMITTED';
// TODO: find a better place to define BACKEND_CONNECTION_LOST_CODE, as it's not an action
export const BACKEND_CONNECTION_LOST_CODE = 101;

// State Update Action

// TODO: rename to StateMessageReceived
interface StateUpdatedAction {
  type: typeof STATE_UPDATED;
  messageType: MessageType;
  state: PBMessage;
}

export type StateUpdateAction = StateUpdatedAction;

// State Commit Actions

// TODO: rename to SettingCommitted
export interface CommitAction {
  type: string;
  messageType: MessageType;
  update: Record<string, unknown>;
}
