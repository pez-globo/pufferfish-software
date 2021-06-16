/**
 * @summary Re-usable component for Increment/Decrement Click button pair
 *
 */
import React, { useEffect, useState } from 'react';
import { Grid, Button, makeStyles, Theme } from '@material-ui/core';
import KeyboardArrowUp from '@material-ui/icons/KeyboardArrowUp';
import KeyboardArrowDown from '@material-ui/icons/KeyboardArrowDown';
import { Subscription } from 'rxjs';
import { setActiveRotaryReference, getActiveRotaryReference } from '../app/Service';
import RotaryEncodeController from './RotaryEncodeController';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    justifyContent: 'center',
    alignItems: 'center',
    width: '100%',
    // border: '1px dashed red',
  },
  iconButton: {
    border: `1px solid ${theme.palette.common.black}`,
    borderRadius: 8,
    height: '100%',
    minWidth: 78,
    marginLeft: theme.spacing(1),
  },
  marginBottom: { marginBottom: theme.spacing(1) },
  marginRight: { marginRight: theme.spacing(1) },
}));

type Direction = 'column' | 'column-reverse' | 'row' | 'row-reverse' | undefined;

/**
 * @typedef Props
 *
 * Props interface for the value clicker
 *
 * @prop {number} value Paramater value displayed in UI
 * @prop {function} onClick Callback with value on button click or Rotary encoder
 * @prop {number} min Allowed minimum range value
 * @prop {number} max Allowed maximum range value
 * @prop {Direction} direction Decides styling of `ValueClicker` placed Vertically or Horizontally
 * @prop {number} step Alarm step difference between Range (Defaults to 1)
 * @prop {string} referenceKey Unique identifier of alarm lower/upper range
 *
 */
interface Props {
  value: number;
  onClick: (value: number) => void;
  // An inclusive range defined by [min, max]
  min?: number;
  max?: number;
  direction?: Direction;
  step?: number;
  referenceKey: string;
}
enum Type {
  INCREMENT,
  DECREMENT,
}

/**
 * ValueClicker
 *
 * @component A re-usable component for simple increment and decrement value adjustments.
 *
 * Uses the [[Props]] interface
 * @returns JSX.Element
 */
export const ValueClicker = ({
  value,
  min = 0,
  max = 100,
  onClick,
  direction = 'column',
  step = 1,
  referenceKey,
}: Props): JSX.Element => {
  const classes = useStyles();
  const [disabledInterval] = useState(100);
  const [disableIncrement, setDisableIncrement] = useState(false);
  const [disableDecrement, setDisableDecrement] = useState(false);
  const [isRotaryActive, setIsRotaryActive] = React.useState(false);
  const [activeRef, setActiveRef] = React.useState<string | null>();

  /**
   * Triggers callback with updated value
   *
   * @param {number} step - Alarm step difference
   *
   * @returns callback function
   */
  const update = (step: number) => {
    const change = value + step;
    setDisableIncrement(change >= max);
    setDisableDecrement(change <= min);
    if (change >= max) {
      return onClick(max);
    }
    if (change <= min) {
      return onClick(min);
    }
    return onClick(change);
  };

  /**
   * Validating & disabling button when reached min/max value
   */
  useEffect(() => {
    setDisableIncrement(value >= max);
    setDisableDecrement(value <= min);
  }, [min, max, value]);

  /**
   * Click handler for Increment button
   *
   * @param {number} step - Alarm step difference
   *
   */
  const clickHandlerIncrement = (step: number) => {
    setDisableIncrement(true);
    setTimeout(() => {
      setDisableIncrement(false);
      update(step);
    }, disabledInterval);
  };

  /**
   * Click handler for Decrement button
   *
   * @param {number} step - Alarm step difference
   *
   */
  const clickHandlerDecrement = (step: number) => {
    setDisableDecrement(true);
    setTimeout(() => {
      setDisableDecrement(false);
      update(step);
    }, disabledInterval);
  };

  /**
   * Button Click Wrapper
   *
   * @param {number} value - current value
   * @param {Type} type - Clicker type increment/decrement
   *
   */
  const internalClick = (value: number, type: Type) => {
    if (type === Type.INCREMENT) {
      clickHandlerIncrement(value);
    } else {
      clickHandlerDecrement(value);
    }
    if (!activeRef || (activeRef && activeRef !== referenceKey)) {
      setActiveRotaryReference(referenceKey);
    }
  };

  /**
   * Triggered whenever reference key is updated
   * Updates Rotary Encoder reference to active
   */
  useEffect(() => {
    const refSubscription: Subscription = getActiveRotaryReference().subscribe(
      (refString: string | null) => {
        setActiveRef(refString);
        if (refString && refString === referenceKey) {
          setIsRotaryActive(true);
        } else {
          setIsRotaryActive(false);
        }
      },
    );
    return () => {
      if (refSubscription) {
        refSubscription.unsubscribe();
      }
    };
  }, [referenceKey]);

  return (
    <Grid
      container
      direction={direction}
      className={classes.root}
      wrap="nowrap"
      onClick={(e) => e.stopPropagation()}
    >
      <RotaryEncodeController
        isActive={isRotaryActive}
        value={value}
        onClick={(num: number) => {
          onClick(num);
        }}
        min={min}
        max={max}
      />
      <Grid item className={direction === 'row' ? classes.marginRight : classes.marginBottom}>
        <Button
          disabled={disableIncrement}
          onClick={() => internalClick(step, Type.INCREMENT)}
          variant="contained"
          color="primary"
          className={classes.iconButton}
        >
          <KeyboardArrowUp fontSize="large" />
        </Button>
      </Grid>
      <Grid item>
        <Button
          disabled={disableDecrement}
          onClick={() => internalClick(-step, Type.DECREMENT)}
          variant="contained"
          color="primary"
          className={classes.iconButton}
        >
          <KeyboardArrowDown fontSize="large" />
        </Button>
      </Grid>
    </Grid>
  );
};

export default ValueClicker;
