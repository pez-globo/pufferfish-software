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
      return {
        ...state,
        backendConnection: true,
        lastBackendConnectionTime: state.lastBackendConnectionTime,
      };
    default:
      return state;
  }
}

export default connectionReducer;
