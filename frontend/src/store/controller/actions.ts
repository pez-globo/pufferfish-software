import { MessageType, PBMessage } from '../proto/types';
import {
  StateUpdateAction,
  STATE_UPDATED,
  CommitAction,
  REQUEST_COMMITTED,
  DRAFT_REQUEST_COMMITTED,
} from './types';

// TODO: rename this to receiveMessage, and make SidebarClickable.tsx and OverlayScreen not use it.
// Instead, they should commit values to requests.
// TODO: updateState action creator should be located in store/connection, since that should be the
// only thing to dispatch it
export const updateState = (messageType: MessageType, state: PBMessage): StateUpdateAction => ({
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
