/**
 * @summary Re-usable Component to display value, label and unit
 * currently used in settings > Display tab
 *
 */
import React from 'react';
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: '100%',
    // border: '1px solid red'
  },
  displayContainer: {
    padding: 0,
    // border: '1px solid orange',
  },
  liveContainer: {
    justifyContent: 'center',
    alignItems: 'center',
    height: '100%',
    // border: '1px solid blue'
  },
  liveBox: {
    textAlign: 'center',
    width: '100%',
    fontSize: 14,
    borderRadius: 5,
    border: `2px solid ${theme.palette.primary.main}`,
  },
  valueContainer: {
    justifyContent: 'flex-start',
    alignItems: 'center',
    flexGrow: 1,
    // border: '1px solid yellow',
  },
  valueLabel: {
    fontSize: '4rem',
    lineHeight: '1',
    // border: '1px solid red',
  },
  unitsLabel: {
    paddingLeft: theme.spacing(1),
    paddingTop: theme.spacing(4),
    opacity: 0.8,
    // border: '1px solid red'
  },
}));

/**
 * @typedef ValueProps
 *
 * Interface for the value props
 *
 * @prop {number} value value to be displayed
 */
export interface ValueProps {
  value: number;
}

/**
 * @typedef Props
 *
 * Props Interface for the ValueDisplay component
 *
 * @prop {number} value value to be displayed
 * @prop {string} label Label of the value
 * @prop {string} units unit measurement of the value
 */
interface Props {
  value: number;
  label: string;
  units?: string;
}

/**
 * ValueDisplay
 *
 * @component A re-usable component for displaying numeric values.
 * ValueDisplay is used along with ValueClicker to increment/decrement the displayed value.
 * This component gives a standard markup for how value should be displayed.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const ValueDisplay = ({ value, label, units = '' }: Props): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container direction="column" className={classes.root}>
      <Grid
        container
        item
        direction="row"
        justify="space-between"
        alignItems="center"
        wrap="nowrap"
      >
        <Grid item xs style={{ width: '100%' }}>
          <Typography variant="h5">{label}</Typography>
        </Grid>
      </Grid>
      <Grid
        container
        item
        xs
        justify="flex-start"
        alignItems="center"
        className={classes.displayContainer}
        wrap="nowrap"
      >
        <Typography align="center" variant="h2" className={classes.valueLabel}>
          {value !== undefined && !Number.isNaN(value)
            ? value.toFixed(0).replace('/^-0$/', '0')
            : '--'}
        </Typography>
        {units !== '' && (
          <Typography align="center" variant="h5" className={classes.unitsLabel}>
            {units}
          </Typography>
        )}
      </Grid>
    </Grid>
  );
};

export default ValueDisplay;
