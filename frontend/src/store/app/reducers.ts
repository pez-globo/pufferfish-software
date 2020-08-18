import {
  AppAction,
  AppState,
  LOCALE_DEFAULT,
  SET_LOCALE,
} from './types';

const initialState: AppState = {
  locale: LOCALE_DEFAULT,
};

export function appReducer(
  state = initialState,
  action: AppAction
): AppState {
  switch (action.type) {
    case SET_LOCALE: {
      const {locale} = action
      return {...state, locale}
    }
    default:
      return state
  }
}
