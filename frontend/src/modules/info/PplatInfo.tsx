/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for PplatInfo parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { ValueModal, Knob } from '../controllers';
import { StoreState } from '../../store/types';
import { getCycleMeasurementsVT } from '../../store/controller/selectors';
import { CMH20 } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsVT,
});
const PplatDisplay = connect(displaySelector)(ValueDisplay);

const label = 'Pplat';
const units = CMH20;

/**
 * PplatInfo
 *
 * @component A `Knob`-based component for handling Pplat information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const PplatInfo = (): JSX.Element => (
  <Knob
    valueDisplay={<PplatDisplay label={label} units={units} isLive={true} />}
    valueModal={
      <ValueModal
        label={label}
        units={units}
        committedSetting={12}
        requestCommitSetting={() => null}
      />
    }
    alarmModal={null}
  />
);

export default PplatInfo;
