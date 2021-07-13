import {
  AppAction,
  AppState,
  LOCALE_DEFAULT,
  SET_LOCALE,
  RED_BORDER,
  BACKEND_CONNECTION_UP,
  BACKEND_CONNECTION_DOWN,
} from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
  notifyAlarm: false,
  backendConnection: false,
  lastBackendConnectionTime: null,
};

export function appReducer(state = initialState, action: AppAction): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const { locale } = action;
      return { ...state, locale };
    }
    case RED_BORDER:
      return { ...state, notifyAlarm: action.status };
    case BACKEND_CONNECTION_DOWN:
      return {
        ...state,
        backendConnection: false,
      };
    case BACKEND_CONNECTION_UP:
      return {
        ...state,
        backendConnection: true,
        lastBackendConnectionTime: state.backendConnection
          ? state.lastBackendConnectionTime
          : new Date(),
      };
    default:
      return state;
  }
}

export default appReducer;
