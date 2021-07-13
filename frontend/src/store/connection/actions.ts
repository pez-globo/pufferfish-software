import {
  ConnectionAction,
  BACKEND_HEARTBEAT,
  BACKEND_CONNECTION_UP,
  BACKEND_CONNECTION_DOWN,
} from './types';

export function lostBackendConnection(): ConnectionAction {
  return { type: BACKEND_CONNECTION_DOWN };
}

export function establishedBackendConnection(): ConnectionAction {
  return { type: BACKEND_CONNECTION_UP };
}

export function receivedBackendHeartbeat(): ConnectionAction {
  return { type: BACKEND_HEARTBEAT, backendHeartbeat: new Date() };
}
