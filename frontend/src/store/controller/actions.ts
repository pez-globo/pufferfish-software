import { LogEventCode, LogEventType } from '../proto/mcu_pb';
import { MessageType, PBMessage } from '../proto/types';
import {
  StateUpdateAction,
  STATE_UPDATED,
  CommitAction,
  REQUEST_COMMITTED,
  DRAFT_REQUEST_COMMITTED,
  EphemeralLogEventAction,
  EPHEMERAL_LOG_EVENT_CREATED,
} from './types';

// TODO: rename this to receiveMessage, and make SidebarClickable.tsx and OverlayScreen not use it.
// Instead, they should commit values to requests.
// TODO: updateState action creator should be located in store/connection, since that should be the
// only thing to dispatch it
export const receiveMessage = (messageType: MessageType, state: PBMessage): StateUpdateAction => ({
  type: STATE_UPDATED,
  messageType,
  state,
});

export const commitRequest = <T extends PBMessage>(
  requestMessageType: MessageType,
  updates: Partial<T>,
): CommitAction => ({
  type: REQUEST_COMMITTED,
  messageType: requestMessageType,
  update: updates,
});

export const commitDraftRequest = <T extends PBMessage>(
  requestMessageType: MessageType,
  updates: Partial<T>,
): CommitAction => ({
  type: DRAFT_REQUEST_COMMITTED,
  messageType: requestMessageType,
  update: updates,
});

export function createEphemeralLogEvent(
  code: LogEventCode,
  type: LogEventType,
): EphemeralLogEventAction {
  return {
    type: EPHEMERAL_LOG_EVENT_CREATED,
    code,
    eventType: type,
    time: new Date().getTime(),
  };
}
