// Measurements

import { createSelector, OutputSelector } from 'reselect';
import {
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  VentilationMode,
} from '../../proto/mcu_pb';
import { StoreState } from '../../types';
import { getController, roundValue } from '../selectors';
import { ControllerStates, Measurements, ParametersRequestResponse } from '../types';

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
// Changing Ventilating Status
export const getVentilatingStatusChanging = createSelector(
  getParametersIsVentilating,
  getParametersRequestVentilating,
  (parameters: boolean | null, parametersRequest: boolean | null) =>
    parameters !== parametersRequest,
);
