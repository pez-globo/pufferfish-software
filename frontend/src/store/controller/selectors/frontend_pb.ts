import { createSelector } from 'reselect';
import {
  SystemSettings,
  SystemSettingsRequest,
  FrontendDisplaySetting,
} from '../../proto/frontend_pb';
import { getController } from '../selectors';
import { ControllerStates, RotaryEncoderParameter, SystemSettingsRequestResponse } from '../types';

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

export const getSystemSettings = createSelector(
  getController,
  (states: ControllerStates): SystemSettingsRequestResponse => states.systemSettings,
);

export const getSystemSettingsCurrent = createSelector(
  getSystemSettings,
  (systemSettings: SystemSettingsRequestResponse): SystemSettings | null => systemSettings.current,
);

export const getSystemSettingsCurrentSeqNum = createSelector(
  getSystemSettingsCurrent,
  (systemSettings: SystemSettings | null): number | null =>
    systemSettings === null ? null : systemSettings.seqNum,
);

export const getSystemSettingsRequest = createSelector(
  getSystemSettings,
  (systemSettings: SystemSettingsRequestResponse): SystemSettingsRequest | null =>
    systemSettings.request,
);

// Display Settings

export const getFrontendDisplaySetting = createSelector(
  getController,
  (states: ControllerStates): FrontendDisplaySetting | null => states.frontendDisplaySetting,
);
