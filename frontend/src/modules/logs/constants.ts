/**
 * @summary Constants for LogEvents
 */

import { LogEventCode, LogEventType } from '../../store/proto/mcu_pb';
import { PERCENT, LMIN, BPM } from '../info/units';
import { EventType } from './EventType';

export const eventDetailsMap = new Map<LogEventCode, string>([
  // System settings & alarms
  [
    LogEventCode.mcu_backend_connection_down,
    'Hardware controller lost connection from backend server',
  ],
  [
    LogEventCode.backend_mcu_connection_down,
    'Backend server lost connection from hardware controller',
  ],
  [
    LogEventCode.backend_frontend_connection_down,
    'Backend server lost lost connection from user interface',
  ],
  [
    LogEventCode.frontend_backend_connection_down,
    'User interface lost connection from backend server',
  ],
  [LogEventCode.mcu_backend_connection_up, 'Hardware controller connected to backend server'],
  [LogEventCode.backend_mcu_connection_up, 'Backend server connected to hardware controller'],
  [LogEventCode.backend_frontend_connection_up, 'Backend server connected to backend server'],
  [LogEventCode.frontend_backend_connection_up, 'User interface connected to backend server'],
  [LogEventCode.battery_low, `Less than 30${PERCENT} battery remaining`],
  [LogEventCode.battery_critical, `Less than 5${PERCENT} battery remaining`],
  [LogEventCode.charger_disconnected, 'Battery charger is disconnected'],
  // Alarm muting/unmuting
  [LogEventCode.alarms_muted_user_software, 'Because of display mute toggle button'],
  [LogEventCode.alarms_muted_user_hardware, 'Because of hardware mute toggle button'],
  [LogEventCode.alarms_unmuted_user_software, 'Because of display mute toggle button'],
  [LogEventCode.alarms_unmuted_user_hardware, 'Because of hardware mute toggle button'],
  [LogEventCode.alarms_unmuted_timeout, 'Because of automatic timeout'],
  [LogEventCode.alarms_unmuted_mcu_backend_loss, 'Because hardware controller lost connection'],
  [
    LogEventCode.alarms_unmuted_backend_mcu_loss,
    'Because backend server lost connection from hardware controller',
  ],
  [LogEventCode.alarms_unmuted_backend_frontend_loss, 'Because user interface lost connection'],
  [LogEventCode.alarms_unmuted_frontend_backend_loss, 'Because user interface lost connection'],
  // Sensor loss
  [LogEventCode.sfm3019_air_disconnected, 'Air flow sensor connection lost'],
  [LogEventCode.sfm3019_o2_disconnected, 'O2 flow sensor connection lost'],
  [LogEventCode.fdo2_disconnected, 'O2 concentration sensor connection lost'],
  [LogEventCode.nonin_disconnected, 'SpO2 and HR sensor connection lost'],
  [LogEventCode.nonin_finger_sensor_disconnected, 'SpO2 and HR sensor unplugged'],
  [LogEventCode.nonin_sensor_alarm, 'SpO2 and HR sensor providing unusable data'],
  [LogEventCode.nonin_out_of_track_measurements, 'SpO2 and HR sensor not detecting finger'],
]);

export const EventTypeMap = new Map<LogEventCode, EventType>([
  // Patient alarms
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
  // Control settings
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
  // Alarm limits settings
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
  // System settings & alarms
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
      label: 'Software connection lost',
    },
  ],
  [
    LogEventCode.backend_mcu_connection_down,
    {
      type: LogEventType.system,
      label: 'Software connection lost',
    },
  ],
  [
    LogEventCode.backend_frontend_connection_down,
    {
      type: LogEventType.system,
      label: 'Software connection lost',
    },
  ],
  [
    LogEventCode.frontend_backend_connection_down,
    {
      type: LogEventType.system,
      label: 'Software connection lost',
    },
  ],
  [
    LogEventCode.mcu_backend_connection_up,
    {
      type: LogEventType.system,
      label: 'Software connection established',
    },
  ],
  [
    LogEventCode.backend_frontend_connection_up,
    {
      type: LogEventType.system,
      label: 'Software connection established',
    },
  ],
  [
    LogEventCode.frontend_backend_connection_up,
    {
      type: LogEventType.system,
      label: 'Software connection established',
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
    LogEventCode.battery_critical,
    {
      type: LogEventType.system,
      label: 'Critical battery',
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
    LogEventCode.mcu_started,
    {
      type: LogEventType.system,
      label: 'Software started',
    },
  ],
  [
    LogEventCode.backend_started,
    {
      type: LogEventType.system,
      label: 'Software started',
    },
  ],
  [
    LogEventCode.mcu_shutdown,
    {
      type: LogEventType.system,
      label: 'Software shutting down',
    },
  ],
  [
    LogEventCode.backend_shutdown,
    {
      type: LogEventType.system,
      label: 'Software shutting down',
    },
  ],
  [
    LogEventCode.sysclock_changed,
    {
      type: LogEventType.system,
      label: 'System clock changed',
    },
  ],
  // Alarm muting/unmuting
  [
    LogEventCode.alarms_muted_user_software,
    {
      type: LogEventType.system,
      label: 'Temporarily muting alarms',
    },
  ],
  [
    LogEventCode.alarms_muted_user_hardware,
    {
      type: LogEventType.system,
      label: 'Temporarily muting alarms',
    },
  ],
  [
    LogEventCode.alarms_muted_unknown,
    {
      type: LogEventType.system,
      label: 'Temporarily muting alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_user_software,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_user_hardware,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_initialization,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_timeout,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_mcu_backend_loss,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_backend_mcu_loss,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_backend_frontend_loss,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_frontend_backend_loss,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  [
    LogEventCode.alarms_unmuted_unknown,
    {
      type: LogEventType.system,
      label: 'Unmuted alarms',
    },
  ],
  // Sensor loss
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
    LogEventCode.nonin_disconnected,
    {
      type: LogEventType.system,
      label: 'Internal sensor failed',
    },
  ],
  [
    LogEventCode.nonin_finger_sensor_disconnected,
    {
      type: LogEventType.system,
      label: 'Sensor unplugged',
    },
  ],
  [
    LogEventCode.nonin_sensor_alarm,
    {
      type: LogEventType.system,
      label: 'External sensor failed',
    },
  ],
  [
    LogEventCode.nonin_out_of_track_measurements,
    {
      type: LogEventType.system,
      label: 'External sensor disconnected',
    },
  ],
]);
