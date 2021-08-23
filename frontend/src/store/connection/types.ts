// STATES

export interface ConnectionState {
  backendConnection: boolean;
  lastBackendConnectionTime: Date | null;
}

// ACTIONS

// Action Types

export const BACKEND_CONNECTION_DOWN = '@connection/BACKEND_CONNECTION_DOWN';
export const BACKEND_CONNECTION_UP = '@connection/BACKEND_CONNECTION_UP';

// Actions

interface BackendConnectionDown {
  type: typeof BACKEND_CONNECTION_DOWN;
  lastBackendConnectionTime: Date;
}

interface BackendConnectionUp {
  type: typeof BACKEND_CONNECTION_UP;
  lastBackendConnectionTime: Date;
}

export type ConnectionAction = BackendConnectionDown | BackendConnectionUp;
