import {
  LogEvent,
  NextLogEvents,
  ExpectedLogEvent,
  ActiveLogEvents,
  LogEventCode,
  LogEventType,
} from '../../proto/mcu_pb';
import { RotaryEncoder } from '../../proto/frontend_pb';
import { MessageType, PBMessage } from '../../proto/types';
import {
  StateUpdateAction,
  EventLog,
  RotaryEncoderParameter,
  STATE_UPDATED,
  CommitAction,
  EphemeralLogEventAction,
  EPHEMERAL_LOG_EVENT_CREATED,
} from '../types';
import { ConnectionAction, BACKEND_CONNECTION_DOWN } from '../../connection/types';

// GENERIC REDUCERS

export const messageReducer = <T extends PBMessage>(messageType: MessageType) => (
  state: T | null = null,
  action: StateUpdateAction,
): T | null => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType !== messageType) {
        // ignore irrelevant messages
        return state;
      }

      return action.state as T;
    default:
      return state;
  }
};

export const requestReducer = <T extends PBMessage>(
  requestMessageType: MessageType,
  actionType: string,
) => (state: T | null = null, action: StateUpdateAction | CommitAction): T | null => {
  switch (action.type) {
    case STATE_UPDATED: {
      const updateAction = action as StateUpdateAction;
      if (updateAction.messageType !== requestMessageType) {
        // ignore irrelevant messages
        return state;
      }

      if (state === null) {
        // initialize the store with the backend's value...
        return updateAction.state as T;
      }

      // ...but ignore the backend otherwise
      return state;
    }
    case actionType: {
      const commitAction = action as CommitAction;
      if (commitAction.messageType !== requestMessageType) {
        // ignore irrelevant messages
        return state;
      }

      return { ...state, ...commitAction.update } as T;
    }
    default:
      return state;
  }
};

// MESSAGE STATES FROM mcu_pb

const addEphemeralEvent = (state: EventLog, logEvent: LogEvent): EventLog => {
  //  TODO: check if event is already there in state.ephemeralLogEvents before creating a new one
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
};

export const eventLogReducer = (
  state: EventLog = {
    nextLogEvents: NextLogEvents.fromJSON({}),
    expectedLogEvent: ExpectedLogEvent.fromJSON({ id: 0 }),
    activeLogEvents: ActiveLogEvents.fromJSON({}),
    ephemeralLogEvents: { id: [] },
  },
  action: CommitAction | StateUpdateAction | ConnectionAction | EphemeralLogEventAction,
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
              // The backend's event log is persistent, so its session ID is always 0
              expectedLogEvent: { id: nextEventID, sessionId: 0 },
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
            expectedLogEvent: { id: nextEventID, sessionId: 0 },
            // Deactivate active ephemeral events
            activeLogEvents: { id: state.activeLogEvents.id.filter((d) => !ephemeralIDs.has(d)) },
            ephemeralLogEvents: { id: [] },
          };
        }
        default:
          return state;
      }
    }
    case BACKEND_CONNECTION_DOWN: {
      // Make an ephemeral frontend-only event
      // This is handled separately from EPHEMERAL_LOG_EVENT_CREATED because
      // store/connection can dispatch the event, and it shouldn't be aware of
      // the existence of ephemeral log events or any event log.
      // Note: this reducer will create multiple ephemeral log events when multiple
      // BACKEND_CONNECTION_DOWN actions are dispatched when connection goes down.
      // Refer to the TODO comment in addEphemeralEvent.
      const connectionAction = action as ConnectionAction;
      return addEphemeralEvent(
        state,
        LogEvent.fromJSON({
          code: LogEventCode.frontend_backend_connection_down,
          type: LogEventType.system,
          time: connectionAction.lastBackendConnectionTime,
        }),
      );
    }
    case EPHEMERAL_LOG_EVENT_CREATED: {
      // Make an ephemeral frontend-only event
      const updateAction = action as EphemeralLogEventAction;
      return addEphemeralEvent(
        state,
        LogEvent.fromJSON({
          code: updateAction.code,
          type: updateAction.eventType,
          time: updateAction.time,
        }),
      );
    }
    default:
      return state;
  }
};

// MESSAGE STATES FROM frontend_pb

export const rotaryEncoderReducer = (
  state: RotaryEncoderParameter | null = null,
  action: StateUpdateAction,
): RotaryEncoderParameter | null => {
  switch (action.type) {
    case STATE_UPDATED: {
      if (action.messageType !== MessageType.RotaryEncoder) {
        // ignore irrelevant messages
        return state;
      }

      const newState = action.state as RotaryEncoder;
      if (state === null) {
        // initialize the store with the backend's value
        return { ...newState, stepDiff: 0 };
      }

      const oldState = state as RotaryEncoder;
      const stepDiff = newState.step - oldState.step;
      return { ...newState, stepDiff };
    }
    default:
      return state;
  }
};
