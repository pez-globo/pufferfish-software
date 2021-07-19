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
  ExpectedLogEvent,
  NextLogEvents,
  ActiveLogEvents,
  // Alarm Muting
  AlarmMute,
  AlarmMuteRequest,
  // System Miscellaneous
  MCUPowerStatus,
  BackendConnections,
  ScreenStatus,
} from '../proto/mcu_pb';
import { SystemSettingRequest, FrontendDisplaySetting } from '../proto/frontend_pb';
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

export interface RotaryEncoderParameter {
  step: number;
  lastStepChange: number;
  buttonPressed: boolean;
  lastButtonDown: number;
  lastButtonUp: number;
  stepDiff: number; // this is a derived value not in RotaryEncoder
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
  screenStatus: ScreenStatus | null;

  // Message states from frontend_pb
  rotaryEncoder: RotaryEncoderParameter | null;
  systemSettingRequest: SystemSettingRequest | null;
  frontendDisplaySetting: FrontendDisplaySetting | null;

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
