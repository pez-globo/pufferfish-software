import { createSelector, OutputSelector } from 'reselect';
import DECIMAL_RADIX from '../../modules/app/AppConstants';
import { getBackendConnected } from '../app/selectors';
import { StoreState } from '../types';
import { FrontendDisplaySetting, SystemSettingRequest } from './proto/frontend_pb';
import {
  AlarmLimits,
  AlarmLimitsRequest,
  AlarmMute,
  AlarmMuteRequest,
  CycleMeasurements,
  LogEvent,
  ExpectedLogEvent,
  Parameters,
  ParametersRequest,
  SensorMeasurements,
  VentilationMode,
  BatteryPower,
  ScreenStatus,
} from './proto/mcu_pb';
import {
  Measurements,
  ParametersRequestResponse,
  AlarmLimitsRequestResponse,
  RotaryEncoderParameter,
  SmoothedMeasurements,
  Plots,
  WaveformHistories,
  WaveformHistory,
  WaveformPoint,
  PVHistory,
  PVPoint,
  ControllerStates,
} from './types';

export const roundValue = (value: number): number => {
  return value !== undefined && !Number.isNaN(value)
    ? parseInt(value.toFixed(0).replace(/^-0$/, '0'), DECIMAL_RADIX)
    : NaN;
};

export const getController = ({ controller }: StoreState): ControllerStates => controller;

// MESSAGE STATES FROM mcu_pb
// TODO: split this section off into a new file

// Measurements

export const getMeasurements = createSelector(
  getController,
  (states: ControllerStates): Measurements => states.measurements,
);

// SensorMeasurements
export const getSensorMeasurements = createSelector(
  getMeasurements,
  (measurements: Measurements): SensorMeasurements | null => measurements.sensor,
);
const sensorMeasurementSelector = (key: string) =>
  createSelector(getSensorMeasurements, (measurements: SensorMeasurements | null): number =>
    measurements === null
      ? NaN
      : roundValue(((measurements as unknown) as Record<string, number>)[key]),
  );
export const getSensorMeasurementsTime = sensorMeasurementSelector('time');
export const getSensorMeasurementsPaw = sensorMeasurementSelector('paw');
export const getSensorMeasurementsFlow = sensorMeasurementSelector('flow');
export const getSensorMeasurementsVolume = sensorMeasurementSelector('volume');
export const getSensorMeasurementsFiO2 = sensorMeasurementSelector('fio2');
export const getSensorMeasurementsSpO2 = sensorMeasurementSelector('spo2');
export const getSensorMeasurementsHR = sensorMeasurementSelector('hr');

// CycleMeasurements
export const getCycleMeasurements = createSelector(
  getMeasurements,
  (measurements: Measurements): CycleMeasurements | null => measurements.cycle,
);
const cycleMeasurementSelector = (key: string) =>
  createSelector(getCycleMeasurements, (measurements: CycleMeasurements | null): number =>
    measurements === null
      ? NaN
      : roundValue(((measurements as unknown) as Record<string, number>)[key]),
  );
export const getCycleMeasurementsPIP = cycleMeasurementSelector('pip');
export const getCycleMeasurementsPEEP = cycleMeasurementSelector('peep');
export const getCycleMeasurementsRR = cycleMeasurementSelector('rr');
export const getCycleMeasurementsVT = cycleMeasurementSelector('vt');

// ROX Index
export const getROXIndex = createSelector(
  getSensorMeasurements,
  getCycleMeasurements,
  (
    sensorMeasurements: SensorMeasurements | null,
    cycleMeasurements: CycleMeasurements | null,
  ): number => {
    if (sensorMeasurements === null || cycleMeasurements === null) {
      return NaN;
    }

    if (sensorMeasurements.spo2 && sensorMeasurements.fio2 && cycleMeasurements.rr) {
      return parseFloat(
        (sensorMeasurements.spo2 / sensorMeasurements.fio2 / cycleMeasurements.rr).toFixed(2),
      );
    }
    return NaN;
  },
);

// Parameters

// Generic parameters
export const getParameters = createSelector(
  getController,
  (states: ControllerStates): ParametersRequestResponse => states.parameters,
);
type OptionalParametersLike = Parameters | ParametersRequest | null;
type ParametersSelector = OutputSelector<
  StoreState,
  Parameters | null,
  (res: ParametersRequestResponse) => OptionalParametersLike
>;
const isVentilatingSelector = (parametersSelector: ParametersSelector) =>
  createSelector(parametersSelector, (parameters: OptionalParametersLike): boolean | null =>
    parameters === null ? null : parameters.ventilating,
  );
const ventilationModeSelector = (parametersSelector: ParametersSelector) =>
  createSelector(parametersSelector, (parameters: OptionalParametersLike): VentilationMode | null =>
    parameters === null ? null : parameters.mode,
  );
const numericParameterSelector = (parametersSelector: ParametersSelector, key: string) =>
  createSelector(parametersSelector, (parameters: Parameters | ParametersRequest | null): number =>
    parameters === null
      ? NaN
      : roundValue(((parameters as unknown) as Record<string, number>)[key]),
  );
