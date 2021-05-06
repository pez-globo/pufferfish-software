import {
  AppAction,
  AppState,
  LOCALE_DEFAULT,
  SET_LOCALE,
  CLOCK_UPDATED,
  RED_BORDER,
  BACKEND_HEARTBEAT,
} from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
  clock: new Date(),
  notifyAlarm: false,
  backendHeartbeat: new Date(),
};

export function appReducer(state = initialState, action: AppAction): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const { locale } = action;
      return { ...state, locale };
    }
    case CLOCK_UPDATED:
      // TODO: Redux recommends **against** using new date inside reducers; instead
      // it should come from the code which creates the action to dispatch.
      return { ...state, clock: new Date() };
    case RED_BORDER:
      return { ...state, notifyAlarm: action.status };
    case BACKEND_HEARTBEAT:
      // TODO: Redux recommends **against** using new date inside reducers; instead
      // it should come from the code which creates the action to dispatch.
      return { ...state, backendHeartbeat: new Date() };
    default:
      return state;
  }
}

export default appReducer;
