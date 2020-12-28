export const INITIALIZED = '@app/INITIALIZED';
export const SET_LOCALE = '@app/SET_LOCALE';

export const LOCALE_EN_US = 'en-us';
export const LOCALE_DEFAULT = LOCALE_EN_US;
export const CLOCK_UPDATED = '@app/CLOCK_UPDATED';
export const RED_BORDER = '@app/RED_BORDER';

interface InitializedAction {
  type: typeof INITIALIZED;
}

interface SetLocaleAction {
  type: typeof SET_LOCALE;
  locale: string;
}

interface SetClock {
  type: typeof CLOCK_UPDATED;
}

interface SetRedBorder {
  type: typeof RED_BORDER;
  status: boolean;
}

export type AppAction = InitializedAction | SetLocaleAction | SetClock | SetRedBorder;
export interface AppState {
  locale: string;
  clock: Date;
  notifyAlarm: boolean;
}
