/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for SpO2 parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { getSensorMeasurementsSpO2 } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { PERCENT } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getSensorMeasurementsSpO2,
});
const SPO2Display = connect(displaySelector)(ValueDisplay);

const label = 'SpO2';
const stateKey = 'spo2';
const units = PERCENT;

/**
 * SpO2Info
 *
 * @component A `Knob`-based component for handling SpO2 information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const SpO2Info = (): JSX.Element => (
  <Knob
    valueDisplay={<SPO2Display label={label} units={units} isLive={true} />}
    valueModal={null}
    alarmModal={
      <AlarmModal label={label} units={units} stateKey={stateKey} requestCommitRange={() => null} />
    }
  />
);

export default SpO2Info;
