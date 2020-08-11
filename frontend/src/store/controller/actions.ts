import {
  MessageType,
  StateUpdateAction,
  PBMessage,
  STATE_UPDATED
} from './types'

export function updateState(
  messageType: MessageType, state: PBMessage
): StateUpdateAction {
  return {type: STATE_UPDATED, messageType, state}
}
