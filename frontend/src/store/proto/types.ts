import {
  // Measurements
  SensorMeasurements,
  CycleMeasurements,
  // Parameters
  Parameters,
  ParametersRequest,
  // AlarmLimits
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
  ScreenStatusRequest,
} from './mcu_pb';
import {
  RotaryEncoder,
  SystemSettings,
  SystemSettingsRequest,
  FrontendDisplaySetting,
} from './frontend_pb';

// MESSAGES

export type PBMessage =
  // mcu_pb
  // Measurements
  | SensorMeasurements
  | CycleMeasurements
  // Parameters
  | Parameters
  | ParametersRequest
  // AlarmLimits
  | AlarmLimits
  | AlarmLimitsRequest
  // Log Events
  | ExpectedLogEvent
  | NextLogEvents
  | ActiveLogEvents
  // Alarm Muting
  | AlarmMute
  | AlarmMuteRequest
  // System Miscellaneous
  | MCUPowerStatus
  | BackendConnections
  | ScreenStatus
  | ScreenStatusRequest

  // frontend_pb
  | RotaryEncoder
  | SystemSettings
  | SystemSettingsRequest
  | FrontendDisplaySetting;

export type PBMessageType =
  // mcu_pb
  // Measurements
  | typeof SensorMeasurements
  | typeof CycleMeasurements
  // Parameters
  | typeof Parameters
  | typeof ParametersRequest
  // AlarmLimits
  | typeof AlarmLimits
  | typeof AlarmLimitsRequest
  // Log Events
  | typeof ExpectedLogEvent
  | typeof NextLogEvents
  | typeof ActiveLogEvents
  // Alarm Muting
  | typeof AlarmMute
  | typeof AlarmMuteRequest
  // System Miscellaneous
  | typeof MCUPowerStatus
  | typeof BackendConnections
  | typeof ScreenStatus
  | typeof ScreenStatusRequest

  // frontend_pb
  | typeof RotaryEncoder
  | typeof SystemSettings
  | typeof SystemSettingsRequest
  | typeof FrontendDisplaySetting;

export enum MessageType {
  // mcu_pb
  // Measurements
  SensorMeasurements = 2,
  CycleMeasurements = 3,
  // Parameters
  Parameters = 4,
  ParametersRequest = 5,
  // AlarmLimits
  AlarmLimits = 6,
  AlarmLimitsRequest = 7,
  // Log Events
  ExpectedLogEvent = 8,
  NextLogEvents = 9,
  ActiveLogEvents = 10,
  // Alarm Muting
  AlarmMute = 11,
  AlarmMuteRequest = 12,
  // System Miscellaneous
  MCUPowerStatus = 20,
  BackendConnections = 21,
  ScreenStatus = 22,
  ScreenStatusRequest = 23,

  // frontend_pb
  RotaryEncoder = 129,
  SystemSettings = 130,
  SystemSettingsRequest = 131,
  FrontendDisplaySetting = 132,
  FrontendDisplaySettingRequest = 133,
}

// TODO: should we rename this to MessageClasses?
export const MessageClass = new Map<MessageType, PBMessageType>([
  // mcu_pb
  // Measurements
  [MessageType.SensorMeasurements, SensorMeasurements],
  [MessageType.CycleMeasurements, CycleMeasurements],
  // Parameters
  [MessageType.Parameters, Parameters],
  [MessageType.ParametersRequest, ParametersRequest],
  // AlarmLimits
  [MessageType.AlarmLimits, AlarmLimits],
  [MessageType.AlarmLimitsRequest, AlarmLimitsRequest],
  // Log Events
  [MessageType.ExpectedLogEvent, ExpectedLogEvent],
  [MessageType.NextLogEvents, NextLogEvents],
  [MessageType.ActiveLogEvents, ActiveLogEvents],
  // Alarm Muting
  [MessageType.AlarmMute, AlarmMute],
  [MessageType.AlarmMuteRequest, AlarmMuteRequest],
  // System Miscellaneous
  [MessageType.MCUPowerStatus, MCUPowerStatus],
  [MessageType.BackendConnections, BackendConnections],
  [MessageType.ScreenStatus, ScreenStatus],
  [MessageType.ScreenStatusRequest, ScreenStatusRequest],

  // frontend_pb
  [MessageType.RotaryEncoder, RotaryEncoder],
  [MessageType.SystemSettings, SystemSettings],
  [MessageType.SystemSettingsRequest, SystemSettingsRequest],
  [MessageType.FrontendDisplaySetting, FrontendDisplaySetting],
]);

// TODO: can we auto-generate this from MessageClass?
export const MessageTypes = new Map<PBMessageType, MessageType>([
  // mcu_pb
  // Measurements
  [SensorMeasurements, MessageType.SensorMeasurements],
  [CycleMeasurements, MessageType.CycleMeasurements],
  // Parameters
  [Parameters, MessageType.Parameters],
  [ParametersRequest, MessageType.ParametersRequest],
  // AlarmLimits
  [AlarmLimits, MessageType.AlarmLimits],
  [AlarmLimitsRequest, MessageType.AlarmLimitsRequest],
  // Log Events
  [ExpectedLogEvent, MessageType.ExpectedLogEvent],
  [NextLogEvents, MessageType.NextLogEvents],
  [ActiveLogEvents, MessageType.ActiveLogEvents],
  // Alarm Muting
  [AlarmMute, MessageType.AlarmMute],
  [AlarmMuteRequest, MessageType.AlarmMuteRequest],
  // System Miscellaneous
  [MCUPowerStatus, MessageType.MCUPowerStatus],
  [BackendConnections, MessageType.BackendConnections],
  [ScreenStatus, MessageType.ScreenStatus],
  [ScreenStatusRequest, MessageType.ScreenStatusRequest],

  // frontend_pb
  [RotaryEncoder, MessageType.RotaryEncoder],
  [SystemSettings, MessageType.SystemSettings],
  [SystemSettingsRequest, MessageType.SystemSettingsRequest],
  [FrontendDisplaySetting, MessageType.FrontendDisplaySetting],
]);
