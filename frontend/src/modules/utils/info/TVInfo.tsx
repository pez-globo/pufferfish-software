import React from 'react'
import { connect } from 'react-redux'
import { createStructuredSelector } from 'reselect'
import {
  ValueDisplay, ValueProps
} from '../displays/ValueDisplay'

import { Knob,Knob2 } from '../controls/Knob'
import { AlarmModal } from '../controls/AlarmModal'
import { ValueModal } from '../controls/ValueModal'
import { StoreState } from '../../../store/types'
import {
  getCycleMeasurementsVT
} from '../../../store/controller/selectors'
import { LMIN } from './units'

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsVT
})
const TVDisplay = connect(displaySelector)(ValueDisplay)

const label = 'VT'
const units = LMIN

/**
 * TVInfo 
 * 
 * A `Knob`-based component for handling TV information.
 * 
 * TODO: Hook this component into the redux store with correct selectors.
 */
const TVInfo = () => (
  <Knob2
    valueDisplay={<TVDisplay label={label} units={units} isLive={true} />}
    //ValueModal={<ValueModal label={label} units={units} 
    //committedSetting={1.7} requestCommitSetting={() => null}/>} 
    alarmModal={<AlarmModal label={label} units={units}
    requestCommitRange={() => null} />}
  />
)

export default TVInfo