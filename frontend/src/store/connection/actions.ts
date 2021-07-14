import { ConnectionAction, BACKEND_CONNECTION_UP, BACKEND_CONNECTION_DOWN } from './types';

export function lostBackendConnection(): ConnectionAction {
  return { type: BACKEND_CONNECTION_DOWN };
}

export function establishedBackendConnection(): ConnectionAction {
  return { type: BACKEND_CONNECTION_UP };
}
