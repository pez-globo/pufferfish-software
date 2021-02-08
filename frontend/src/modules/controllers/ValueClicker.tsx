import React, { useEffect, useState } from 'react';
import { Grid, Button, makeStyles, Theme } from '@material-ui/core';
import KeyboardArrowUp from '@material-ui/icons/KeyboardArrowUp';
import KeyboardArrowDown from '@material-ui/icons/KeyboardArrowDown';

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

interface Props {
  value: number;
  onClick: (value: number) => void;
  // An inclusive range defined by [min, max]
  min?: number;
  max?: number;
  direction?: Direction;
  step?: number;
}

/**
 * ValueClicker
 *
 * A re-usable component for simple increment and decrement value adjustments.
 */
export const ValueClicker = ({
  value,
  min = 0,
  max = 100,
  onClick,
  direction = 'column',
  step = 1,
}: Props): JSX.Element => {
  const classes = useStyles();
  const [disabledInterval] = useState(100);
  const [disableIncrement, setDisableIncrement] = useState(false);
  const [disableDecrement, setDisableDecrement] = useState(false);

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

  const clickHandlerIncrement = (step: number) => () => {
    setDisableIncrement(true);
    setTimeout(() => {
      setDisableIncrement(false);
      update(step);
    }, disabledInterval);
  };

  const clickHandlerDecrement = (step: number) => () => {
    setDisableDecrement(true);
    setTimeout(() => {
      setDisableDecrement(false);
      update(step);
    }, disabledInterval);
  };

  return (
    <Grid container direction={direction} className={classes.root} wrap="nowrap">
      <Grid item className={direction === 'row' ? classes.marginRight : classes.marginBottom}>
        <Button
          disabled={disableIncrement}
          onClick={clickHandlerIncrement(step)}
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
          onClick={clickHandlerDecrement(-step)}
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
