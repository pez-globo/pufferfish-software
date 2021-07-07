/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for Paw parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { StoreState } from '../../store/types';
import { getCycleMeasurementsVT } from '../../store/controller/selectors';
import { AlarmModal, Knob } from '../controllers';
import { CMH20 } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsVT,
});
const PawDisplay = connect(displaySelector)(ValueDisplay);

const label = 'Paw';
const stateKey = 'paw';
const units = CMH20;

/**
 * PawInfo
 *
 * @component A `Knob`-based component for handling Paw information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const PawInfo = (): JSX.Element => (
  <Knob
    valueDisplay={<PawDisplay label={label} units={units} isLive={true} />}
    valueModal={null}
    alarmModal={
      <AlarmModal label={label} units={units} stateKey={stateKey} getCommittedRange={() => null} />
    }
  />
);

export default PawInfo;
