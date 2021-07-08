/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for MVeInfo parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ParameterInfo';
import { AlarmModal, Knob } from '../controllers';
import { StoreState } from '../../store/types';
import { getCycleMeasurementsVT } from '../../store/controller/selectors';
import { LMIN } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsVT,
});
const MVeDisplay = connect(displaySelector)(ValueDisplay);

const label = 'MVe';
const stateKey = 'mve';
const units = LMIN;

/**
 * MVeInfo
 *
 * @component A `Knob`-based component for handling MVe information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const MVeInfo = (): JSX.Element => (
  <Knob
    valueDisplay={<MVeDisplay label={label} units={units} isLive={true} />}
    valueModal={null}
    alarmModal={
      <AlarmModal label={label} units={units} stateKey={stateKey} requestCommitRange={() => null} />
    }
  />
);

export default MVeInfo;
