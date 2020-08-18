export const INITIALIZED = '@app/INITIALIZED';
export const SET_LOCALE = '@app/SET_LOCALE';

export const LOCALE_EN_US = 'en-us';
export const LOCALE_DEFAULT = LOCALE_EN_US;

interface InitializedAction {
  type: typeof INITIALIZED
}

interface SetLocaleAction {
  type: typeof SET_LOCALE
  locale: string
}

export type AppAction = InitializedAction | SetLocaleAction;
export interface AppState {
  locale: string
}
