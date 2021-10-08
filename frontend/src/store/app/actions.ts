import { INITIALIZED, SET_LOCALE, AppAction, SET_WIZARD, ROTARY_REF, SCREEN_LOCK } from './types';

export function initialize(): AppAction {
  return { type: INITIALIZED };
}

export function setLocale(locale: string): AppAction {
  return { type: SET_LOCALE, locale };
}

export function setVentilationWizard(wizardOpen: boolean, stateKey?: string): AppAction {
  return { type: SET_WIZARD, wizardOpen, stateKey };
}

export function setActiveRotaryReference(reference: string | null): AppAction {
  return { type: ROTARY_REF, reference };
}

export function setScreenLock(lock: boolean): AppAction {
  return { type: SCREEN_LOCK, lock };
}
