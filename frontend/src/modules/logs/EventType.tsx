/**
 * @summary Components for Event Type & Event Alarm Details
 *
 */
import { LogEvent, LogEventCode, LogEventType } from '../../store/proto/mcu_pb';
import { PERCENT } from '../info/units';
import { eventDetailsMap, EventTypeMap } from './constants';

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
    const eventDetails = eventDetailsMap.get(event.code);
    if (event.code === LogEventCode.sysclock_changed) {
      return `${new Date(event.oldUint32).toLocaleString()} \u2794 ${new Date(event.newUint32).toLocaleString()}`;
    } else if (eventDetails === undefined) {
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
  const eventType = EventTypeMap.get(code);
  if (eventType === undefined) {
    return { type: LogEventType.system, label: '' };
  }
  return eventType;
};

export default getEventType;
