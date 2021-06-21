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
  unit?: string;
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
      return event.newBool ? 'Ventilation started' : 'Ventilation stopped';
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
    const eventDetailsMap = new Map<LogEventCode, string>([
      [LogEventCode.mcu_backend_connection_down, 'Hardware controller lost connection'],
      [
        LogEventCode.backend_mcu_connection_down,
        'Backend server lost connection from hardware controller',
      ],
      [LogEventCode.backend_frontend_connection_down, 'User interface lost connection'],
      [LogEventCode.frontend_backend_connection_down, 'User interface lost connection'],
      [LogEventCode.mcu_backend_connection_up, 'Hardware controller connected'],
      [LogEventCode.backend_frontend_connection_up, 'User interface connected'],
      [LogEventCode.charger_disconnected, 'Battery charger is disconnected'],
      [LogEventCode.battery_low, `Less than 30${unit} battery remaining`],
      [LogEventCode.battery_critical, `Less than 5${unit} battery remaining`],
    ]);
    const eventDetails = eventDetailsMap.get(event.code);
    if (eventDetails === undefined) {
      return '';
    }
    return eventDetails;
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
  const EventTypeMap = new Map<LogEventCode, EventType>([
    [
      LogEventCode.fio2_too_low,
      {
        type: LogEventType.patient,
        label: 'FiO2 is too low',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.fio2_too_high,
      {
        type: LogEventType.patient,
        label: 'FiO2 is too high',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.flow_too_low,
      {
        type: LogEventType.patient,
        label: 'Flow Rate is too low',
        stateKey: 'flow',
        unit: LMIN,
      },
    ],
    [
      LogEventCode.flow_too_high,
      {
        type: LogEventType.patient,
        label: 'Flow Rate is too high',
        stateKey: 'flow',
        unit: LMIN,
      },
    ],
    [
      LogEventCode.spo2_too_low,
      {
        type: LogEventType.patient,
        label: 'SpO2 is too low',
        stateKey: 'spo2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.spo2_too_high,
      {
        type: LogEventType.patient,
        label: 'SpO2 is too high',
        stateKey: 'spo2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.hr_too_low,
      {
        type: LogEventType.patient,
        label: 'Heart Rate is too low',
        stateKey: 'hr',
        unit: BPM,
      },
    ],
    [
      LogEventCode.hr_too_high,
      {
        type: LogEventType.patient,
        label: 'Heart Rate is too high',
        stateKey: 'hr',
        unit: BPM,
      },
    ],
    [
      LogEventCode.ventilation_operation_changed,
      {
        type: LogEventType.control,
        label: 'Ventilation',
        stateKey: 'ventilating',
      },
    ],
    [
      LogEventCode.ventilation_mode_changed,
      {
        type: LogEventType.control,
        label: 'Ventilation mode changed',
        stateKey: 'mode',
      },
    ],
    [
      LogEventCode.fio2_setting_changed,
      {
        type: LogEventType.control,
        label: 'FiO2 changed',
        stateKey: 'FiO2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.flow_setting_changed,
      {
        type: LogEventType.control,
        label: 'Flow Rate changed',
        stateKey: 'Flow',
        unit: LMIN,
      },
    ],
    [
      LogEventCode.fio2_alarm_limits_changed,
      {
        type: LogEventType.alarm_limits,
        label: 'FiO2 limits changed',
        stateKey: 'FiO2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.flow_alarm_limits_changed,
      {
        type: LogEventType.alarm_limits,
        label: 'Flow limits changed',
        stateKey: 'Flow',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.spo2_alarm_limits_changed,
      {
        type: LogEventType.alarm_limits,
        label: 'SpO2 limits changed',
        stateKey: 'SpO2',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.hr_alarm_limits_changed,
      {
        type: LogEventType.alarm_limits,
        label: 'Heart Rate limits changed',
        stateKey: 'HR',
        unit: BPM,
      },
    ],
    [
      LogEventCode.screen_locked,
      {
        type: LogEventType.system,
        label: 'Screen is locked',
      },
    ],
    [
      LogEventCode.mcu_backend_connection_down,
      {
        type: LogEventType.system,
        label: 'Software connectivity lost',
      },
    ],
    [
      LogEventCode.backend_mcu_connection_down,
      {
        type: LogEventType.system,
        label: 'Software connectivity lost',
      },
    ],
    [
      LogEventCode.backend_frontend_connection_down,
      {
        type: LogEventType.system,
        label: 'Software connectivity lost',
      },
    ],
    [
      LogEventCode.frontend_backend_connection_down,
      {
        type: LogEventType.system,
        label: 'Software connectivity lost',
      },
    ],
    [
      LogEventCode.mcu_backend_connection_up,
      {
        type: LogEventType.system,
        label: 'Software connected',
      },
    ],
    [
      LogEventCode.backend_frontend_connection_up,
      {
        type: LogEventType.system,
        label: 'Software connected',
      },
    ],
    [
      LogEventCode.battery_low,
      {
        type: LogEventType.system,
        label: 'Battery power is low',
        unit: PERCENT,
      },
    ],
    [
      LogEventCode.charger_disconnected,
      {
        type: LogEventType.system,
        label: 'Charger is disconnected',
      },
    ],
    [
      LogEventCode.sfm3019_air_disconnected,
      {
        type: LogEventType.system,
        label: 'Air sensor connectivity lost',
      },
    ],
    [
      LogEventCode.sfm3019_o2_disconnected,
      {
        type: LogEventType.system,
        label: 'O2 sensor connectivity lost',
      },
    ],
    [
      LogEventCode.fdo2_disconnected,
      {
        type: LogEventType.system,
        label: 'FIO2 sensor connectivity lost',
      },
    ],
    [
      LogEventCode.battery_critical,
      {
        type: LogEventType.system,
        label: 'Battery power critically low',
        unit: PERCENT,
      },
    ],
  ]);
  const eventType = EventTypeMap.get(code);
  if (eventType === undefined) {
    return { type: LogEventType.system, label: '' };
  }
  return eventType;
};

export default getEventType;
