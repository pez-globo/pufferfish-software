import { createSelector } from 'reselect';
import DECIMAL_RADIX from '../../modules/app/AppConstants';
import {
  // Alarm Limits
  Range,
  AlarmLimits,
  AlarmLimitsRequest,
  // Log Events
  LogEventCode,
  LogEvent,
  ExpectedLogEvent,
  // Alarm Muting
  AlarmMute,
  AlarmMuteRequest,
  // System Miscellaneous
  BackendConnections,
  ScreenStatus,
  ScreenStatusRequest,
} from '../proto/mcu_pb';
import { StoreState } from '../types';
import { AlarmLimitsRequestResponse, EventLog, ControllerStates } from './types';
import { SelectorType } from '../../modules/controllers/ValueSelectorDisplay';

export const roundValue = (value: number): number => {
  return value !== undefined && !Number.isNaN(value)
    ? parseInt(value.toFixed(0).replace(/^-0$/, '0'), DECIMAL_RADIX)
    : NaN;
};

export const getController = ({ controller }: StoreState): ControllerStates => controller;

// MESSAGE STATES FROM mcu_pb
// TODO: split this section off into a new file

// Alarm Limits

export const getAlarmLimits = createSelector(
  getController,
  (states: ControllerStates): AlarmLimitsRequestResponse => states.alarmLimits,
);
export const getAlarmLimitsCurrent = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimits | null => alarmLimits.current,
);
export const getAlarmLimitsRequest = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimitsRequest | null => alarmLimits.request,
);
export const getAlarmLimitsRequestDraft = createSelector(
  getAlarmLimits,
  (alarmLimits: AlarmLimitsRequestResponse): AlarmLimitsRequest | null => alarmLimits.draft,
);
export const getAlarmLimitsUnsavedKeys = createSelector(
  getAlarmLimitsRequest,
  getAlarmLimitsRequestDraft,
  (
    alarmLimitsRequest: AlarmLimitsRequest | null,
    alarmLimitsRequestDraft: AlarmLimitsRequest | null,
  ): string[] => {
    if (alarmLimitsRequest === null || alarmLimitsRequestDraft === null) {
      return [];
    }
    const alarmLimitsRequestRange = (alarmLimitsRequest as unknown) as Record<string, Range>;
    const alarmLimitsRequestDraftRange = (alarmLimitsRequestDraft as unknown) as Record<
      string,
      Range
    >;
    const keys = ['spo2', 'hr'];
    return keys.filter(
      (key: string) =>
        alarmLimitsRequestRange[key]?.lower !== alarmLimitsRequestDraftRange[key]?.lower ||
        alarmLimitsRequestRange[key]?.upper !== alarmLimitsRequestDraftRange[key]?.upper,
    );
  },
);

export const getAlarmLimitsRequestUnsaved = createSelector(
  getAlarmLimitsUnsavedKeys,
  (unsavedKeys: string[]): boolean => unsavedKeys.length > 0,
);

// Alarm Limits
// return a Range of alarmLimits for ValueInfo based on the stateKey
const alarmLimitsSelector = (selector: SelectorType, stateKey: string) =>
  createSelector(selector, (alarmLimits: AlarmLimits | null) => {
    const range =
      alarmLimits === null
        ? undefined
        : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
    const { lower, upper } = range === undefined ? { lower: 0, upper: 0 } : range;
    return { lower, upper };
  });

export const getSpO2AlarmLimitsCurrent = alarmLimitsSelector(getAlarmLimitsCurrent, 'spo2');
export const getHRAlarmLimitsCurrent = alarmLimitsSelector(getAlarmLimitsCurrent, 'hr');
export const getSpO2AlarmLimitsRequest = alarmLimitsSelector(getAlarmLimitsRequest, 'spo2');
export const getHRAlarmLimitsRequest = alarmLimitsSelector(getAlarmLimitsRequest, 'hr');
export const getFiO2AlarmLimitsCurrent = alarmLimitsSelector(getAlarmLimitsCurrent, 'fio2');
export const getFlowAlarmLimitsCurrent = alarmLimitsSelector(getAlarmLimitsCurrent, 'flow');
// Event log

