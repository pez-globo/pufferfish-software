/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for ROXIndex parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ParameterInfo';
import { Knob } from '../controllers';

import { StoreState } from '../../store/types';
import { getROXIndex } from '../../store/controller/selectors';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getROXIndex,
});
const ROXIndexDisplay = connect(displaySelector)(ValueDisplay);

const label = 'ROX Index';

/**
 * ROXIndexInfo
 *
 * @component A `Knob`-based component for handling ROX Index information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const ROXIndexInfo = (): JSX.Element => (
  <Knob
    valueDisplay={<ROXIndexDisplay label={label} isLive={true} />}
    valueModal={null}
    alarmModal={null}
  />
);

export default ROXIndexInfo;
