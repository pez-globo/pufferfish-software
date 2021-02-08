import { createSelector, OutputSelector } from 'reselect';
import DECIMAL_RADIX from '../../modules/app/AppConstants';
import { StoreState } from '../types';
import { FrontendDisplaySetting, SystemSettingRequest } from './proto/frontend_pb';
import {
  AlarmLimitsRequest,
  AlarmMute,
  AlarmMuteRequest,
  CycleMeasurements,
  LogEvent,
  Parameters,
  ParametersRequest,
  SensorMeasurements,
  VentilationMode,
  Range,
} from './proto/mcu_pb';
import {
  ControllerStates,
  PVHistory,
  PVPoint,
  RotaryEncoderParameter,
  WaveformHistory,
  WaveformPoint,
} from './types';

export const roundValue = (value: number): number => {
  return value !== undefined && !Number.isNaN(value)
    ? parseInt(value.toFixed(0).replace(/^-0$/, '0'), DECIMAL_RADIX)
    : NaN;
};
export const getController = ({ controller }: StoreState): ControllerStates => controller;

// SensorMeasurements
export const getSensorMeasurements = createSelector(
  getController,
  (states: ControllerStates): SensorMeasurements => states.sensorMeasurements,
);
export const getSensorMeasurementsTime = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.time),
);
export const getSensorMeasurementsPaw = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.paw),
);
export const getSensorMeasurementsFlow = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.flow),
);
export const getSensorMeasurementsVolume = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.volume),
);
export const getSensorMeasurementsFiO2 = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.fio2),
);
export const getSensorMeasurementsSpO2 = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.spo2),
);
export const getSensorMeasurementsHR = createSelector(
  getSensorMeasurements,
  (sensorMeasurements: SensorMeasurements): number => roundValue(sensorMeasurements.hr),
);

// SensorMeasurementsSmoothed
export const getSmoothedFlow = createSelector(getController, (states: ControllerStates): number =>
  roundValue(states.smoothedMeasurements.flow.smoothed),
);
export const getSmoothedFiO2 = createSelector(getController, (states: ControllerStates): number =>
  roundValue(states.smoothedMeasurements.fio2.smoothed),
);
export const getSmoothedSpO2 = createSelector(getController, (states: ControllerStates): number =>
  roundValue(states.smoothedMeasurements.spo2.smoothed),
);

// CycleMeasurements
export const getCycleMeasurements = createSelector(
  getController,
  (states: ControllerStates): CycleMeasurements => states.cycleMeasurements,
);
export const getCycleMeasurementsPIP = createSelector(
  getCycleMeasurements,
  (cycleMeasurements: CycleMeasurements): number => roundValue(cycleMeasurements.pip),
);
export const getCycleMeasurementsPEEP = createSelector(
  getCycleMeasurements,
  (cycleMeasurements: CycleMeasurements): number => roundValue(cycleMeasurements.peep),
);
export const getCycleMeasurementsRR = createSelector(
  getCycleMeasurements,
  (cycleMeasurements: CycleMeasurements): number => roundValue(cycleMeasurements.rr),
);
export const getCycleMeasurementsVT = createSelector(
  getCycleMeasurements,
  (cycleMeasurements: CycleMeasurements): number => roundValue(cycleMeasurements.vt),
);

// ROX Index
export const getROXIndex = createSelector(
  getSensorMeasurements,
  getCycleMeasurements,
  (sensorMeasurements: SensorMeasurements, cycleMeasurements: CycleMeasurements): number => {
    if (sensorMeasurements.spo2 && sensorMeasurements.fio2 && cycleMeasurements.rr) {
      return parseFloat(
        (sensorMeasurements.spo2 / sensorMeasurements.fio2 / cycleMeasurements.rr).toFixed(2),
      );
    }
    return 0;
  },
);

// Parameters
export const getParameters = createSelector(
  getController,
  (states: ControllerStates): Parameters => states.parameters,
);
export const getParametersPIP = createSelector(getParameters, (parameters: Parameters): number =>
  roundValue(parameters.pip),
);
export const getParametersPEEP = createSelector(getParameters, (parameters: Parameters): number =>
  roundValue(parameters.peep),
);
export const getParametersRR = createSelector(getParameters, (parameters: Parameters): number =>
  roundValue(parameters.rr),
);
export const getParametersFiO2 = createSelector(getParameters, (parameters: Parameters): number =>
  roundValue(parameters.fio2),
);
export const getParametersFlow = createSelector(getParameters, (parameters: Parameters): number =>
  roundValue(parameters.flow),
);
export const getIsVentilating = createSelector(
  getParameters,
  (parameters: Parameters): boolean => parameters.ventilating,
);
// ParametersRequest
export const getParametersRequest = createSelector(
  getController,
  (states: ControllerStates): ParametersRequest => states.parametersRequest,
);

export const getParametersRequestStandby = createSelector(
  getController,
  (states: ControllerStates): ParametersRequest =>
    states.parametersRequestStandby.parameters as ParametersRequest,
);

export const getParametersRequestMode = createSelector(
  getParametersRequest,
  (parametersRequest: ParametersRequest): VentilationMode => parametersRequest.mode,
);

// Derived FiO2 Sensor Measurements
export const getSmoothedFiO2Value = createSelector(
  getSmoothedFiO2,
  getParametersFlow,
  (fio2: number, getParametersFlow: number): number | undefined => {
    return getParametersFlow > 0 ? roundValue(fio2) : undefined;
  },
);

