/**
 * @summary Component wrapper to display Sensor Measurement's Value in Screensaver
 *
 */
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React from 'react';
import { ValueSelectorDisplay } from '../../shared/value/ValueSelectorDisplay';
import { SelectorProps } from './ControlJumbotron';

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
  valueLabel: {
    fontSize: '8rem',
    margin: '0 auto',
    fontWeight: 'initial',
    // border: '1px solid red',
  },
  unitsLabel: {
    fontWeight: 100,
    paddingLeft: theme.spacing(0),
    // paddingTop: theme.spacing(4),
    color: theme.palette.grey[400],
    textAlign: 'center',
    // border: '1px solid red'
  },
  labelItems: {
    alignItems: 'flex-start',
    // border: '1px solid red'
  },
}));

/**
 * ValueJumbotron
 *
 * @component A component for displays a single value with a large font size.
 *
 * Uses the [[SelectorProps]] interface
 *
 * @returns {JSX.Element}
 */
export const ValueJumbotron = ({
  selector,
  label,
  units = '',
  decimal = 0,
}: SelectorProps): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container className={classes.root}>
      <Grid item style={{ width: '100%' }} className={classes.labelItems}>
        <Typography variant="h3" style={{ fontWeight: 'bold', fontSize: '4rem' }}>
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
        <Typography align="left" variant="h1" className={classes.valueLabel}>
          <ValueSelectorDisplay decimal={decimal} selector={selector} />
        </Typography>
      </Grid>
      <Grid item style={{ width: '100%' }}>
        <Typography align="left" variant="h4" className={classes.unitsLabel}>
          {units}
        </Typography>
      </Grid>
    </Grid>
  );
};

export default ValueJumbotron;
