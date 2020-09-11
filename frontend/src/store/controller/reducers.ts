import { combineReducers, Action } from 'redux';
import {
  Alarms,
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  Ping,
  Announcement,
  AlarmLimitsRequest,
  VentilationMode,
  ThemeVariant,
  Unit,
  FrontendDisplaySetting,
  SystemSettingRequest,
} from './proto/mcu_pb';
import { RotaryEncoder } from './proto/frontend_pb';
import {
  MessageType,
  ParameterCommitAction,
  PARAMETER_COMMITTED,
  PBMessage,
  PBMessageType,
  StateUpdateAction,
  STATE_UPDATED,
  WaveformHistory,
  ALARM_LIMITS,
  FRONTEND_DISPLAY_SETTINGS,
  SYSTEM_SETTINGS,
  commitAction,
} from './types';
import { DECIMAL_RADIX } from '../../modules/app/AppConstants';

const messageReducer = <T extends PBMessage>(
  messageType: MessageType,
  pbMessageType: PBMessageType,
) => (state: T = pbMessageType.fromJSON({}) as T, action: StateUpdateAction): T => {
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

const alarmLimitsReducer = (
  state: AlarmLimitsRequest = AlarmLimitsRequest.fromJSON({
    rrMax: 100,
    pipMax: 100,
    peepMax: 100,
    ipAbovePeepMax: 100,
    inspTimeMax: 100,
    fio2Max: 100,
    pawMax: 100,
    mveMax: 100,
    tvMax: 100,
    etco2Max: 100,
    flowMax: 100,
    apneaMax: 100,
  }) as AlarmLimitsRequest,
  action: commitAction,
): AlarmLimitsRequest => {
  return withRequestUpdate<AlarmLimitsRequest>(state, action, ALARM_LIMITS);
};

const frontendDisplaySettingReducer = (
  state: FrontendDisplaySetting = FrontendDisplaySetting.fromJSON({
    theme: ThemeVariant.dark,
    unit: Unit.imperial,
  }) as FrontendDisplaySetting,
  action: commitAction,
): FrontendDisplaySetting => {
  return withRequestUpdate<FrontendDisplaySetting>(state, action, FRONTEND_DISPLAY_SETTINGS);
};

const systemSettingRequestReducer = (
  state: SystemSettingRequest = SystemSettingRequest.fromJSON({
    brightness: 100,
    date: parseInt((new Date().getTime() / 1000).toFixed(0), DECIMAL_RADIX),
  }) as SystemSettingRequest,
  action: commitAction,
): SystemSettingRequest => {
  return withRequestUpdate<SystemSettingRequest>(state, action, SYSTEM_SETTINGS);
};

const withRequestUpdate = <T>(state: T, action: commitAction, prefix: string): T => {
  switch (action.type) {
    case STATE_UPDATED: // ignore message from backend
      return state;
    case `@controller/${prefix}_COMMITTED`:
      return { ...state, ...action.update } as T;
    default:
      return state;
  }
};

const parametersRequestReducer = (
  state: ParametersRequest = ParametersRequest.fromJSON({
    mode: VentilationMode.pc_ac,
    pip: 30,
    peep: 0,
    rr: 30,
    ie: 1.0,
    fio2: 60.0,
  }) as ParametersRequest,
  action: StateUpdateAction | ParameterCommitAction,
): ParametersRequest => {
  switch (action.type) {
    case STATE_UPDATED: // ignore message from backend
      return state;
    case PARAMETER_COMMITTED:
      return { ...state, ...action.update };
    default:
      return state;
  }
};

const waveformHistoryReducer = <T extends PBMessage>(
  messageType: MessageType,
  getTime: (values: T) => number,
  getValue: (values: T) => number,
  maxDuration = 10000,
  gapDuration = 500,
) => (
  state: WaveformHistory = {
    waveformOld: [],
    waveformNew: [],
    waveformNewStart: 0,
  },
  action: StateUpdateAction,
): WaveformHistory => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === messageType) {
        const sampleTime = getTime(action.state as T);
        const lastTime =
          state.waveformNew.length === 0
            ? state.waveformNewStart
            : state.waveformNew[state.waveformNew.length - 1].date;
        if (
          sampleTime > state.waveformNewStart + maxDuration ||
          sampleTime < lastTime ||
          new Date(sampleTime - gapDuration - state.waveformNewStart) > lastTime
        ) {
          // make waveformNew start over
          return {
            waveformOld: state.waveformNew,
            waveformNew: [
              {
                date: new Date(0),
                value: getValue(action.state as T),
              },
            ],
            waveformNewStart: sampleTime,
          };
        }

        return {
          waveformOld: state.waveformOld,
          waveformNew: state.waveformNew.concat([
            {
              date: new Date(sampleTime - state.waveformNewStart),
              value: getValue(action.state as T),
            },
          ]),
          waveformNewStart: state.waveformNewStart,
        };
      }
      return state;
    default:
      return state;
  }
};

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  alarms: messageReducer<Alarms>(MessageType.Alarms, Alarms),
  alarmLimitsRequest: alarmLimitsReducer,
  systemSettingRequest: systemSettingRequestReducer,
  frontendDisplaySetting: frontendDisplaySettingReducer,
  sensorMeasurements: messageReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    SensorMeasurements,
  ),
  cycleMeasurements: messageReducer<CycleMeasurements>(
    MessageType.CycleMeasurements,
    CycleMeasurements,
  ),
  parameters: messageReducer<Parameters>(MessageType.Parameters, Parameters),
  parametersRequest: parametersRequestReducer,
  ping: messageReducer<Ping>(MessageType.Ping, Ping),
  announcement: messageReducer<Announcement>(MessageType.Announcement, Announcement),

  // Message states from frontend_pb
  rotaryEncoder: messageReducer<RotaryEncoder>(MessageType.RotaryEncoder, RotaryEncoder),

  // Derived states
  waveformHistoryPaw: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.paw,
  ),
  waveformHistoryFlow: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.time,
    (sensorMeasurements: SensorMeasurements) => sensorMeasurements.flow,
  ),
});

export default controllerReducer;
