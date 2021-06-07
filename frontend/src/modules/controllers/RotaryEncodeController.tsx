/**
 * @summary Controller handling the Rotary encoder functionality
 *
 */
import React, { useCallback, useEffect, useRef } from 'react';
import { useSelector } from 'react-redux';
import {
  // getRotaryEncoderButtonPressed,
  getRotaryEncoderStep,
  getRotaryEncoderStepDiff,
} from '../../store/controller/selectors';

/**
 * @typedef Props
 *
 * Props interface for RotaryEncodeController component
 *
 * @prop {number} value values passed from `ValueClicker`
 * @prop {function} onClick Triggers when rotation step is changed in rotary encoder
 * @prop {number} min Value below which rotary encoder rotation will not decrement
 * @prop {number} max Value above which rotary encoder rotation will not increment
 * @prop {boolean} isActive Determines if Rotary enoder should operate
 */
interface Props {
  value: number;
  onClick: (value: number) => void;
  min?: number;
  max?: number;
  isActive: boolean;
  disableChange?: boolean;
}

/**
 * RotaryEncodeController
 *
 * @component A re-usable "RotaryEncoder" component for adjusting `ValueClicker` value
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
  disableChange,
}: Props): JSX.Element => {
  const step = useSelector(getRotaryEncoderStep);
  const stepDiff = useSelector(getRotaryEncoderStepDiff);
  // const buttonPressed = useSelector(getRotaryEncoderButtonPressed);
  const [prevStep, setPrevStep] = React.useState(step);
  const isInitialMount = useRef(true);

  const updateRotaryData = useCallback(() => {
    if (!isInitialMount.current) {
      isInitialMount.current = false;
      return;
    }
    if (Number.isNaN(stepDiff)) {
      return;
    }
    if (step === prevStep) {
      return;
    }
    if (disableChange) {
      return;
    }
    setPrevStep(step);
    const newValue = value + stepDiff;
    if (newValue < min) {
      onClick(min);
    } else if (newValue > max) {
      onClick(max);
    } else {
      onClick(newValue);
    }
    // if (buttonPressed) {
    //   handleConfirm();
    // }
  }, [step, stepDiff, disableChange, min, max, value, prevStep, onClick]);

  useEffect(() => {
    if (isActive) {
      updateRotaryData();
    }
  }, [updateRotaryData, isActive]);

  return <React.Fragment />;
};

export default RotaryEncodeController;
