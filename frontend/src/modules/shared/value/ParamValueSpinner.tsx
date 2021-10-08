import { useTheme } from '@material-ui/core';
import React, { RefObject, useEffect } from 'react';
import { useSelector } from 'react-redux';
import ValueSpinner from './ValueSpinner';
import { SelectorType } from './ValueSelectorDisplay';
import useRotaryReference from '../rotary/useRotaryReference';

interface ValueProps {
  selector: SelectorType;
  value?: number;
  onClick: (value: number) => void;
  reference: string;
  label: string;
  units?: string;
  elRefsArray: Record<string, RefObject<HTMLDivElement>>;
  min?: number;
  max?: number;
}

const ParamValueSpinner = ({
  label,
  onClick,
  reference,
  selector,
  units,
  value,
  elRefsArray,
  min,
  max,
}: ValueProps): JSX.Element => {
  const theme = useTheme();
  const selectorValue = useSelector(selector);
  const { initRefListener } = useRotaryReference(theme);

  /**
   * State to manage Wrapper HTML reference of parameter `ValueSpinner`
   * This wrapper's HTML border is added or removed based on user's interaction with Controls
   * It is used for UI identification of which control value is changing via rotary encoder
   */
  const [elRefs] = React.useState<Record<string, RefObject<HTMLDivElement>>>(elRefsArray);

  /**
   * Calls on initalization of the component
   * This is an event listener which listens to user input on `ValueSpinner` buttons click
   * Based on this event Border around Alarm's HTML wrapper is added/removed
   */
  useEffect(() => {
    initRefListener(elRefs);
  }, [initRefListener, elRefs]);

  return (
    <ValueSpinner
      reference={elRefs[reference]}
      referenceKey={reference}
      label={label}
      units={units}
      value={value ?? selectorValue}
      onClick={onClick}
      min={min}
      max={max}
    />
  );
};

export default ParamValueSpinner;
