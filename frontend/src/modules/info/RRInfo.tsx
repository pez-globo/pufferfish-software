import React from 'react'
import { bindActionCreators } from 'redux'
import { connect } from 'react-redux'
import { createStructuredSelector } from 'reselect'
import {
  ValueDisplay, ValueProps
} from '../displays/ValueDisplay'
import {
  ValueModal, SettingAdjustProps
} from '../controllers/ValueModal'
import { AlarmModal, Knob } from '../controllers'
import store from '../../store'
import { StoreState } from '../../store/types'
import { PARAMETER_COMMITTED } from '../../store/controller/types'
import {
  getCycleMeasurementsRR,
  getParametersRR
} from '../../store/controller/selectors'
import { BMIN } from './units'
import { updateCommitedParameter } from '../../store/controller/actions'

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsRR
})
const RRDisplay = connect(displaySelector)(ValueDisplay)

const doSetRR = (setting: number) => updateCommitedParameter({ rr: setting })
const boundDoSetRR = bindActionCreators(doSetRR, store.dispatch)

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersRR
})
const RRValueModal = connect(settingSelector)(ValueModal)

const label = 'RR'
const stateKey = "rr"
const units = BMIN

/**
 * RRInfo 
 * 
 * A `Knob`-based component for handling RR information.
 * 
 * TODO: Hook this component into the redux store with correct selectors.
 */
const RRInfo = () => (
  <Knob
    valueDisplay={<RRDisplay label={label} units={units} isLive={true} />}
    valueModal={<RRValueModal label={label} units={units} requestCommitSetting={boundDoSetRR} />}
    alarmModal={<AlarmModal label={label} units={units} stateKey={stateKey} requestCommitRange={() => null} />}
  />
)

export default RRInfo