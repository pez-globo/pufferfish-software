/**
 * @summary Components for Event Type & Event Alarm Details
 *
 */
import { LogEvent, LogEventCode, LogEventType } from '../../store/controller/proto/mcu_pb';
import { PERCENT, BPM, LMIN } from '../info/units';

/**
 * @typedef EventType
 *
 * Interface to get data about event.
 *
 * @prop {LogEventType} type  Event log type
 * @prop {string} label Event Alarm Label
 * @prop {string} unit Unit measurement
 * @prop {string} head Header text
 * @prop {string} stateKey unit of stateKey
 *
 */
export interface EventType {
  type: LogEventType;
  label: string;
  unit: string;
  head?: string;
  stateKey?: string;
}

/**
 * function to get event details
 *
 * @param {LogEvent} event Event log object
 * @param {EventType} eventType Event log Type
 *
 * @returns {string} Event log details
 *
 */
export const getEventDetails = (event: LogEvent, eventType: EventType): string => {
  const unit = eventType.unit === PERCENT ? eventType.unit : ` ${eventType.unit}`;
  if (event.type === LogEventType.patient) {
    const alarmLimits = event?.alarmLimits;
    if (eventType?.stateKey && alarmLimits) {
      return eventType.label.includes('high')
        ? `Upper limit is ${alarmLimits.upper}${unit}`
        : `Lower limit is ${alarmLimits.lower}${unit}`;
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
  } else if (event.type === LogEventType.system) {
    if (event.code === LogEventCode.mcu_backend_connection_down) {
      return 'Hardware controller lost connection';
    }
    if (event.code === LogEventCode.backend_mcu_connection_down) {
      return 'Backend server lost connection from hardware controller';
    }
    if (event.code === LogEventCode.backend_frontend_connection_down) {
      return 'User interface lost connection';
    }
    if (event.code === LogEventCode.frontend_backend_connection_down) {
      return 'User interface lost connection';
    }
    if (event.code === LogEventCode.mcu_backend_connection_up) {
      return 'Hardware controller connected';
    }
    if (event.code === LogEventCode.backend_frontend_connection_up) {
      return 'User interface connected';
    }
    if (event.code === LogEventCode.charger_disconnected) {
      return 'Battery charger is disconnected';
    }
  }
  return '';
};

/**
 * function to get event type data
 *
 * @param {LogEventCode} code Event log code
 *
 * @returns {EventType}
 *
 */

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
    case LogEventCode.flow_too_low:
      return {
        type: LogEventType.patient,
        label: 'Flow Rate is too low',
        stateKey: 'flow',
        unit: LMIN,
      };
    case LogEventCode.flow_too_high:
      return {
        type: LogEventType.patient,
        label: 'Flow Rate is too high',
        stateKey: 'flow',
        unit: LMIN,
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
    case LogEventCode.flow_alarm_limits_changed:
      return {
        type: LogEventType.alarm_limits,
        label: 'Flow limits changed',
        stateKey: 'Flow',
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
    // System
    case LogEventCode.screen_locked:
      return {
        type: LogEventType.system,
        label: 'Screen is locked',
        unit: '',
      };
    case LogEventCode.mcu_backend_connection_down:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    case LogEventCode.backend_mcu_connection_down:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    case LogEventCode.backend_frontend_connection_down:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    case LogEventCode.frontend_backend_connection_down:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    case LogEventCode.mcu_backend_connection_up:
      return {
        type: LogEventType.system,
        label: 'Software connected',
        unit: '',
      };
    case LogEventCode.backend_frontend_connection_up:
      return {
        type: LogEventType.system,
        label: 'Software connected',
        unit: '',
      };
    case LogEventCode.battery_low:
      return {
        type: LogEventType.system,
        label: 'Battery power is low',
        unit: PERCENT,
      };
    case LogEventCode.charger_disconnected:
      return {
        type: LogEventType.system,
        label: 'Charger is disconnected',
        unit: '',
      };
    default:
      return { type: LogEventType.system, label: '', unit: '' };
  }
};

export default getEventType;
