import {
  MessageType,
  StateUpdateAction,
  PBMessage,
  STATE_UPDATED,
  PARAMETER_COMMITTED
} from './types'
import { AlarmLimits } from './proto/mcu_pb'

export function updateState(
  messageType: MessageType, state: PBMessage
): StateUpdateAction {
  return {type: STATE_UPDATED, messageType, state}
}

export function updateCommitedParameter(updates: Object) {  
  return {
    type: PARAMETER_COMMITTED, update: updates
  }
}