// Current
export const getParametersCurrent = createSelector(
  getParameters,
  (parameters: ParametersRequestResponse): Parameters | null => parameters.current,
);
export const getParametersIsVentilating = isVentilatingSelector(getParametersCurrent);
export const getParametersFiO2 = numericParameterSelector(getParametersCurrent, 'fio2');
export const getParametersFlow = numericParameterSelector(getParametersCurrent, 'flow');
export const getParametersPIP = numericParameterSelector(getParametersCurrent, 'pip');
export const getParametersPEEP = numericParameterSelector(getParametersCurrent, 'peep');
export const getParametersVT = numericParameterSelector(getParametersCurrent, 'vt');
export const getParametersRR = numericParameterSelector(getParametersCurrent, 'rr');
// Request
export const getParametersRequest = createSelector(
  getParameters,
  (parameters: ParametersRequestResponse): ParametersRequest | null => parameters.request,
);
export const getParametersRequestMode = ventilationModeSelector(getParametersRequest);
// Standby
export const getParametersRequestStandby = createSelector(
  getParameters,
  (parameters: ParametersRequestResponse): ParametersRequest | null => parameters.standby,
);
export const getParametersRequestStandbyFiO2 = numericParameterSelector(
  getParametersRequestStandby,
  'fio2',
);
export const getParametersRequestStandbyFlow = numericParameterSelector(
  getParametersRequestStandby,
  'flow',
);
export const getParametersRequestStandbyPIP = numericParameterSelector(
  getParametersRequestStandby,
  'pip',
);
export const getParametersRequestStandbyPEEP = numericParameterSelector(
  getParametersRequestStandby,
  'peep',
);
export const getParametersRequestStandbyVT = numericParameterSelector(
  getParametersRequestStandby,
  'vt',
);
export const getParametersRequestStandbyRR = numericParameterSelector(
  getParametersRequestStandby,
  'rr',
);

// Alarm Limits

export const getAlarmLimits = createSelector(
  getController,
  (states: ControllerStates): AlarmLimitsRequestResponse => states.alarmLimits,
);
export const getAlarmLimitsCurrent = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimits | null => alarmLimits.current,
);
export const getAlarmLimitsRequest = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimitsRequest | null => alarmLimits.request,
);
export const getAlarmLimitsRequestStandby = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimitsRequest | null => alarmLimits.standby,
);

// Event log

// Next log events
export const getNextLogEvents = createSelector(
  getController,
  (states: ControllerStates): LogEvent[] => states.eventLog.nextLogEvents.elements,
);

export const getExpectedLogEvent = createSelector(
  getController,
  (states: ControllerStates): number => states.eventLog.expectedLogEvent.id,
);
export const getFullExpectedLogEvent = createSelector(
  getController,
  (states: ControllerStates): ExpectedLogEvent => states.eventLog.expectedLogEvent,
);
// Active log events
export const getActiveLogEventIds = createSelector(
  getController,
  (states: ControllerStates): number[] => states.eventLog.activeLogEvents.id,
);
export const getHasActiveAlarms = createSelector(
  getActiveLogEventIds,
  (activeLogEventIds: number[]): boolean => activeLogEventIds.length > 0,
);
// TODO: rename this selector to something more descriptive:
export const getPopupEventLog = createSelector(getController, (states: ControllerStates):
  | LogEvent
  | undefined => {
  const maxId = Math.max(...states.eventLog.activeLogEvents.id);
  return states.eventLog.nextLogEvents.elements.find((el: LogEvent) => el.id === maxId);
});

// Backend Initialized
export const getBackendInitialized = createSelector(
  getParametersRequest,
  getAlarmLimitsRequest,
  getBackendConnected,
  (
    parametersRequest: ParametersRequest | null,
    alarmLimitsRequest: AlarmLimitsRequest | null,
    backendConnected: boolean,
  ): boolean => parametersRequest !== null && alarmLimitsRequest !== null && backendConnected,
);

// Alarm muting

export const getAlarmMuteRequest = createSelector(
  getController,
  (states: ControllerStates): AlarmMuteRequest | null => states.alarmMuteRequest,
);
// TODO: Need to change state from 'alarmMuteRequest' to 'alarmMute'
export const getAlarmMuteStatus = createSelector(
  getController,
  (states: ControllerStates): AlarmMute | null => states.alarmMuteRequest,
);

// Battery power

export const getBatteryPower = createSelector(
  getController,
  (states: ControllerStates): BatteryPower | null => states.batteryPower,
);
export const getBatteryPowerLeft = createSelector(
  getBatteryPower,
  (batteryPower: BatteryPower | null): number =>
    batteryPower === null ? 0 : batteryPower.powerLeft,
);
export const getChargingStatus = createSelector(
  getBatteryPower,
  (batteryPower: BatteryPower | null): boolean =>
    batteryPower === null ? false : batteryPower.chargingStatus,
);

