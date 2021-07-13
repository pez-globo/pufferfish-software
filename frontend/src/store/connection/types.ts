// STATES

export interface ConnectionState {
  backendHeartbeat: Date;
  backendConnection: boolean;
  lastBackendConnectionTime: Date | null;
}

// ACTIONS

// Action Types

export const BACKEND_HEARTBEAT = '@connection/BACKEND_HEARTBEAT';
export const BACKEND_CONNECTION_DOWN = '@connection/BACKEND_CONNECTION_DOWN';
export const BACKEND_CONNECTION_UP = '@connection/BACKEND_CONNECTION_UP';

// Actions

interface UpdateBackendHeartbeat {
  type: typeof BACKEND_HEARTBEAT;
  backendHeartbeat: Date;
}

interface BackendConnectionDown {
  type: typeof BACKEND_CONNECTION_DOWN;
}

interface BackendConnectionUp {
  type: typeof BACKEND_CONNECTION_UP;
}

export type ConnectionAction = BackendConnectionDown | BackendConnectionUp | UpdateBackendHeartbeat;
