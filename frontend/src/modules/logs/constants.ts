/**
 * @summary Constants for LogEvents
 */

import { LogEventCode, LogEventType } from '../../store/proto/mcu_pb';
import { PERCENT, LMIN, BPM } from '../info/units';
import { EventType } from './EventType';

export const eventDetailsMap = new Map<LogEventCode, string>([
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
  [LogEventCode.battery_low, `Less than 30${PERCENT} battery remaining`],
  [LogEventCode.battery_critical, `Less than 5${PERCENT} battery remaining`],
  [LogEventCode.sfm3019_air_disconnected, 'Air sensor connection lost'],
  [LogEventCode.sfm3019_o2_disconnected, 'O2 sensor connection lost'],
]);

export const EventTypeMap = new Map<LogEventCode, EventType>([
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
      label: 'Low battery',
      unit: PERCENT,
    },
  ],
  [
    LogEventCode.charger_disconnected,
    {
      type: LogEventType.system,
      label: 'Wall Power lost',
    },
  ],
  [
    LogEventCode.sfm3019_air_disconnected,
    {
      type: LogEventType.system,
      label: 'Internal sensor failed',
    },
  ],
  [
    LogEventCode.sfm3019_o2_disconnected,
    {
      type: LogEventType.system,
      label: 'Internal sensor failed',
    },
  ],
  [
    LogEventCode.fdo2_disconnected,
    {
      type: LogEventType.system,
      label: 'Internal sensor failed',
    },
  ],
  [
    LogEventCode.battery_critical,
    {
      type: LogEventType.system,
      label: 'Critical battery',
      unit: PERCENT,
    },
  ],
]);
