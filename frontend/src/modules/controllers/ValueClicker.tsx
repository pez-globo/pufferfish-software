/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
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
 * @prop {number} value desc for value
 * @prop {function} onClick desc for onClick
 * @prop {number} min desc for min
 * @prop {number} max desc for max
 * @prop {Direction} direction desc for direction
 * @prop {number} step desc for step 
 * @prop {string} referenceKey desc for referenceKey
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
   * add description.
   *
   * @param {number} step - desc for step
   *
   * @returns output of onClick function
   */
  const update = (step: number) => {
    const change = value + step;
    setDisableIncrement(change >= max);
    setDisableDecrement(change <= min);
    return onClick(change);
  };

  useEffect(() => {
    setDisableIncrement(value >= max);
    setDisableDecrement(value <= min);
  }, [min, max, value]);

  /**
   * add description.
   *
   * @param {number} step - desc for step
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
   * add description.
   *
   * @param {number} step - desc for step
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
   * add description.
   *
   * @param {number} value - desc for value
   * @param {Type} type - desc for type
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
