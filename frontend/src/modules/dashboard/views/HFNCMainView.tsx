/**
 * @summary UI container for displaying HFNC mode ValueControl
 *
 * @file UI container with only `LargeValueInfo` containers
 *
 */
import { Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React from 'react';
import { shallowEqual, useSelector } from 'react-redux';
import {
  getHRAlarmLimitsCurrent,
  // getAlarmLimitsCurrent,
  getParametersFiO2,
  getParametersFlow,
  getSmoothedFiO2Value,
  getSmoothedFlow,
  getSmoothedHR,
  getSmoothedSpO2,
  getSpO2AlarmLimitsCurrent,
} from '../../../store/controller/selectors';
import { BPM, LMIN, PERCENT } from '../../info/units';
import ValueInfo from '../components/ValueInfo';

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
 * @component HFNCMainView container
 *
 * @returns {JSX.Element}
 *
 */
const HFNCMainView = (): JSX.Element => {
  const classes = useStyles();
  const spo2AlarmLimits = useSelector(getSpO2AlarmLimitsCurrent, shallowEqual);
  const hrAlarmLimits = useSelector(getHRAlarmLimitsCurrent, shallowEqual);

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
            <ValueInfo
              selector={getSmoothedSpO2}
              label="SpO2"
              stateKey="spo2"
              units={PERCENT}
              alarmLimits={spo2AlarmLimits}
              showLimits
              isLarge
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <ValueInfo
              selector={getSmoothedHR}
              label="HR"
              stateKey="hr"
              units={BPM}
              alarmLimits={hrAlarmLimits}
              showLimits
              isLarge
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
            <ValueInfo
              selector={getSmoothedFiO2Value}
              label="FiO2"
              stateKey="fio2"
              units={PERCENT}
              isLarge
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <ValueInfo
              selector={getSmoothedFlow}
              label="Flow"
              stateKey="flow"
              units={LMIN}
              isLarge
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
            <ValueInfo
              selector={getParametersFiO2}
              label="FiO2 Setting"
              stateKey="fio2"
              units={PERCENT}
              isLarge
            />
          </Grid>
          <Grid container item justify="center" alignItems="stretch" className={classes.panelValue}>
            <ValueInfo
              selector={getParametersFlow}
              label="Flow Setting"
              stateKey="flow"
              units={LMIN}
              isLarge
            />
          </Grid>
        </Grid>
      </Grid>
    </Grid>
  );
};

export default HFNCMainView;
