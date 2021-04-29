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
  commitAction,
  EventLog,
} from '../types';

export const messageReducer = <T extends PBMessage>(
  messageType: MessageType,
  pbMessageType: PBMessageType,
  initializer: Record<string, unknown> = {},
) => (state: T = pbMessageType.fromJSON(initializer) as T, action: StateUpdateAction): T => {
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
    ephemeralLogEvents: { id: [] },
  },
  action: commitAction | StateUpdateAction,
): EventLog => {
  switch (action.type) {
    case STATE_UPDATED: {
      const updateAction = action as StateUpdateAction;
      switch (updateAction.messageType) {
        case MessageType.ActiveLogEvents:
          return {
            ...state,
            activeLogEvents: updateAction.state as ActiveLogEvents,
          };
        case MessageType.NextLogEvents: {
          const newNextLogEvents = updateAction.state as NextLogEvents;
          if (!state.nextLogEvents || state.nextLogEvents.elements.length === 0) {
            // Initialize the elements list from the action's list
            const numElements = newNextLogEvents.elements.length;
            const nextEventID = numElements ? newNextLogEvents.elements[numElements - 1].id + 1 : 0;
            return {
              ...state,
              nextLogEvents: newNextLogEvents as NextLogEvents,
              expectedLogEvent: { id: nextEventID },
              ephemeralLogEvents: { id: [] }, // any ephemeral events are now overwritten
            };
          }

          // Add any new events to the elements list
          const ephemeralIDs = new Set(state.ephemeralLogEvents.id);
          const nextLogEvents = {
            ...newNextLogEvents,
            elements: [
              ...state.nextLogEvents.elements.filter(
                // Discard ephemeral events and any stored events at/past expected ID
                (d) => !ephemeralIDs.has(d.id) && d.id < state.expectedLogEvent.id,
              ),
              // Discard received events below expected ID
              ...newNextLogEvents.elements.filter((d) => d.id >= state.expectedLogEvent.id),
            ],
          };
          const numElements = nextLogEvents.elements.length;
          const nextEventID = numElements ? nextLogEvents.elements[numElements - 1].id + 1 : 0;
          return {
            nextLogEvents,
            expectedLogEvent: { id: nextEventID },
            // Deactivate active ephemeral events
            activeLogEvents: { id: state.activeLogEvents.id.filter((d) => !ephemeralIDs.has(d)) },
            ephemeralLogEvents: { id: [] },
          };
        }
        default:
          return state;
      }
    }
    case BACKEND_CONNECTION_LOST: {
      // Make an ephemeral frontend-only event
      const logEvent = (action.update as unknown) as LogEvent;
      const eventID = state.expectedLogEvent.id;
      return {
        ...state,
        nextLogEvents: {
          ...state.nextLogEvents,
          elements: [...state.nextLogEvents.elements, { ...logEvent, id: eventID }],
        },
        activeLogEvents: { id: [...state.activeLogEvents.id, eventID] },
        ephemeralLogEvents: { id: [...state.ephemeralLogEvents.id, eventID] },
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
