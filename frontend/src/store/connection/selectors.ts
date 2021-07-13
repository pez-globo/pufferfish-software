import { createSelector } from 'reselect';
import { StoreState } from '../types';
import { ConnectionState } from './types';

// Connection

export const getConnection = (store: StoreState): ConnectionState => store.connection;

// Backend

export const getbackendHeartBeat = createSelector(
  getConnection,
  (connection: ConnectionState): Date => connection.backendHeartbeat,
);
export const getBackendConnected = createSelector(
  getConnection,
  (connection: ConnectionState): boolean => connection.backendConnection,
);
export const getLastBackendConnectionTime = createSelector(
  getConnection,
  (connection: ConnectionState): Date | null => connection.lastBackendConnectionTime,
);
