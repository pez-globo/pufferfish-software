import React from 'react'
import { connect } from 'react-redux'
import { createStructuredSelector } from 'reselect'
import {
    ValueDisplay, ValueProps
} from '../displays/ValueDisplay'
import { Knob } from '../controls/Knob'
import { AlarmModal } from '../controls/AlarmModal'
import { StoreState } from '../../../store/types'
import {
    getCycleMeasurementsVT
} from '../../../store/controller/selectors'
import { MMHG } from './units'

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
    value: getCycleMeasurementsVT
})
const EtCO2Display = connect(displaySelector)(ValueDisplay)

const label = 'EtCO2'
const units = MMHG

/**
 * EtCO2
 * 
 * A `Knob`-based component for handling EtCO2 information.
 * 
 * TODO: Hook this component into the redux store with correct selectors.
 */
const EtCO2Info = () => (
    <Knob
        valueDisplay={<EtCO2Display label={label} units={units} isLive={true} />}
        ValueModal={null}
        alarmModal={<AlarmModal label={label} units={units} requestCommitRange={() => null} />}
    />
)

export default EtCO2Info