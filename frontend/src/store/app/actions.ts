import {
  INITIALIZED,
  SET_LOCALE,
  AppAction,
  BACKEND_CONNECTION_UP,
  BACKEND_CONNECTION_DOWN,
} from './types';

export function initialize(): AppAction {
  return { type: INITIALIZED };
}

export function setLocale(locale: string): AppAction {
  return { type: SET_LOCALE, locale };
}

export function lostBackendConnection(clock: Date): AppAction {
  return { type: BACKEND_CONNECTION_DOWN, clock };
}

export function establishedBackendConnection(clock: Date): AppAction {
  return { type: BACKEND_CONNECTION_UP, clock };
}
