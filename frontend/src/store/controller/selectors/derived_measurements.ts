// Smoothed measurements

import { createSelector } from 'reselect';
import { roundValue } from '../selectors';
import {
  getSensorMeasurementsFlow,
  getSensorMeasurementsFiO2,
  getParametersFlow,
  getSensorMeasurementsSpO2,
  getSensorMeasurementsHR,
} from './measurements';

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
