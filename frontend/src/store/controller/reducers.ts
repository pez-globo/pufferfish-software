import { combineReducers } from 'redux';
import {
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  AlarmLimits,
  AlarmLimitsRequest,
  AlarmMute,
  AlarmMuteRequest,
  BatteryPower,
  ScreenStatus,
} from './proto/mcu_pb';
import { SystemSettingRequest, FrontendDisplaySetting } from './proto/frontend_pb';
import {
  messageReducer,
  requestReducer,
  eventLogReducer,
  rotaryEncoderReducer,
} from './reducers/backend';
import {
  sensorMeasurementSmoothingReducer,
  waveformHistoryReducer,
  pvHistoryReducer,
} from './reducers/derived';
import { MessageType, REQUEST_COMMITTED, STANDBY_REQUEST_COMMITTED } from './types';

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  measurements: combineReducers({
    sensor: messageReducer<SensorMeasurements>(MessageType.SensorMeasurements),
    cycle: messageReducer<CycleMeasurements>(MessageType.CycleMeasurements),
  }),
  parameters: combineReducers({
    current: messageReducer<Parameters>(MessageType.Parameters),
    request: requestReducer<ParametersRequest>(MessageType.ParametersRequest, REQUEST_COMMITTED),
    standby: requestReducer<ParametersRequest>(
      MessageType.ParametersRequest,
      STANDBY_REQUEST_COMMITTED,
    ),
  }),
  alarmLimits: combineReducers({
    current: messageReducer<AlarmLimits>(MessageType.AlarmLimits),
    request: requestReducer<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, REQUEST_COMMITTED),
    standby: requestReducer<AlarmLimitsRequest>(
      MessageType.AlarmLimitsRequest,
      STANDBY_REQUEST_COMMITTED,
    ),
  }),
  eventLog: eventLogReducer,
  alarmMute: combineReducers({
    current: messageReducer<AlarmMute>(MessageType.AlarmMute),
    request: requestReducer<AlarmMuteRequest>(MessageType.AlarmMuteRequest, REQUEST_COMMITTED),
  }),
  batteryPower: messageReducer<BatteryPower>(MessageType.BatteryPower),

  // Message states from frontend_pb
  screenStatus: messageReducer<ScreenStatus>(MessageType.ScreenStatus),
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
  smoothedMeasurements: combineReducers({
    fio2: sensorMeasurementSmoothingReducer(
      0.5,
      0.1,
      500,
      200,
      (sensorMeasurements) => sensorMeasurements.fio2,
    ),
    flow: sensorMeasurementSmoothingReducer(
      0.5,
      0.5,
      500,
      200,
      (sensorMeasurements) => sensorMeasurements.flow,
    ),
    spo2: sensorMeasurementSmoothingReducer(
      1,
      1.0,
      200,
      1000,
      (sensorMeasurements) => sensorMeasurements.spo2,
    ),
    hr: sensorMeasurementSmoothingReducer(
      1,
      1.0,
      200,
      1000,
      (sensorMeasurements) => sensorMeasurements.hr,
    ),
  }),
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
