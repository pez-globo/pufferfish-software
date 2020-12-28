import {
  AppAction,
  AppState,
  LOCALE_DEFAULT,
  SET_LOCALE,
  CLOCK_UPDATED,
  RED_BORDER,
} from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
  clock: new Date(),
  notifyAlarm: false,
};

export function appReducer(state = initialState, action: AppAction): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const { locale } = action;
      return { ...state, locale };
    }
    case CLOCK_UPDATED:
      return { ...state, clock: new Date() };
    case RED_BORDER:
      return { ...state, notifyAlarm: action.status };
    default:
      return state;
  }
}

export default appReducer;
