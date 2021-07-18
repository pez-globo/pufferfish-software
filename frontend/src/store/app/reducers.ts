import { AppAction, AppState, LOCALE_DEFAULT, SET_LOCALE, RED_BORDER } from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
  notifyAlarm: false,
};

export function appReducer(state = initialState, action: AppAction): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const { locale } = action;
      return { ...state, locale };
    }
    case RED_BORDER:
      return { ...state, notifyAlarm: action.status };
    default:
      return state;
  }
}

export default appReducer;