// RotaryEncoder
export const getRotaryEncoder = createSelector(
  getController,
  (states: ControllerStates): RotaryEncoderParameter => states.rotaryEncoder,
);

// Waveforms
export const getWaveformHistoryPaw = createSelector(
  getController,
  (states: ControllerStates): WaveformHistory => states.waveformHistoryPaw,
);
export const getWaveformPawOld = createSelector(
  getWaveformHistoryPaw,
  (waveformHistory: WaveformHistory): WaveformPoint[] => waveformHistory.waveformOld.full,
);
// Paw Waveforms
export const getWaveformPawNewSegments = createSelector(
  getWaveformHistoryPaw,
  (waveformHistory: WaveformHistory): WaveformPoint[][] => waveformHistory.waveformNew.segmented,
);
export const getWaveformPawNewSegment = (
  segmentIndex: number,
): OutputSelector<StoreState, WaveformPoint[], unknown> =>
  createSelector(
    getWaveformPawNewSegments,
    (waveformSegments: WaveformPoint[][]): WaveformPoint[] => waveformSegments[segmentIndex],
  );
export const getWaveformHistoryFlow = createSelector(
  getController,
  (states: ControllerStates): WaveformHistory => states.waveformHistoryFlow,
);
export const getWaveformHistoryVolume = createSelector(
  getController,
  (states: ControllerStates): WaveformHistory => states.waveformHistoryVolume,
);
export const getWaveformFlowOld = createSelector(
  getWaveformHistoryFlow,
  (waveformHistory: WaveformHistory): WaveformPoint[] => waveformHistory.waveformOld.full,
);
export const getWaveformFlowNewSegments = createSelector(
  getWaveformHistoryFlow,
  (waveformHistory: WaveformHistory): WaveformPoint[][] => waveformHistory.waveformNew.segmented,
);
export const getWaveformFlowNewSegment = (
  segmentIndex: number,
): OutputSelector<StoreState, WaveformPoint[], unknown> =>
  createSelector(
    getWaveformFlowNewSegments,
    (waveformSegments: WaveformPoint[][]): WaveformPoint[] => waveformSegments[segmentIndex],
  );
// Volume Waveforms
export const getWaveformVolumeOld = createSelector(
  getWaveformHistoryVolume,
  (waveformHistory: WaveformHistory): WaveformPoint[] => waveformHistory.waveformOld.full,
);
export const getWaveformVolumeNewSegments = createSelector(
  getWaveformHistoryVolume,
  (waveformHistory: WaveformHistory): WaveformPoint[][] => waveformHistory.waveformNew.segmented,
);
export const getWaveformVolumeNewSegment = (
  segmentIndex: number,
): OutputSelector<StoreState, WaveformPoint[], unknown> =>
  createSelector(
    getWaveformVolumeNewSegments,
    (waveformSegments: WaveformPoint[][]): WaveformPoint[] => waveformSegments[segmentIndex],
  );

// P-V Loops
export const getPVHistory = createSelector(
  getController,
  (states: ControllerStates): PVHistory => states.pvHistory,
);
export const getPVLoop = createSelector(
  getPVHistory,
  (pvHistory: PVHistory): PVPoint[] => pvHistory.loop,
);

// Alarm Limits
export const getAlarmLimitsRequest = createSelector(
  getController,
  (states: ControllerStates): AlarmLimitsRequest | Record<string, Range> =>
    states.alarmLimitsRequest,
);

export const getAlarmLimitsRequestStandby = createSelector(
  getController,
  (states: ControllerStates): AlarmLimitsRequest | Record<string, Range> =>
    states.alarmLimitsRequestStandby.alarmLimits as AlarmLimitsRequest,
);

// Display Settings
export const getFrontendDisplaySetting = createSelector(
  getController,
  (states: ControllerStates): FrontendDisplaySetting => states.frontendDisplaySetting,
);

// Display Settings
export const getSystemSettingRequest = createSelector(
  getController,
  (states: ControllerStates): SystemSettingRequest => states.systemSettingRequest,
);

// Next Log Events
export const getNextLogEvents = createSelector(
  getController,
  (states: ControllerStates): LogEvent[] => states.nextLogEvents.elements,
);

// Patient Alarm Event
export const getExpectedLogEvent = createSelector(
  getController,
  (states: ControllerStates): number => states.expectedLogEvent.id,
);

// Active log event Ids
export const getActiveLogEventIds = createSelector(
  getController,
  (states: ControllerStates): number[] => states.activeLogEvents.id,
);

// Active popup event log
export const getPopupEventLog = createSelector(getController, (states: ControllerStates):
  | LogEvent
  | undefined => {
  const maxId = Math.max(...states.activeLogEvents.id);
  return states.nextLogEvents.elements.find((el: LogEvent) => el.id === maxId);
});

// Battery power
export const getBatteryPower = createSelector(
  getController,
  (states: ControllerStates): number => states.batteryPower.powerLeft,
);

// Charging Status
export const getChargingStatus = createSelector(
  getController,
  (states: ControllerStates): boolean => states.batteryPower.chargingStatus,
);

// Srceen Status
export const getScreenStatus = createSelector(
  getController,
  (states: ControllerStates): boolean => states.screenStatus.lock,
);

// Alarm mute Status
export const getAlarmMuteRequest = createSelector(
  getController,
  (states: ControllerStates): AlarmMuteRequest => states.alarmMuteRequest,
);

// TODO: Need to change state from 'alarmMuteRequest' to 'alarmMute'
export const getAlarmMuteStatus = createSelector(
  getController,
  (states: ControllerStates): AlarmMute => states.alarmMuteRequest,
);
