/**
 * @summary Re-usable Wrapper container to control & display value
 *
 */
import React, { RefObject } from 'react';
import { Grid, makeStyles, Theme } from '@material-ui/core';
import { ValueClicker } from '.';
import ValueDisplay from './ValueDisplay';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    justifyContent: 'center',
    height: '100%',
    padding: theme.spacing(2),
    alignItems: 'center',
  },
}));

/**
 * @typedef Props
 *
 * Props interface for the value controller
 *
 * @prop {number} value Paramater value displayed in UI
 * @prop {string} label Label to associate to which value belongs to
 * @prop {function} onClick Callback with value on button click or Rotary encoder
 * @prop {number} min Minimum (Default to 0)
 * @prop {number} max Maximum (Default to 100)
 * @prop {string} units Paramater unit measurement to display
 * @prop {RefObject<HTMLDivElement>} reference HTML element reference to highlight element for rotary encoder
 * @prop {string} referenceKey Unique identifier of alarm range for Rotary encoder
 *
 */
interface Props {
  value: number;
  label: string;
  onClick: (value: number) => void;
  // An inclusive range can defined by [min, max].
  min?: number;
  max?: number;
  units?: string;
  reference: RefObject<HTMLDivElement>;
  referenceKey: string;
}

/**
 * ValueSpinner
 *
 * @component A re-usable component for adjusting some value with a clicker.
 * Use ValueSpinner when the visual aspect of the 'Value' is similar to what we have in ValueDisplay
 * if a different look is required then it's recommended to only use ValueClicker.
 * more info: https://github.com/pez-globo/pufferfish-software/pull/413#issuecomment-882515233
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
const ValueSpinner = ({
  label,
  value,
  min = 0,
  max = 100,
  onClick,
  units,
  reference,
  referenceKey,
}: Props): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid ref={reference} container className={classes.root}>
      <Grid item xs>
        <ValueDisplay value={value} label={label} units={units} />
      </Grid>
      <Grid item>
        <ValueClicker
          referenceKey={referenceKey}
          value={value}
          onClick={onClick}
          min={min}
          max={max}
        />
      </Grid>
    </Grid>
  );
};

export default ValueSpinner;
