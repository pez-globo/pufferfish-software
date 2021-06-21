import {
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  AlarmLimits,
  AlarmLimitsRequest,
  ExpectedLogEvent,
  NextLogEvents,
  ActiveLogEvents,
  AlarmMute,
  AlarmMuteRequest,
  MCUPowerStatus,
  ScreenStatus,
} from './proto/mcu_pb';
import {
  BackendConnections,
  RotaryEncoder,
  SystemSettingRequest,
  FrontendDisplaySetting,
} from './proto/frontend_pb';

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
  | AlarmMute
  | AlarmMuteRequest
  | MCUPowerStatus
  | ScreenStatus
  // frontend_pb
  | BackendConnections
  | RotaryEncoder
  | SystemSettingRequest
  | FrontendDisplaySetting;

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
  | typeof AlarmMute
  | typeof AlarmMuteRequest
  | typeof MCUPowerStatus
  | typeof ScreenStatus
  // frontend_pb
  | typeof BackendConnections
  | typeof RotaryEncoder
  | typeof SystemSettingRequest
  | typeof FrontendDisplaySetting;

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
  AlarmMute = 11,
  AlarmMuteRequest = 12,
  MCUPowerStatus = 20,
  ScreenStatus = 65,
  // frontend_pb
  BackendConnections = 128,
  RotaryEncoder = 129,
  SystemSetting = 130,
  SystemSettingRequest = 131,
  FrontendDisplaySetting = 132,
  FrontendDisplaySettingRequest = 133,
}

// TODO: should we rename this to MessageClasses?
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
  [MessageType.AlarmMute, AlarmMute],
  [MessageType.AlarmMuteRequest, AlarmMuteRequest],
  [MessageType.MCUPowerStatus, MCUPowerStatus],
  [MessageType.ScreenStatus, ScreenStatus],
  // frontend_pb
  [MessageType.BackendConnections, BackendConnections],
  [MessageType.RotaryEncoder, RotaryEncoder],
  [MessageType.SystemSettingRequest, SystemSettingRequest],
  [MessageType.FrontendDisplaySetting, FrontendDisplaySetting],
]);

// TODO: can we auto-generate this from MessageClass?
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
  [AlarmMute, MessageType.AlarmMute],
  [AlarmMuteRequest, MessageType.AlarmMuteRequest],
  [MCUPowerStatus, MessageType.MCUPowerStatus],
  [ScreenStatus, MessageType.ScreenStatus],
  // frontend_pb
  [BackendConnections, MessageType.BackendConnections],
  [RotaryEncoder, MessageType.RotaryEncoder],
  [SystemSettingRequest, MessageType.SystemSettingRequest],
  [FrontendDisplaySetting, MessageType.FrontendDisplaySetting],
]);

// STATES

export interface Measurements {
  sensor: SensorMeasurements | null;
  cycle: CycleMeasurements | null;
}
export interface ParametersRequestResponse {
  current: Parameters | null;
  request: ParametersRequest | null;
  // draft is a scratchpad for modifying individual fields of ParametersRequest
  // across different screens in standby mode, before committing the final values
  // of all fields to ParametersRequest to send to the backend
  draft: ParametersRequest | null;
}
export interface AlarmLimitsRequestResponse {
  current: AlarmLimits | null;
  request: AlarmLimitsRequest | null;
  // draft is a scratchpad for modifying individual fields of AlarmLimitsRequest
  // across different screens in standby mode, before committing the final values
  // of all fields to AlarmLimitsRequest to send to the backend
  draft: AlarmLimitsRequest | null;
}
export interface AlarmMuteRequestResponse {
  current: AlarmMute | null;
  request: AlarmMuteRequest | null;
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
  alarmMute: AlarmMuteRequestResponse;
  systemSettingRequest: SystemSettingRequest | null;
  frontendDisplaySetting: FrontendDisplaySetting | null;
  mcuPowerStatus: MCUPowerStatus | null;
  screenStatus: ScreenStatus | null;
  heartbeatBackend: { time: Date };

  // Message states from frontend_pb
  backendConnections: BackendConnections | null;
  rotaryEncoder: RotaryEncoderParameter | null;

  // Derived states
  plots: Plots;
}

// ACTIONS

// Action Types

// TODO: rename STATE_UPDATED to STATE_MESSAGE_RECEIVED
export const STATE_UPDATED = '@controller/STATE_UPDATED';
export const REQUEST_COMMITTED = '@controller/REQUEST_COMMITTED';
export const DRAFT_REQUEST_COMMITTED = '@controller/DRAFT_COMMITTED';

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
