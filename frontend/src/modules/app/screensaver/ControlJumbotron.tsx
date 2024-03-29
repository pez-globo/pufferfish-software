/**
 * @summary Component wrapper to display Sensor Measurement's Controls in Screensaver
 *
 */
import React from 'react';
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import { SelectorType, ValueSelectorDisplay } from '../../shared/value/ValueSelectorDisplay';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100%',
    width: '100%',
    textAlign: 'center',
    // border: '1px solid red'
  },
  displayContainer: {
    padding: 0,
    alignItems: 'flex-start',
    // border: '1px solid orange',
  },
  valueContainer: {
    flexGrow: 1,
    // border: '1px solid yellow',
  },
  ControlLabel: {
    fontSize: '4.5rem',
    // border: '1px solid red',
    fontWeight: 'lighter',
    lineHeight: '.9',
    margin: '0 auto',
  },
  unitsLabel: {
    fontWeight: 100,
    paddingLeft: theme.spacing(0),
    // paddingTop: theme.spacing(4),
    color: theme.palette.grey[400],
    // border: '1px solid red'
  },
  labelItems: {
    alignItems: 'flex-start',
    // border: '1px solid red'
  },
}));

/**
 * @typedef SelectorProps
 *
 * Interface for the Control Jumbotron
 *
 * @prop {SelectorType} selector Redux Selector
 * @prop {number} decimal Number of Decimals on the value
 * @prop {string} label Value label
 * @prop {string} units Unit measurement of value to display
 */
export interface SelectorProps {
  selector: SelectorType;
  decimal?: number;
  label: string;
  units?: string;
}

/**
 * ControlJumbotron
 *
 * @component A component for displaying a single value with large font size.
 *
 * Uses the [[SelectorProps]] interface
 *
 * @returns {JSX.Element}
 */
export const ControlJumbotron = ({ selector, label, units = '' }: SelectorProps): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container className={classes.root}>
      <Grid item style={{ width: '100%' }} className={classes.labelItems}>
        <Typography variant="h3" style={{ fontWeight: 'bold', fontSize: '1.8rem' }}>
          {label}
        </Typography>
      </Grid>
      <Grid
        container
        item
        xs
        alignItems="center"
        className={classes.displayContainer}
        wrap="nowrap"
      >
        <Typography variant="h1" className={classes.ControlLabel}>
          <ValueSelectorDisplay selector={selector} />
        </Typography>
      </Grid>
      <Grid item style={{ width: '100%' }}>
        <Typography variant="h4" className={classes.unitsLabel} style={{ fontSize: '1.3rem' }}>
          {units}
        </Typography>
      </Grid>
    </Grid>
  );
};

export default ControlJumbotron;
