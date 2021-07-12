// STATES

export interface AppState {
  locale: string;
  notifyAlarm: boolean;
  backendConnection: boolean;
  lastBackendConnectionTime: Date | null;
}

// ACTIONS

// Action Types

export const SET_LOCALE = '@app/SET_LOCALE';
export const LOCALE_EN_US = 'en-us';
export const LOCALE_DEFAULT = LOCALE_EN_US;

export const INITIALIZED = '@app/INITIALIZED';

// TODO: move BACKEND actions, reducers, and saga stuff into a separate domain, '@connection'
export const BACKEND_CONNECTION_DOWN = '@app/BACKEND_CONNECTION_DOWN';
export const BACKEND_CONNECTION_UP = '@app/BACKEND_CONNECTION_UP';

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

interface BackendConnectionDown {
  type: typeof BACKEND_CONNECTION_DOWN;
  update: Record<string, unknown>;
}

interface BackendConnectionUp {
  type: typeof BACKEND_CONNECTION_UP;
}

export type AppAction =
  | InitializedAction
  | SetLocaleAction
  | SetRedBorder
  | BackendConnectionDown
  | BackendConnectionUp;
