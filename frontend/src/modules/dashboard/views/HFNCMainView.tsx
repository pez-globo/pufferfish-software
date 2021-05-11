/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React from 'react';
import {
  getParametersFiO2,
  getParametersFlow,
  getSmoothedFiO2Value,
  getSmoothedFlow,
  getSmoothedSpO2,
  getSmoothedHR,
} from '../../../store/controller/selectors';
import { BPM, LMIN, PERCENT } from '../../info/units';
import LargeValueInfo from '../containers/LargeValueInfo';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100%',
    width: '100%',
    alignItems: 'stretch',
  },
  leftPanel: {
    borderRadius: theme.panel.borderRadius,
    backgroundColor: theme.palette.background.paper,
    marginRight: theme.spacing(2),
    overflow: 'hidden',
  },
  rightPanel: {
    borderRadius: theme.panel.borderRadius,
    backgroundColor: theme.palette.background.paper,
    overflow: 'hidden',
  },
  panelColumn: {
    borderRadius: theme.panel.borderRadius,
    backgroundColor: theme.palette.background.paper,
    display: 'grid',
    overflow: 'hidden',
  },
  panelValue: {
    marginTop: theme.spacing(2),
    marginBottom: theme.spacing(2),
  },
}));

/**
 * HFNCMainView
 *
 * @component HFNCMainView.
 *
 * @returns {JSX.Element}
 *
 */
const HFNCMainView = (): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container direction="row" className={classes.root}>
      <Grid container item xs direction="row" alignItems="stretch" className={classes.leftPanel}>
        <Grid
          container
          item
          xs
          direction="column"
          alignItems="stretch"
          className={classes.panelColumn}
        >
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getSmoothedSpO2}
              label="SpO2"
              stateKey="spo2"
              units={PERCENT}
              showLimits
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getSmoothedHR}
              label="HR"
              stateKey="hr"
              units={BPM}
              showLimits
            />
          </Grid>
        </Grid>
        <Grid
          container
          item
          xs
          direction="column"
          alignItems="stretch"
          className={classes.panelColumn}
        >
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getSmoothedFiO2Value}
              label="FiO2"
              stateKey="fio2"
              units={PERCENT}
              showLimits
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getSmoothedFlow}
              label="Flow"
              stateKey="flow"
              units={LMIN}
              showLimits
            />
          </Grid>
        </Grid>
      </Grid>
      <Grid container item xs={4} direction="row" className={classes.rightPanel} wrap="nowrap">
        <Grid
          container
          item
          xs
          direction="column"
          alignItems="stretch"
          className={classes.panelColumn}
        >
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getParametersFiO2}
              label="FiO2 Setting"
              stateKey="fio2"
              units={PERCENT}
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <LargeValueInfo
              selector={getParametersFlow}
              label="Flow Setting"
              stateKey="flow"
              units={LMIN}
            />
          </Grid>
        </Grid>
      </Grid>
    </Grid>
  );
};

export default HFNCMainView;
