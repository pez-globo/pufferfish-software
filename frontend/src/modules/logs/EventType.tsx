import { LogEvent, LogEventCode, LogEventType, Range } from '../../store/controller/proto/mcu_pb';
import { BACKEND_CONNECTION_LOST_CODE } from '../../store/controller/types';
import { PERCENT, BMIN, BPM, LMIN } from '../info/units';

export interface EventType {
  type: LogEventType;
  label: string;
  unit: string;
  head?: string;
  stateKey?: string;
}

export const getEventDetails = (
  event: LogEvent,
  eventType: EventType,
  alarmLimits: Record<string, Range>,
): string => {
  if (event.type === LogEventType.patient) {
    if (eventType?.stateKey) {
      return eventType.label.includes('high')
        ? `Upper limit of ${eventType?.stateKey} is ${alarmLimits[eventType.stateKey].upper}`
        : `Lower limit of ${eventType?.stateKey} is ${alarmLimits[eventType.stateKey].lower}`;
    }
  } else if (event.type === LogEventType.control) {
    return event.oldValue != null && event.newValue != null
      ? `Control ${eventType?.stateKey} changed from ${event.oldValue} ${eventType.unit} to ${event.newValue} ${eventType.unit}`
      : '';
  }
  return '';
};

export const getEventType = (code: LogEventCode): EventType => {
  switch (code) {
    case LogEventCode.fio2_too_low:
      return {
        type: LogEventType.patient,
        label: 'fiO2 is too low',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.fio2_too_high:
      return {
        type: LogEventType.patient,
        label: 'fiO2 is too high',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.rr_too_low:
      return {
        type: LogEventType.patient,
        label: 'Respiratory Rate is too low',
        stateKey: 'rr',
        unit: BMIN,
      };
    case LogEventCode.rr_too_high:
      return {
        type: LogEventType.patient,
        label: 'Respiratory Rate is too high',
        stateKey: 'rr',
        unit: BMIN,
      };
    case LogEventCode.hr_too_low:
      return {
        type: LogEventType.patient,
        label: 'Heart Rate is too low',
        stateKey: 'hr',
        unit: BPM,
      };
    case LogEventCode.hr_too_high:
      return {
        type: LogEventType.patient,
        label: 'Heart Rate is too high',
        stateKey: 'hr',
        unit: BPM,
      };
    case LogEventCode.spo2_too_low:
      return {
        type: LogEventType.patient,
        label: 'spO2 is too low',
        stateKey: 'spo2',
        unit: PERCENT,
      };
    case LogEventCode.spo2_too_high:
      return {
        type: LogEventType.patient,
        label: 'spO2 is too high',
        stateKey: 'spo2',
        unit: PERCENT,
      };
    case LogEventCode.fio2_setting_changed:
      return {
        type: LogEventType.control,
        label: 'Fio2 Settings changed',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.flow_setting_changed:
      return {
        type: LogEventType.control,
        label: 'Flow Settings changed',
        stateKey: 'flow',
        unit: LMIN,
      };
    case LogEventCode.battery_low:
      return {
        type: LogEventType.system,
        label: 'Battery power is low',
        unit: PERCENT,
      };
    case LogEventCode.screen_locked:
      return {
        type: LogEventType.system,
        label: 'Screen is locked',
        unit: '',
      };
    case BACKEND_CONNECTION_LOST_CODE:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    default:
      return { type: LogEventType.system, label: '', unit: '' };
  }
};

export default getEventType;
