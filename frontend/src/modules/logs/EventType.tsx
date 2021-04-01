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
  const unit = eventType.unit === PERCENT ? eventType.unit : ` ${eventType.unit}`;
  if (event.type === LogEventType.patient) {
    if (eventType?.stateKey) {
      return eventType.label.includes('high')
        ? `Upper limit is ${alarmLimits[eventType.stateKey].upper}${unit}`
        : `Lower limit is ${alarmLimits[eventType.stateKey].lower}${unit}`;
    }
  } else if (event.type === LogEventType.control) {
    if (event.code === LogEventCode.ventilation_operation_changed) {
      if (event.newBool === true) {
        return 'Ventilation started';
      }
      if (event.newBool === false) {
        return 'Ventilation stopped';
      }
      return '';
    }
    if (event.oldFloat != null && event.newFloat != null) {
      return `${eventType.stateKey}: ${event.oldFloat}${unit} \u2794 ${event.newFloat}${unit}`;
    }
    return '';
  } else if (event.type === LogEventType.alarm_limits) {
    if (event.oldRange != null && event.newRange != null) {
      return `${eventType.stateKey}: [${event.oldRange.lower}${unit} - ${event.oldRange.upper}${unit}] \u2794 [${event.newRange.lower}${unit} - ${event.newRange.upper}${unit}]`;
    }
    return '';
  }
  return '';
};

export const getEventType = (code: LogEventCode): EventType => {
  switch (code) {
    // Patient
    case LogEventCode.fio2_too_low:
      return {
        type: LogEventType.patient,
        label: 'FiO2 is too low',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.fio2_too_high:
      return {
        type: LogEventType.patient,
        label: 'FiO2 is too high',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.spo2_too_low:
      return {
        type: LogEventType.patient,
        label: 'SpO2 is too low',
        stateKey: 'spo2',
        unit: PERCENT,
      };
    case LogEventCode.spo2_too_high:
      return {
        type: LogEventType.patient,
        label: 'SpO2 is too high',
        stateKey: 'spo2',
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
    // System
    case BACKEND_CONNECTION_LOST_CODE:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
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
    // Control
    case LogEventCode.ventilation_operation_changed:
      return {
        type: LogEventType.control,
        label: 'Ventilation',
        stateKey: 'ventilating',
        unit: '',
      };
    case LogEventCode.ventilation_mode_changed:
      return {
        type: LogEventType.control,
        label: 'Ventilation mode changed',
        stateKey: 'mode',
        unit: '',
      };
    case LogEventCode.fio2_setting_changed:
      return {
        type: LogEventType.control,
        label: 'FiO2 changed',
        stateKey: 'FiO2',
        unit: PERCENT,
      };
    case LogEventCode.flow_setting_changed:
      return {
        type: LogEventType.control,
        label: 'Flow Rate changed',
        stateKey: 'Flow',
        unit: LMIN,
      };
    // Alarm Limits
    case LogEventCode.fio2_alarm_limits_changed:
      return {
        type: LogEventType.alarm_limits,
        label: 'FiO2 limits changed',
        stateKey: 'FiO2',
        unit: PERCENT,
      };
    case LogEventCode.spo2_alarm_limits_changed:
      return {
        type: LogEventType.alarm_limits,
        label: 'SpO2 limits changed',
        stateKey: 'SpO2',
        unit: PERCENT,
      };
    case LogEventCode.hr_alarm_limits_changed:
      return {
        type: LogEventType.alarm_limits,
        label: 'Heart Rate limits changed',
        stateKey: 'HR',
        unit: BPM,
      };
    default:
      return { type: LogEventType.system, label: '', unit: '' };
  }
};

export default getEventType;
