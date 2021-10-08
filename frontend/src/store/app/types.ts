// STATES

export interface AppState {
  locale: string;
  notifyAlarm: boolean;
  wizardOpen: boolean;
  stateKey: string | undefined;
  discardPopupOpen: boolean;
  rotaryReference: string | null;
  screenLock: boolean;
}

// ACTIONS

// Action Types

export const SET_LOCALE = '@app/SET_LOCALE';
export const LOCALE_EN_US = 'en-us';
export const LOCALE_DEFAULT = LOCALE_EN_US;

export const INITIALIZED = '@app/INITIALIZED';

export const RED_BORDER = '@app/RED_BORDER';
export const SET_WIZARD = '@app/SET_WIZARD';
export const DISCARD_LIMITS_POPUP = '@app/DISCARD_LIMITS_POPUP';
export const ROTARY_REF = '@app/ROTARY_REF';
export const SCREEN_LOCK = '@app/SCREEN_LOCK';

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

interface SetVentilationWizard {
  type: typeof SET_WIZARD;
  wizardOpen: boolean;
  stateKey: string | undefined;
}

interface DiscardAlarmLimitsPopup {
  type: typeof DISCARD_LIMITS_POPUP;
  open: boolean;
}

interface SetRotaryReference {
  type: typeof ROTARY_REF;
  reference: string | null;
}

interface SetScreenLock {
  type: typeof SCREEN_LOCK;
  lock: boolean;
}

export type AppAction =
  | InitializedAction
  | SetLocaleAction
  | SetRedBorder
  | SetVentilationWizard
  | DiscardAlarmLimitsPopup
  | SetRotaryReference
  | SetScreenLock;
