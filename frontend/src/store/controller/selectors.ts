import { createSelector, OutputSelector } from 'reselect';
import DECIMAL_RADIX from '../../modules/app/AppConstants';
import { getBackendConnected } from '../app/selectors';
import { StoreState } from '../types';
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
  ScreenStatus,
  Range,
  MCUPowerStatus,
  LogEventCode,
} from './proto/mcu_pb';
import {
  BackendConnections,
  FrontendDisplaySetting,
  SystemSettingRequest,
} from './proto/frontend_pb';
import {
  Measurements,
  ParametersRequestResponse,
  AlarmLimitsRequestResponse,
  RotaryEncoderParameter,
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
// Note: these are currently smoothed measurements if the firmware is running, as the
// firmware does smoothing. However, the simulator backend does not perform smoothing.
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
export const getParametersRequestVentilating = isVentilatingSelector(getParametersRequest);
export const getParametersRequestMode = ventilationModeSelector(getParametersRequest);
// Draft
export const getParametersRequestDraft = createSelector(
  getParameters,
  (parameters: ParametersRequestResponse): ParametersRequest | null => parameters.draft,
);
export const getParametersRequestDraftFiO2 = numericParameterSelector(
  getParametersRequestDraft,
  'fio2',
);
export const getParametersRequestDraftFlow = numericParameterSelector(
  getParametersRequestDraft,
  'flow',
);
export const getParametersRequestDraftPIP = numericParameterSelector(
  getParametersRequestDraft,
  'pip',
);
export const getParametersRequestDraftPEEP = numericParameterSelector(
  getParametersRequestDraft,
  'peep',
);
export const getParametersRequestDraftVT = numericParameterSelector(
  getParametersRequestDraft,
  'vt',
);
export const getParametersRequestDraftRR = numericParameterSelector(
  getParametersRequestDraft,
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
export const getAlarmLimitsRequestDraft = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimitsRequest | null => alarmLimits.draft,
);
export const getAlarmLimitsUnsavedKeys = createSelector(
  getAlarmLimitsRequest,
  getAlarmLimitsRequestDraft,
  (
    alarmLimitsRequest: AlarmLimitsRequest | null,
    alarmLimitsRequestDraft: AlarmLimitsRequest | null,
  ): string[] => {
    if (alarmLimitsRequest === null || alarmLimitsRequestDraft === null) {
      return [];
    }
    const alarmLimitsRequestRange = (alarmLimitsRequest as unknown) as Record<string, Range>;
    const alarmLimitsRequestDraftRange = (alarmLimitsRequestDraft as unknown) as Record<
      string,
      Range
    >;
    const keys = ['spo2', 'hr'];
    return keys.filter(
      (key: string) =>
        alarmLimitsRequestRange[key]?.lower !== alarmLimitsRequestDraftRange[key]?.lower ||
        alarmLimitsRequestRange[key]?.upper !== alarmLimitsRequestDraftRange[key]?.upper,
    );
  },
);

export const getAlarmLimitsRequestUnsaved = createSelector(
  getAlarmLimitsUnsavedKeys,
  (unsavedKeys: string[]): boolean => unsavedKeys.length > 0,
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
// Event log code selector
const getLogEventCode = (logEventCode: number) =>
  createSelector(getNextLogEvents, (events: LogEvent[]): LogEvent | undefined =>
    events.find((el: LogEvent) => (el.code as number) === logEventCode),
  );
// Note: this selector should only be used to check whether the frontend's event log
// has a temporary event indicating that the backend is down. The proper selector to
// check whether the backend is connected is store/app/selector.ts's getBackendConnected
export const getBackendDownEvent = getLogEventCode(LogEventCode.frontend_backend_connection_down);

// Changing Ventilating Status
export const getVentilatingStatusChanging = createSelector(
  getParametersIsVentilating,
  getParametersRequestVentilating,
  (parameters: boolean | null, parametersRequest: boolean | null) =>
    parameters !== parametersRequest,
);

// Alarm muting

export const getAlarmMuteRequest = createSelector(
  getController,
  (states: ControllerStates): AlarmMuteRequest | null => states.alarmMute.request,
);
export const getAlarmMuteStatus = createSelector(
  getController,
  (states: ControllerStates): AlarmMute | null => states.alarmMute.current,
);
export const getAlarmMuteActive = createSelector(
  getAlarmMuteStatus,
  (alarmMute: AlarmMute | null): boolean => (alarmMute === null ? false : alarmMute.active),
);
export const getAlarmMuteRemaining = createSelector(
  getAlarmMuteStatus,
  (alarmMute: AlarmMute | null) => (alarmMute === null ? 0 : alarmMute.remaining),
);
export const getAlarmMuteRequestRemaining = createSelector(
  getAlarmMuteRequest,
  (alarmMuteRequest: AlarmMute | null) =>
    alarmMuteRequest === null ? 0 : alarmMuteRequest.remaining,
);
export const getAlarmMuteRequestActive = createSelector(
  getAlarmMuteRequest,
  (alarmMuteRequest: AlarmMuteRequest | null) =>
    alarmMuteRequest === null ? false : alarmMuteRequest.active,
);

// Alarm Limits
// return a number[] of alarmLimits for ValueInfo based on the stateKey
export const alarmLimitsCurrentSelector = (stateKey: string) =>
  createSelector(getAlarmLimitsCurrent, (alarmLimits: AlarmLimits | null) => {
    const range =
      alarmLimits === null
        ? undefined
        : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
    const { lower, upper } = range === undefined ? { lower: 0, upper: 0 } : range;
    return [lower, upper];
  });

export const getSpO2AlarmLimitsCurrent = alarmLimitsCurrentSelector('spo2');
export const getHRAlarmLimitsCurrent = alarmLimitsCurrentSelector('hr');

// Battery power

export const getMcuPowerStatus = createSelector(
  getController,
  (states: ControllerStates): MCUPowerStatus | null => states.mcuPowerStatus,
);
export const getBatteryPowerLeft = createSelector(
  getMcuPowerStatus,
  (mcuPowerStatus: MCUPowerStatus | null): number =>
    mcuPowerStatus === null ? 0 : mcuPowerStatus.powerLeft,
);
export const getChargingStatus = createSelector(
  getMcuPowerStatus,
  (mcuPowerStatus: MCUPowerStatus | null): boolean =>
    mcuPowerStatus === null ? false : mcuPowerStatus.charging,
);

// MESSAGE STATES FROM frontend_pb
// TODO: split this section off into a new file

// Connection Statuses
export const getBackendConnections = createSelector(
  getController,
  (states: ControllerStates): BackendConnections | null => states.backendConnections,
);

// getBackendConnected is a selector defined in store/app/selectors.ts
export const getFirmwareConnected = createSelector(
  getBackendConnections,
  (backendConnections: BackendConnections | null): boolean =>
    backendConnections === null ? false : backendConnections.hasMcu,
);
export const getStoreReady = createSelector(
  getParametersRequest,
  getAlarmLimitsRequest,
  getAlarmMuteRequest,
  getBackendConnected,
  getFirmwareConnected,
  (
    parametersRequest: ParametersRequest | null,
    alarmLimitsRequest: AlarmLimitsRequest | null,
    alarmMuteRequest: AlarmMuteRequest | null,
    backendConnected: boolean,
    firmwareConnected: boolean,
  ): boolean =>
    parametersRequest !== null &&
    alarmLimitsRequest !== null &&
    alarmMuteRequest !== null &&
    backendConnected &&
    firmwareConnected,
);

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
export const getRotaryEncoderStep = createSelector(
  getRotaryEncoder,
  (rotaryEncoder: RotaryEncoderParameter | null) =>
    rotaryEncoder === null ? 0 : rotaryEncoder?.step,
);
export const getRotaryEncoderStepDiff = createSelector(
  getRotaryEncoder,
  (rotaryEncoder: RotaryEncoderParameter | null) =>
    rotaryEncoder === null ? 0 : rotaryEncoder?.stepDiff,
);
export const getRotaryEncoderButtonPressed = createSelector(
  getRotaryEncoder,
  (rotaryEncoder: RotaryEncoderParameter | null) =>
    rotaryEncoder === null ? false : rotaryEncoder?.buttonPressed,
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

export const getSmoothedFlow = getSensorMeasurementsFlow;
export const getSmoothedFiO2 = getSensorMeasurementsFiO2;
export const getSmoothedFiO2Value = createSelector(
  getSmoothedFiO2,
  getParametersFlow,
  (fio2: number, getParametersFlow: number): number => {
    return getParametersFlow > 0 ? roundValue(fio2) : NaN;
  },
);
export const getSmoothedSpO2 = getSensorMeasurementsSpO2;
export const getSmoothedHR = getSensorMeasurementsHR;

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
