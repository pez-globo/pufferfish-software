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
  getCycleMeasurementsPEEP,
  getParametersPEEP
} from '../../store/controller/selectors'
import { CMH20 } from './units'


const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsPEEP
})
const PEEPDisplay = connect(displaySelector)(ValueDisplay)

const doSetPEEP = (setting: number) => ({
  type: PARAMETER_COMMITTED, update: { peep: setting }
})
const boundDoSetPEEP = bindActionCreators(doSetPEEP, store.dispatch)

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersPEEP
})
const PEEPValueModal = connect(settingSelector)(ValueModal)

const label = 'PEEP'
const units = CMH20

/**
 * PEEPInfo 
 * 
 * A `Knob`-based component for handling PEEP information.
 * 
 * TODO: Hook this component into the redux store with correct selectors.
 */
const PEEPInfo = () => (
  <Knob
    valueDisplay={<PEEPDisplay label={label} units={units} isLive={true} />}
    valueModal={<PEEPValueModal label={label} units={units} requestCommitSetting={boundDoSetPEEP} />}
    alarmModal={<AlarmModal label={label} units={units} requestCommitRange={() => null} />}
  />
)

export default PEEPInfo