// STATES

export interface AppState {
  locale: string;
  notifyAlarm: boolean;
}

// ACTIONS

// Action Types

export const SET_LOCALE = '@app/SET_LOCALE';
export const LOCALE_EN_US = 'en-us';
export const LOCALE_DEFAULT = LOCALE_EN_US;

export const INITIALIZED = '@app/INITIALIZED';

export const RED_BORDER = '@app/RED_BORDER';

// Actions

interface InitializedAction {
  type: typeof INITIALIZED;
}

interface SetLocaleAction {
  type: typeof SET_LOCALE;
  locale: string;
}

interface SetRedBorder {
  type: typeof RED_BORDER;
  status: boolean;
}

export type AppAction = InitializedAction | SetLocaleAction | SetRedBorder;
