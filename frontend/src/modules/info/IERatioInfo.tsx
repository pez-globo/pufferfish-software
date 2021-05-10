/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import React from 'react';
import { ValueDisplay } from '../displays/ValueDisplay';
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
