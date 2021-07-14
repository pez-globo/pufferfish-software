import {
  ConnectionAction,
  ConnectionState,
  BACKEND_CONNECTION_UP,
  BACKEND_CONNECTION_DOWN,
} from './types';

const initialState: ConnectionState = {
  backendConnection: false,
  lastBackendConnectionTime: null,
};

export function connectionReducer(state = initialState, action: ConnectionAction): ConnectionState {
  switch (action.type) {
    case BACKEND_CONNECTION_DOWN:
      return { ...state, backendConnection: false };
    case BACKEND_CONNECTION_UP:
      // TODO: Redux recommends **against** using new date inside reducers; instead
      // it should come from the code which creates the action to dispatch.
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

export default connectionReducer;
