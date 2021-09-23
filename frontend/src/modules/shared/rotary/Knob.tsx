/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
import React from 'react';
import { Grid, makeStyles, Theme } from '@material-ui/core';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    paddingTop: theme.spacing(1),
    paddingBottom: theme.spacing(2),
    paddingLeft: theme.spacing(2),
    paddingRight: theme.spacing(2),
    // border: '1px solid teal',
    flexDirection: 'initial',
    height: '100%',
  },
  controlPanel: {
    justifyContent: 'space-between',
    alignItems: 'center',
    // border: '1px solid orange'
  },
  marginRight: {
    marginRight: theme.spacing(1),
  },
}));

/**
 * @typedef Props
 *
 * Props interface for Knob component
 *
 * @prop {React.ReactNode} valueDisplay desc for valueDisplay
 * @prop {React.ReactNode} valueModal desc for valueModal
 * @prop {React.ReactNode} alarmModal desc for alarmModal
 */
interface Props {
  valueDisplay: React.ReactNode;
  valueModal: React.ReactNode;
  alarmModal?: React.ReactNode;
}

/**
 * Knob
 * @deprecated
 * @component A re-usable "knob" component for adjusting alarms and updating values all-in-one.
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const Knob = ({ valueDisplay, valueModal, alarmModal }: Props): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container direction="column" className={classes.root}>
      <Grid item xs style={{ width: '100%', maxHeight: '77%' }}>
        {valueDisplay}
      </Grid>
      <Grid container item direction="row" className={classes.controlPanel} wrap="nowrap">
        <Grid item xs={6} className={classes.marginRight}>
          {valueModal}
        </Grid>
        {alarmModal && (
          <Grid item xs={6}>
            {alarmModal}
          </Grid>
        )}
      </Grid>
    </Grid>
  );
};

export default Knob;
