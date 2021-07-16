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
} from './mcu_pb';
import {
  BackendConnections,
  RotaryEncoder,
  SystemSettingRequest,
  FrontendDisplaySetting,
} from './frontend_pb';

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
