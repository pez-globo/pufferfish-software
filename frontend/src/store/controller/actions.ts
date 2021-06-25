import {
  MessageType,
  PBMessage,
  StateUpdateAction,
  STATE_UPDATED,
  CommitAction,
  REQUEST_COMMITTED,
  DRAFT_REQUEST_COMMITTED,
} from './types';

// TODO: rename this to receiveMessage, and make SidebarClickable.tsx and OverlayScreen not use it.
// Instead, they should commit values to requests.
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
