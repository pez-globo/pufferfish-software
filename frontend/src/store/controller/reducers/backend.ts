import { LogEvent, NextLogEvents, ExpectedLogEvent, ActiveLogEvents } from '../proto/mcu_pb';
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
  EventLog,
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

export const eventLogReducer = (
  state: EventLog = {
    nextLogEvents: NextLogEvents.fromJSON({}),
    expectedLogEvent: ExpectedLogEvent.fromJSON({ id: 0 }),
    activeLogEvents: ActiveLogEvents.fromJSON({}),
  },
  action: commitAction | StateUpdateAction,
): EventLog => {
  switch (action.type) {
    case STATE_UPDATED: {
      const actionClone = { ...(action as StateUpdateAction) };
      if (actionClone.messageType === MessageType.NextLogEvents) {
        const newEventState = actionClone.state as NextLogEvents;
        const oldEventState = state.nextLogEvents as NextLogEvents;
        if (!oldEventState || !oldEventState.elements.length) {
          // Initialize the elements list
          const numElements = newEventState.elements.length;
          const nextEventID = numElements ? newEventState.elements[numElements - 1].id + 1 : 0;
          return {
            ...state,
            nextLogEvents: newEventState as NextLogEvents,
            expectedLogEvent: { id: nextEventID },
          };
        }

        // Update the elements list with new ones
        const ids = new Set(oldEventState.elements.map((d) => d.id));
        const events = [
          ...oldEventState.elements.filter((d) => d.code !== BACKEND_CONNECTION_LOST_CODE),
          ...newEventState.elements.filter((d) => !ids.has(d.id)),
        ];
        const nextLogEvents = {
          ...newEventState,
          elements: events,
        };
        const numElements = nextLogEvents.elements.length;
        const nextEventID = numElements ? nextLogEvents.elements[numElements - 1].id + 1 : 0;
        return {
          ...state,
          nextLogEvents,
          expectedLogEvent: { id: nextEventID },
        };
      }
      if (actionClone.messageType === MessageType.ActiveLogEvents) {
        return {
          ...state,
          activeLogEvents: actionClone.state as ActiveLogEvents,
        };
      }
      return state;
    }
    case BACKEND_CONNECTION_LOST: {
      // Make an ephemeral frontend-only event
      const logEvent = (action.update as unknown) as LogEvent;
      return {
        ...state,
        nextLogEvents: {
          ...state.nextLogEvents,
          elements: [
            ...state.nextLogEvents.elements,
            {
              ...logEvent,
              id: state.expectedLogEvent.id,
            },
          ],
        },
        activeLogEvents: {
          id: [...state.activeLogEvents.id, state.expectedLogEvent.id],
        },
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
