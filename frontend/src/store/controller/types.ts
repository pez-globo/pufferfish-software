import {
  Alarms,
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  Ping,
  Announcement,
  AlarmLimitsRequest
} from './proto/mcu_pb'
import {
  RotaryEncoder
} from './proto/frontend_pb'

// Action Types

export const STATE_UPDATED = '@controller/STATE_UPDATED'
export const PARAMETER_COMMITTED = '@controller/PARAMETER_COMMITTED'
export const THEME_SWITCHED = '@controller/THEME_SWITCHED'

// Protocol Buffers

export type PBMessage = (
  // mcu_pb
  AlarmLimitsRequest | Alarms | SensorMeasurements | CycleMeasurements
  | Parameters | ParametersRequest
  | Ping | Announcement
  // frontend_pb
  | RotaryEncoder
)

export type PBMessageType = (
  // mcu_pb
  typeof AlarmLimitsRequest | typeof Alarms | typeof SensorMeasurements | typeof CycleMeasurements
  | typeof Parameters | typeof ParametersRequest
  | typeof Ping | typeof Announcement
  // frontend_pb
  | typeof RotaryEncoder
)

export enum MessageType {
  Alarms = 1,
  SensorMeasurements = 2,
  CycleMeasurements = 3,
  Parameters = 4,
  ParametersRequest = 5,
  Ping = 6,
  Announcement = 7,
  AlarmLimitsRequest = 8,
  RotaryEncoder = 128
};

export enum ThemeVariant {
  DARK = 1,
  LIGHT = 2
}

// States

export interface WaveformPoint {
  date: Date,
  value: number
}

export interface WaveformHistory {
  waveformOld: WaveformPoint[],
  waveformNew: WaveformPoint[],
  waveformNewStart: number
}

export interface ControllerStates {
  // Message states from mcu_pb
  alarms: Alarms;
  alarmLimitsRequest: AlarmLimitsRequest;
  sensorMeasurements: SensorMeasurements;
  cycleMeasurements: CycleMeasurements;
  parameters: Parameters;
  parametersRequest: ParametersRequest;
  ping: Ping;
  announcement: Announcement;

  // Message states from frontend_pb
  rotaryEncoder: RotaryEncoder;

  // Derived states
  waveformHistoryPaw: WaveformHistory;
  waveformHistoryFlow: WaveformHistory;
  clock: Date
  theme: ThemeVariant
};

export const MessageClass = new Map<MessageType, PBMessageType>([
  [MessageType.Alarms, Alarms],
  [MessageType.AlarmLimitsRequest, AlarmLimitsRequest],
  [MessageType.SensorMeasurements, SensorMeasurements],
  [MessageType.CycleMeasurements, CycleMeasurements],
  [MessageType.Parameters, Parameters],
  [MessageType.ParametersRequest, ParametersRequest],
  [MessageType.Ping, Ping],
  [MessageType.Announcement, Announcement],
  [MessageType.RotaryEncoder, RotaryEncoder],
]);

export const MessageTypes = new Map<PBMessageType, MessageType>([
  [Alarms, MessageType.Alarms],
  [AlarmLimitsRequest, MessageType.AlarmLimitsRequest],
  [SensorMeasurements, MessageType.SensorMeasurements],
  [CycleMeasurements, MessageType.CycleMeasurements],
  [Parameters, MessageType.Parameters],
  [ParametersRequest, MessageType.ParametersRequest],
  [Ping, MessageType.Ping],
  [Announcement, MessageType.Announcement],
  [RotaryEncoder, MessageType.RotaryEncoder],
]);

// State Update Actions

interface StateUpdatedAction {
  type: typeof STATE_UPDATED
  messageType: MessageType
  state: PBMessage
};

export type StateUpdateAction = StateUpdatedAction;

// Parameter Commit Actions

interface ParameterCommittedAction {
  type: typeof PARAMETER_COMMITTED
  update: any
};

export type ParameterCommitAction = ParameterCommittedAction;

// Switch Theme Action

export interface SwitchThemeAction {
  type: typeof THEME_SWITCHED
  theme: any
};