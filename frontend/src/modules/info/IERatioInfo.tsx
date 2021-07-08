/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for IERatio parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { ValueDisplay } from '../displays/ParameterInfo';
import { Knob } from '../controllers';

/**
 * IERatioInfo
 *
 * @component A `Knob`-based component for handling I:E Ratio information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const IERatioInfo = (): JSX.Element => (
  <Knob
    valueDisplay={<ValueDisplay value={0} label="I:E Ratio" units="" isLive={true} />}
    valueModal={null}
    alarmModal={null}
  />
);

export default IERatioInfo;