export const getEventLog = createSelector(
  getController,
  (states: ControllerStates): EventLog => states.eventLog,
);
// Next log events
export const getNextLogEvents = createSelector(
  getEventLog,
  (eventLog: EventLog): LogEvent[] => eventLog.nextLogEvents.elements,
);

export const getExpectedLogEvent = createSelector(
  getEventLog,
  (eventLog: EventLog): number => eventLog.expectedLogEvent.id,
);
export const getFullExpectedLogEvent = createSelector(
  getEventLog,
  (eventLog: EventLog): ExpectedLogEvent => eventLog.expectedLogEvent,
);
// Active log events
export const getActiveLogEventIds = createSelector(
  getEventLog,
  (eventLog: EventLog): number[] => eventLog.activeLogEvents.id,
);
export const getNumActiveAlarms = createSelector(
  getActiveLogEventIds,
  (activeLogEventIds: number[]): number => activeLogEventIds.length,
);
export const getHasActiveAlarms = createSelector(
  getNumActiveAlarms,
  (numActiveAlarms: number): boolean => numActiveAlarms > 0,
);
export const getMostRecentEvent = createSelector(
  getActiveLogEventIds,
  getNextLogEvents,
  (activeLogEventIds: number[], nextLogEvents: LogEvent[]): LogEvent | undefined => {
    const maxId = Math.max(...activeLogEventIds);
    return nextLogEvents.find((el: LogEvent) => el.id === maxId);
  },
);
// Event log code selector
const getLogEventCode = (logEventCode: number) =>
  createSelector(getNextLogEvents, (events: LogEvent[]): LogEvent | undefined =>
    events.find((el: LogEvent) => (el.code as number) === logEventCode),
  );
// Note: this selector should only be used to check whether the frontend's event log
// has a temporary event indicating that the backend is down. The proper selector to
// check whether the backend is connected is store/app/selector.ts's getBackendConnected
export const getBackendDownEvent = getLogEventCode(LogEventCode.frontend_backend_connection_down);

// Alarm Muting

export const getAlarmMuteRequest = createSelector(
  getController,
  (states: ControllerStates): AlarmMuteRequest | null => states.alarmMute.request,
);
export const getAlarmMuteStatus = createSelector(
  getController,
  (states: ControllerStates): AlarmMute | null => states.alarmMute.current,
);
export const getAlarmMuteActive = createSelector(
  getAlarmMuteStatus,
  (alarmMute: AlarmMute | null): boolean => (alarmMute === null ? false : alarmMute.active),
);
export const getAlarmMuteSeqNum = createSelector(getAlarmMuteStatus, (alarmMute: AlarmMute | null):
  | number
  | null => (alarmMute === null ? null : alarmMute.seqNum));
export const getAlarmMuteRemaining = createSelector(
  getAlarmMuteStatus,
  (alarmMute: AlarmMute | null) => (alarmMute === null ? 0 : alarmMute.remaining),
);
export const getAlarmMuteRequestActive = createSelector(
  getAlarmMuteRequest,
  (alarmMuteRequest: AlarmMuteRequest | null) =>
    alarmMuteRequest === null ? false : alarmMuteRequest.active,
);

// Connection Statuses
export const getBackendConnections = createSelector(
  getController,
  (states: ControllerStates): BackendConnections | null => states.backendConnections,
);
// getBackendConnected is a selector defined in store/app/selectors.ts
export const getFirmwareConnected = createSelector(
  getBackendConnections,
  (backendConnections: BackendConnections | null): boolean =>
    backendConnections === null ? false : backendConnections.hasMcu,
);

// Screen Status
export const getScreenStatus = createSelector(
  getController,
  (states: ControllerStates): ScreenStatus | null => states.screenStatus.current,
);
export const getScreenStatusRequest = createSelector(
  getController,
  (states: ControllerStates): ScreenStatusRequest | null => states.screenStatus.request,
);
export const getScreenStatusLock = createSelector(
  getScreenStatus,
  (screenStatus: ScreenStatus | null): boolean =>
    screenStatus === null ? false : screenStatus.lock,
);
