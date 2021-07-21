import { AppState } from './app/types';
import { ControllerStates } from './controller/types';
import { ConnectionState } from './connection/types';

export interface StoreState {
  app: AppState;
  controller: ControllerStates;
  connection: ConnectionState;
}