// MESSAGE STATES FROM frontend_pb
// TODO: split this section off into a new file

// Screen Status

export const getScreenStatus = createSelector(
  getController,
  (states: ControllerStates): ScreenStatus | null => states.screenStatus,
);
export const getScreenStatusLock = createSelector(
  getScreenStatus,
  (screenStatus: ScreenStatus | null): boolean =>
    screenStatus === null ? false : screenStatus.lock,
);

// RotaryEncoder

export const getRotaryEncoder = createSelector(
  getController,
  (states: ControllerStates): RotaryEncoderParameter | null => states.rotaryEncoder,
);

// System Settings

export const getSystemSettingRequest = createSelector(
  getController,
  (states: ControllerStates): SystemSettingRequest | null => states.systemSettingRequest,
);

// Display Settings

export const getFrontendDisplaySetting = createSelector(
  getController,
  (states: ControllerStates): FrontendDisplaySetting | null => states.frontendDisplaySetting,
);

// DERIVED STATES
// TODO: split this section off into a new file

// Smoothed measurements

export const getSmoothedMeasurements = createSelector(
  getController,
  (states: ControllerStates): SmoothedMeasurements => states.smoothedMeasurements,
);
// TODO: we could unify this code with a generic selector, but it's only worth it
// if we need selectors for any more fields besides "smoothed".
export const getSmoothedFlow = createSelector(
  getSmoothedMeasurements,
  (smoothed: SmoothedMeasurements): number => roundValue(smoothed.flow.smoothed),
);
export const getSmoothedFiO2 = createSelector(
  getSmoothedMeasurements,
  (smoothed: SmoothedMeasurements): number => roundValue(smoothed.fio2.smoothed),
);
export const getSmoothedFiO2Value = createSelector(
  getSmoothedFiO2,
  getParametersFlow,
  (fio2: number, getParametersFlow: number): number => {
    return getParametersFlow > 0 ? roundValue(fio2) : NaN;
  },
);
export const getSmoothedSpO2 = createSelector(
  getSmoothedMeasurements,
  (smoothed: SmoothedMeasurements): number => roundValue(smoothed.spo2.smoothed),
);
export const getSmoothedHR = createSelector(
  getSmoothedMeasurements,
  (smoothed: SmoothedMeasurements): number => roundValue(smoothed.hr.smoothed),
);

// Plots

export const getPlots = createSelector(
  getController,
  (states: ControllerStates): Plots => states.plots,
);
// Generic waveforms
export const getWaveforms = createSelector(
  getPlots,
  (plots: Plots): WaveformHistories => plots.waveforms,
);
type WaveformSelector = OutputSelector<
  StoreState,
  WaveformHistory,
  (res: WaveformHistories) => WaveformHistory
>;
const waveformOldSelector = (waveformSelector: WaveformSelector) =>
  createSelector(
    waveformSelector,
    (waveformHistory: WaveformHistory): WaveformPoint[] => waveformHistory.waveformOld.full,
  );
const waveformNewSegmentsSelector = (waveformSelector: WaveformSelector) =>
  createSelector(
    waveformSelector,
    (waveformHistory: WaveformHistory): WaveformPoint[][] => waveformHistory.waveformNew.segmented,
  );
const waveformNewSegmentSelector = (
  waveformNewSegmentsSelector: OutputSelector<
    StoreState,
    WaveformPoint[][],
    (res: WaveformHistory) => WaveformPoint[][]
  >,
) => (segmentIndex: number) =>
  createSelector(
    waveformNewSegmentsSelector,
    (waveformSegments: WaveformPoint[][]): WaveformPoint[] => waveformSegments[segmentIndex],
  );
// Paw Waveform
export const getWaveformPaw = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformPawOld = waveformOldSelector(getWaveformPaw);
export const getWaveformPawNewSegments = waveformNewSegmentsSelector(getWaveformPaw);
export const getWaveformPawNewSegment = waveformNewSegmentSelector(getWaveformPawNewSegments);
// Flow Waveform
export const getWaveformFlow = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformFlowOld = waveformOldSelector(getWaveformFlow);
export const getWaveformFlowNewSegments = waveformNewSegmentsSelector(getWaveformFlow);
export const getWaveformFlowNewSegment = waveformNewSegmentSelector(getWaveformFlowNewSegments);
// Volume Waveform
export const getWaveformVolume = createSelector(
  getWaveforms,
  (histories: WaveformHistories): WaveformHistory => histories.paw,
);
export const getWaveformVolumeOld = waveformOldSelector(getWaveformVolume);
export const getWaveformVolumeNewSegments = waveformNewSegmentsSelector(getWaveformVolume);
export const getWaveformVolumeNewSegment = waveformNewSegmentSelector(getWaveformVolumeNewSegments);
// P-V Loop
export const getPVHistory = createSelector(getPlots, (plots: Plots): PVHistory => plots.pvLoop);
export const getPVLoop = createSelector(
  getPVHistory,
  (pvHistory: PVHistory): PVPoint[] => pvHistory.loop,
);
