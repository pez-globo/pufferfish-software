import { combineReducers } from 'redux';
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
  // Alarm Muting
  AlarmMute,
  AlarmMuteRequest,
  // System Miscellaneous
  MCUPowerStatus,
  BackendConnections,
  ScreenStatus,
} from '../proto/mcu_pb';
import { SystemSettingRequest, FrontendDisplaySetting } from '../proto/frontend_pb';
import { MessageType } from '../proto/types';
import {
  messageReducer,
  requestReducer,
  eventLogReducer,
  rotaryEncoderReducer,
} from './reducers/backend';
import { waveformHistoryReducer, pvHistoryReducer } from './reducers/derived';
import { REQUEST_COMMITTED, DRAFT_REQUEST_COMMITTED } from './types';

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  measurements: combineReducers({
    sensor: messageReducer<SensorMeasurements>(MessageType.SensorMeasurements),
    cycle: messageReducer<CycleMeasurements>(MessageType.CycleMeasurements),
  }),
  parameters: combineReducers({
    current: messageReducer<Parameters>(MessageType.Parameters),
    request: requestReducer<ParametersRequest>(MessageType.ParametersRequest, REQUEST_COMMITTED),
    draft: requestReducer<ParametersRequest>(
      MessageType.ParametersRequest,
      DRAFT_REQUEST_COMMITTED,
    ),
  }),
  alarmLimits: combineReducers({
    current: messageReducer<AlarmLimits>(MessageType.AlarmLimits),
    request: requestReducer<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, REQUEST_COMMITTED),
    draft: requestReducer<AlarmLimitsRequest>(
      MessageType.AlarmLimitsRequest,
      DRAFT_REQUEST_COMMITTED,
    ),
  }),
  eventLog: eventLogReducer,
  alarmMute: combineReducers({
    current: messageReducer<AlarmMute>(MessageType.AlarmMute),
    request: requestReducer<AlarmMuteRequest>(MessageType.AlarmMuteRequest, REQUEST_COMMITTED),
  }),
  mcuPowerStatus: messageReducer<MCUPowerStatus>(MessageType.MCUPowerStatus),
  backendConnections: messageReducer<BackendConnections>(MessageType.BackendConnections),
  screenStatus: messageReducer<ScreenStatus>(MessageType.ScreenStatus),

  // Message states from frontend_pb
  rotaryEncoder: rotaryEncoderReducer,
  systemSettingRequest: requestReducer<SystemSettingRequest>(
    MessageType.SystemSettingRequest,
    REQUEST_COMMITTED,
  ),
  frontendDisplaySetting: requestReducer<FrontendDisplaySetting>(
    MessageType.FrontendDisplaySetting,
    REQUEST_COMMITTED,
  ),

  // Derived states
  // TODO: move plots into a separate top-level slice of the store
  plots: combineReducers({
    waveforms: combineReducers({
      paw: waveformHistoryReducer<SensorMeasurements>(
        MessageType.SensorMeasurements,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.paw,
        60,
        0,
      ),
      flow: waveformHistoryReducer<SensorMeasurements>(
        MessageType.SensorMeasurements,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.flow,
        60,
        20,
      ),
      volume: waveformHistoryReducer<SensorMeasurements>(
        MessageType.SensorMeasurements,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
        (sensorMeasurements: SensorMeasurements) => sensorMeasurements.volume,
        60,
        40,
      ),
    }),
    pvLoop: pvHistoryReducer,
  }),
});

export default controllerReducer;
