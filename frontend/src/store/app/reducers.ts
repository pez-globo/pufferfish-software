import {
  AppAction,
  AppState,
  LOCALE_DEFAULT,
  SET_LOCALE,
  RED_BORDER,
  SET_WIZARD,
  DISCARD_LIMITS_POPUP,
  ROTARY_REF,
  SCREEN_LOCK,
} from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
  notifyAlarm: false,
  wizardOpen: false,
  stateKey: 'spo2',
  discardPopupOpen: false,
  rotaryReference: null,
  screenLock: false,
};

export function appReducer(state = initialState, action: AppAction): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const { locale } = action;
      return { ...state, locale };
    }
    case RED_BORDER:
      return { ...state, notifyAlarm: action.status };
    case SET_WIZARD:
      return { ...state, wizardOpen: action.wizardOpen, stateKey: action.stateKey };
    case DISCARD_LIMITS_POPUP:
      return { ...state, discardPopupOpen: action.open };
    case ROTARY_REF:
      return { ...state, rotaryReference: action.reference };
    case SCREEN_LOCK:
      return { ...state, screenLock: action.lock };
    default:
      return state;
  }
}

export default appReducer;
