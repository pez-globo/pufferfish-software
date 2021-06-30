/**
 * @summary Re-usable UI wrapper for displaying Large size `ValueInfo`
 *
 */
import React from 'react';
import { SelectorType } from '../../displays/ValueSelectorDisplay';
import ValueInfo from './ValueInfo';

/**
 * @typedef ValueInfoProps
 *
 * some description
 *
 * @prop {Props} mainContainer Main Container wrapper
 *
 */
export interface ValueInfoProps {
  mainContainer: Props;
}

/**
 * @typedef Props
 *
 * Props interface for the control value information.
 *
 * @prop {SelectorType} selector Redux Selector
 * @prop {string} label Value label
 * @prop {string} stateKey Unique identifier
 * @prop {string} units Unit measurement of value to display
 * @prop {boolean} isLive Config to show isLive in UI
 * @prop {boolean} showLimits Config to show Alarm limts in container (on top left - small size)
 * @prop {number} decimal Number of Decimals on the value
 *
 */
export interface Props {
  selector: SelectorType;
  label: string;
  stateKey: string;
  units?: string;
  isLive?: boolean;
  showLimits?: boolean;
  decimal?: number;
  isLarge?: boolean;
}

/**
 * ValueInfo
 *
 * @component Component for showing information.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 *
 */
const LargeValueInfo = ({
  selector,
  label,
  stateKey,
  units = '',
  showLimits,
  decimal,
  isLarge = false,
}: Props): JSX.Element => {
  return (
    <ValueInfo
      mainContainer={{
        selector,
        label,
        stateKey,
        units,
        showLimits,
        decimal,
        isLarge,
      }}
    />
  );
};

export default LargeValueInfo;
