import { LogEvent, NextLogEvents } from '../proto/mcu_pb';
import { RotaryEncoder } from '../proto/frontend_pb';
import {
  MessageType,
  PBMessage,
  PBMessageType,
  StateUpdateAction,
  STATE_UPDATED,
  RotaryEncoderParameter,
  BACKEND_CONNECTION_LOST,
  BACKEND_CONNECTION_LOST_CODE,
  commitAction,
} from '../types';

export const messageReducer = <T extends PBMessage>(
  messageType: MessageType,
  pbMessageType: PBMessageType,
) => (state: T = pbMessageType.fromJSON({}) as T, action: StateUpdateAction): T => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === messageType) {
        return action.state as T;
      }
      return state;
    default:
      return state;
  }
};

export const nextLogEventsReducer = (
  state: NextLogEvents = NextLogEvents.fromJSON({}),
  action: commitAction | StateUpdateAction,
): NextLogEvents => {
  switch (action.type) {
    case STATE_UPDATED: {
      const actionClone = { ...(action as StateUpdateAction) };
      if (actionClone.messageType === MessageType.NextLogEvents) {
        const newEventState = actionClone.state as NextLogEvents;
        const oldEventState = state as NextLogEvents;
        if (!oldEventState || !oldEventState.elements.length) {
          return newEventState as NextLogEvents;
        }
        const ids = new Set(oldEventState.elements.map((d) => d.id));
        const events = [
          ...oldEventState.elements.filter((d) => d.code !== BACKEND_CONNECTION_LOST_CODE),
          ...newEventState.elements.filter((d) => !ids.has(d.id)),
        ];
        return {
          ...newEventState,
          elements: events,
        };
      }
      return state;
    }
    case BACKEND_CONNECTION_LOST: {
      const logEvent = (action.update as unknown) as LogEvent;
      logEvent.id = state.elements.length ? state.elements.length + 1 : 1;
      state.elements.push(logEvent);
      return {
        ...state,
        nextExpected: logEvent.id + 1,
        elements: state.elements,
      };
    }
    default:
      return state;
  }
};

export const rotaryEncoderReducer = (
  state: RotaryEncoder = RotaryEncoder.fromJSON({}),
  action: StateUpdateAction,
): RotaryEncoderParameter => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === MessageType.RotaryEncoder) {
        const newState = action.state as RotaryEncoder;
        const oldState = state as RotaryEncoder;
        const stepDiff = newState.step - oldState.step;
        const stateCopy = { ...newState } as RotaryEncoderParameter;
        stateCopy.stepDiff = stepDiff;
        return stateCopy;
      }
      return state as RotaryEncoderParameter;
    default:
      return state as RotaryEncoderParameter;
  }
};
