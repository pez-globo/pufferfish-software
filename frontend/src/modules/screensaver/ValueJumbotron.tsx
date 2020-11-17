import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React from 'react';

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

export interface Props {
  value: number;
  label: string;
  units?: string;
}

/**
 * ValueJumbotron
 *
 * A component for displaying values in a large way.
 */
export const ValueJumbotron = ({ value, label, units = '' }: Props): JSX.Element => {
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
          {value !== undefined && !Number.isNaN(value)
            ? value.toFixed(0).replace(/^-0$/, '0')
            : '--'}
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
