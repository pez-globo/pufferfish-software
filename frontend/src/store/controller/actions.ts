import {
  MessageType,
  StateUpdateAction,
  PBMessage,
  STATE_UPDATED,
  PARAMETER_COMMITTED
} from './types'

export function updateState(
  messageType: MessageType, state: PBMessage
): StateUpdateAction {
  return {type: STATE_UPDATED, messageType, state}
}

export function updateCommittedParameter(updates: Object) {  
  return {
    type: PARAMETER_COMMITTED, update: updates
  }
}
