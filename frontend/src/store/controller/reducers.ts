import {combineReducers, Action} from 'redux'
import {
  Alarms,
  SensorMeasurements,
  CycleMeasurements,
  Parameters,
  ParametersRequest,
  Ping,
  Announcement,
  SpontaneousSupport,
  VentilationCycling
} from './proto/mcu_pb'
import {
  RotaryEncoder
} from './proto/frontend_pb'
import {
  MessageType,
  ParameterCommitAction,
  PARAMETER_COMMITTED,
  PBMessage,
  PBMessageType,
  StateUpdateAction,
  STATE_UPDATED,
  WaveformHistory,
  THEME_SWITCHED,
  ThemeVariant,
  SwitchThemeAction
} from './types'

const messageReducer = <T extends PBMessage>(
  messageType: MessageType, pbMessageType: PBMessageType
) => (
  state: T = pbMessageType.fromJSON({}) as T, action: StateUpdateAction
): T => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === messageType) {
        return action.state as T
      }
      return state
    default:
      return state
  }
}

const parametersRequestReducer = (
  state: ParametersRequest = ParametersRequest.fromJSON({
    mode: {
      support: SpontaneousSupport.ac,
      cycling: VentilationCycling.pc
    },
    pip: 30,
    peep: 0,
    rr: 30,
    ie: 1.0,
    fio2: 60.0
  }) as ParametersRequest,
  action: StateUpdateAction | ParameterCommitAction
): ParametersRequest => {
  switch (action.type) {
    case STATE_UPDATED:  // ignore message from backend
      return state
    case PARAMETER_COMMITTED:
      console.log(Object.assign({}, state, action.update))
      return Object.assign({}, state, action.update, {mode: {
        support: SpontaneousSupport.ac,
        cycling: VentilationCycling.pc
      }})
    default:
      return state
  }
}

const waveformHistoryReducer = <T extends PBMessage>(
  messageType: MessageType,
  getTime: (values: T) => number,
  getValue: (values: T) => number,
  maxDuration: number = 10000
) => (
  state: WaveformHistory = {
    waveformOld: [], waveformNew: [], waveformNewStart: 0
  },
  action: StateUpdateAction
): WaveformHistory => {
  switch (action.type) {
    case STATE_UPDATED:
      if (action.messageType === messageType) {
        const sampleTime = getTime(action.state as T)
        if (sampleTime > state.waveformNewStart + maxDuration) {
          // make waveformNew start over
          return {
            waveformOld: state.waveformNew,
            waveformNew: [{
              date: new Date(0),
              value: getValue(action.state as T)
            }],
            waveformNewStart: sampleTime
          }
        }

        return {
          waveformOld: state.waveformOld,
          waveformNew: state.waveformNew.concat([{
            date: new Date(sampleTime - state.waveformNewStart),
            value: getValue(action.state as T)
          }]),
          waveformNewStart: state.waveformNewStart
        }
      }
      return state
    default:
      return state
  }
}

const switchThemeReducer = (state = ThemeVariant.DARK, action: SwitchThemeAction): ThemeVariant => {
  switch (action.type) {
    case THEME_SWITCHED:
      return action.theme
    default:
      return state
  }
}

export const controllerReducer = combineReducers({
  // Message states from mcu_pb
  alarms: messageReducer<Alarms>(MessageType.Alarms, Alarms),
  sensorMeasurements: messageReducer<SensorMeasurements>(
    MessageType.SensorMeasurements, SensorMeasurements
  ),
  cycleMeasurements: messageReducer<CycleMeasurements>(
    MessageType.CycleMeasurements, CycleMeasurements
  ),
  parameters: messageReducer<Parameters>(
    MessageType.Parameters, Parameters
  ),
  parametersRequest: parametersRequestReducer,
  ping: messageReducer<Ping>(MessageType.Ping, Ping),
  announcement: messageReducer<Announcement>(
    MessageType.Announcement, Announcement
  ),

  // Message states from frontend_pb
  rotaryEncoder: messageReducer<RotaryEncoder>(
    MessageType.RotaryEncoder, RotaryEncoder
  ),

  // Derived states
  waveformHistoryPaw: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => (sensorMeasurements.time),
    (sensorMeasurements: SensorMeasurements) => (sensorMeasurements.paw)
  ),
  waveformHistoryFlow: waveformHistoryReducer<SensorMeasurements>(
    MessageType.SensorMeasurements,
    (sensorMeasurements: SensorMeasurements) => (sensorMeasurements.time),
    (sensorMeasurements: SensorMeasurements) => (sensorMeasurements.flow)
  ),
  theme: switchThemeReducer,
})
