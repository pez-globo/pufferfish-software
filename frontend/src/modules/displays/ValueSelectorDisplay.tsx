import React from 'react';
import { OutputSelector } from 'reselect';
import { useSelector } from 'react-redux';

// The Selector type is generic and its complicated to define its type.
// eslint-disable @typescript-eslint/no-explicit-any
// eslint-disable-next-line
export type SelectorType = OutputSelector<any, any, any>;

interface ValueSelectorDisplayProps {
  selector: SelectorType;
  decimal?: number;
}

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
