/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import React from 'react';
import { OutputSelector } from 'reselect';
import { useSelector } from 'react-redux';

// The Selector type is generic and its complicated to define its type.
// eslint-disable @typescript-eslint/no-explicit-any
// eslint-disable-next-line
export type SelectorType = OutputSelector<any, any, any>;

/**
 * @typedef ValueSelectorDisplayProps
 *
 * Props Interface for the ValueSelectorDisplay component
 *
 * @prop {SelectorType} selector desc for selector
 * @prop {number} decimal desc for decimal
 */
interface ValueSelectorDisplayProps {
  selector: SelectorType;
  decimal?: number;
}

/**
 * ValueSelectorDisplay
 *
 * @component A container for displaying the value selectors.
 *
 * Uses the [[ValueSelectorDisplay]] interface
 *
 * @returns {JSX.Element}
 */
export const ValueSelectorDisplay = ({
  selector,
  decimal = 0,
}: ValueSelectorDisplayProps): JSX.Element => {
  const value: number | undefined = useSelector(selector);

  return (
    <React.Fragment>
      {value !== undefined && !Number.isNaN(value)
        ? value.toFixed(decimal).replace(/^-0$/, '0')
        : '--'}
    </React.Fragment>
  );
};
