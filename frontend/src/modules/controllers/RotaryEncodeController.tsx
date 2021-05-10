/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import React, { useCallback, useEffect, useRef } from 'react';
import { useSelector, shallowEqual } from 'react-redux';
import { getRotaryEncoder } from '../../store/controller/selectors';

/**
 * @typedef Props
 *
 * Props interface for RotaryEncodeController component
 *
 * @prop {number} value desc for value
 * @prop {function} onClick desc for onClick
 * @prop {number} min desc for min
 * @prop {number} max desc for max
 * @prop {boolean} isActive desc for isActive
 */
interface Props {
  value: number;
  onClick: (value: number) => void;
  min?: number;
  max?: number;
  isActive: boolean;
}

/**
 * RotaryEncodeController
 *
 * @component A re-usable "RotaryEncoder" component for adjusting ValueClicker value
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const RotaryEncodeController = ({
  value,
  onClick,
  min = 0,
  max = 100,
  isActive,
}: Props): JSX.Element => {
  const rotaryEncoder = useSelector(getRotaryEncoder, shallowEqual);
  const isInitialMount = useRef(true);

  const updateRotaryData = useCallback(
    () => {
      if (isInitialMount.current) {
        isInitialMount.current = false;
      } else if (!Number.isNaN(rotaryEncoder?.stepDiff)) {
        const stepDiff = rotaryEncoder?.stepDiff || 0;
        const valueClone = value >= min ? value : min;
        const newValue = valueClone + stepDiff;
        if (newValue < min) {
          onClick(min);
        } else if (newValue > max) {
          onClick(max);
        } else {
          onClick(newValue);
        }
        // if (rotaryEncoder.buttonPressed) {
        //   handleConfirm();
        // }
      }
    },
    // eslint-disable-next-line react-hooks/exhaustive-deps
    [rotaryEncoder?.step, rotaryEncoder?.buttonPressed, min, max, value],
  );

  useEffect(() => {
    if (isActive) {
      updateRotaryData();
    }
  }, [updateRotaryData, isActive]);

  return <React.Fragment />;
};

export default RotaryEncodeController;
