import {
  ConnectionAction,
  ConnectionState,
  BACKEND_CONNECTION_UP,
  BACKEND_CONNECTION_DOWN,
  BACKEND_HEARTBEAT,
} from './types';

const initialState: ConnectionState = {
  backendHeartbeat: new Date(),
  backendConnection: false,
  lastBackendConnectionTime: null,
};

export function connectionReducer(state = initialState, action: ConnectionAction): ConnectionState {
  switch (action.type) {
    case BACKEND_CONNECTION_DOWN:
      return { ...state, backendConnection: false };
    case BACKEND_HEARTBEAT:
      // TODO: Redux recommends **against** using new date inside reducers; instead
      // it should come from the code which creates the action to dispatch.
      return { ...state, backendHeartbeat: state.backendHeartbeat };
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

export default connectionReducer;
