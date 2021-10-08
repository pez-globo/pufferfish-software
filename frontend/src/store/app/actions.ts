import { INITIALIZED, SET_LOCALE, AppAction, SET_WIZARD, ROTARY_REF } from './types';

export function initialize(): AppAction {
  return { type: INITIALIZED };
}

export function setLocale(locale: string): AppAction {
  return { type: SET_LOCALE, locale };
}

export function setVentilationWizard(open: boolean, stateKey?: string): AppAction {
  return { type: SET_WIZARD, open, stateKey };
}

export function setActiveRotaryReference(reference: string | null): AppAction {
  return { type: ROTARY_REF, reference };
}
