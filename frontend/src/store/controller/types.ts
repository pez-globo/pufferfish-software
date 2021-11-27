import {
  // Measurements
  SensorMeasurements,
  CycleMeasurements,
  // Parameters
  Parameters,
  ParametersRequest,
  // Alarm Limits
  AlarmLimits,
  AlarmLimitsRequest,
  // Log Events
  LogEventCode,
  LogEventType,
  NextLogEvents,
  ExpectedLogEvent,
  ActiveLogEvents,
  // Alarm Muting
  AlarmMute,
  AlarmMuteRequest,
  // System Miscellaneous
  MCUPowerStatus,
  BackendConnections,
  ScreenStatus,
  ScreenStatusRequest,
} from '../proto/mcu_pb';
import {
  SystemSettings,
  SystemSettingsRequest,
  FrontendDisplaySetting,
} from '../proto/frontend_pb';
import { PBMessage, MessageType } from '../proto/types';

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
export interface EventLog {
  expectedLogEvent: ExpectedLogEvent;
  nextLogEvents: NextLogEvents;
  activeLogEvents: ActiveLogEvents;
  ephemeralLogEvents: {
    id: number[];
  };
}
export interface AlarmMuteRequestResponse {
  current: AlarmMute | null;
  request: AlarmMuteRequest | null;
}
export interface ScreenStatusRequestResponse {
  current: ScreenStatus | null;
  request: ScreenStatusRequest | null;
}

export interface RotaryEncoderParameter {
  step: number;
  lastStepChange: number;
  buttonPressed: boolean;
  lastButtonDown: number;
  lastButtonUp: number;
  stepDiff: number; // this is a derived value not in RotaryEncoder
}
export interface SystemSettingsRequestResponse {
  current: SystemSettings | null;
  request: SystemSettingsRequest | null;
}

// Plots
// TODO: move plots into a separate folder under store

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
  mcuPowerStatus: MCUPowerStatus | null;
  backendConnections: BackendConnections | null;
  screenStatus: ScreenStatusRequestResponse;

  // Message states from frontend_pb
  rotaryEncoder: RotaryEncoderParameter | null;
  systemSettings: SystemSettingsRequestResponse;
  frontendDisplaySetting: FrontendDisplaySetting | null;

  // Derived states
  plots: Plots;

  // Dummy Null value
  dummy: null;
}

// ACTIONS

// Action Types

// TODO: rename STATE_UPDATED to STATE_MESSAGE_RECEIVED
export const STATE_UPDATED = '@controller/STATE_UPDATED';
export const REQUEST_COMMITTED = '@controller/REQUEST_COMMITTED';
export const DRAFT_REQUEST_COMMITTED = '@controller/DRAFT_COMMITTED';
export const EPHEMERAL_LOG_EVENT_CREATED = '@controller/EPHEMERAL_LOG_EVENT_CREATED';

// TODO: rename to StateMessageReceived
interface StateUpdatedAction {
  type: typeof STATE_UPDATED;
  messageType: MessageType;
  state: PBMessage;
}

// TODO: get rid of this type alias
export type StateUpdateAction = StateUpdatedAction;

// TODO: rename to SettingCommitted
export interface CommitAction {
  type: string;
  messageType: MessageType;
  update: Record<string, unknown>;
}

export interface EphemeralLogEventAction {
  type: typeof EPHEMERAL_LOG_EVENT_CREATED;
  code: LogEventCode;
  eventType: LogEventType;
  time: number;
}
