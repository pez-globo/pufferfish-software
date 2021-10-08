import { createSelector } from 'reselect';
import enUS from '../../translations/en-us.json';
import { StoreState } from '../types';
import { LOCALE_DEFAULT, LOCALE_EN_US, AppState } from './types';

// Locale

const MESSAGE_CATALOG: Record<string, Record<string, string>> = {
  [LOCALE_EN_US]: enUS,
};
export const getLocale = ({ app }: StoreState): string => app.locale;
export const getLocales = (): Array<string> => [LOCALE_EN_US];
export const getMessages = createSelector(
  getLocale,
  (locale: string): Record<string, string> => {
    return {
      ...MESSAGE_CATALOG[LOCALE_DEFAULT],
      ...MESSAGE_CATALOG[locale],
    };
  },
);

// App

export const getApp = (store: StoreState): AppState => store.app;

export const getAlarmNotifyStatus = createSelector(
  getApp,
  (app: AppState): boolean => app.notifyAlarm,
);

export const getVentilationWizardOpen = createSelector(
  getApp,
  (app: AppState): boolean => app.wizardOpen,
);

export const getCurrentStateKey = createSelector(getApp, (app: AppState): string =>
  app.stateKey === undefined ? 'spo2' : app.stateKey,
);

export const getDiscardAlarmLimitsPopupOpen = createSelector(
  getApp,
  (app: AppState): boolean => app.discardPopupOpen,
);

export const getRotaryReference = createSelector(
  getApp,
  (app: AppState): string | null => app.rotaryReference,
);

export const getScreenLock = createSelector(getApp, (app: AppState): boolean => app.screenLock);
