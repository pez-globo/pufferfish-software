/**
 * @summary Component for Screensaver page
 *
 */
import React from 'react';
import { makeStyles, Theme } from '@material-ui/core/styles';
import { Grid } from '@material-ui/core';
import { useSelector } from 'react-redux';
import ModeBanner from '../displays/ModeBanner';
import ValueJumbotron from './ValueJumbotron';
import ControlJumbotron from './ControlJumbotron';
import { PERCENT, BMIN, LMIN } from '../info/units';
import {
  getSensorMeasurementsSpO2,
  getCycleMeasurementsRR,
  getROXIndex,
  getSensorMeasurementsFiO2,
  getSensorMeasurementsFlow,
  getParametersRequestMode,
} from '../../store/controller/selectors';
import { VentilationMode } from '../../store/proto/mcu_pb';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    border: '1px solid white',
  },
  gridItems: {
    paddingLeft: theme.spacing(2),
    paddingRight: theme.spacing(2),
    paddingTop: theme.spacing(1),
    paddingBottom: theme.spacing(1),
  },
  screenSavergrid: {
    display: 'grid',
    gridGap: '15px',
    gridTemplateRows: '1fr 140px 100px',
    gridTemplateColumns: '1fr',
  },
  screenSaverItem: {
    textAlign: 'center',
    background: '#fff',
    color: '#000',
    fontSize: '17px',
    padding: '10px',
    fontWeight: 'bolder',
    transform: 'rotate(-180deg)',
    writingMode: 'vertical-rl',
  },
}));

/**
 * HFNCValueGrid
 *
 * @component A component for displaying HFNC value grid.
 *
 * @returns {JSX.Element}
 */
const HFNCValueGrid = (): JSX.Element => {
  const classes = useStyles();

  return (
    <React.Fragment>
      <Grid item xs={3} className={classes.gridItems}>
        <ValueJumbotron selector={getSensorMeasurementsSpO2} label="SpO2" units={PERCENT} />
      </Grid>
      <Grid item xs={3} className={classes.gridItems}>
        <ValueJumbotron selector={getCycleMeasurementsRR} label="RR" units={BMIN} />
      </Grid>
      <Grid item xs={6} className={classes.gridItems}>
        <ValueJumbotron decimal={2} selector={getROXIndex} label="ROX Index" units="" />
      </Grid>
    </React.Fragment>
  );
};

/**
 * HFNCControlGrid
 *
 * @component A component for displaying HFNC control grid.
 *
 * @returns {JSX.Element}
 */
const HFNCControlGrid = (): JSX.Element => {
  const classes = useStyles();

  return (
    <React.Fragment>
      <Grid item xs={3} className={classes.gridItems}>
        <ControlJumbotron selector={getSensorMeasurementsFiO2} label="FiO2" units={PERCENT} />
      </Grid>
      <Grid item xs={3} className={classes.gridItems}>
        <ControlJumbotron selector={getSensorMeasurementsFlow} label="Flow Rate" units={LMIN} />
      </Grid>
      <Grid item xs={3} className={classes.gridItems} />
      <Grid item xs={3} className={classes.gridItems} />
    </React.Fragment>
  );
};

/**
 * ConfigureControlMode
 *
 * @component A component for configuring value control wrapper based on ventilation mode.
 *
 * @param {VentilationMode | null} mode - some desc for mode
 *
 * @returns {JSX.Element}
 */
const ConfigureControlMode = ({ mode }: { mode: VentilationMode | null }): JSX.Element => {
  switch (mode) {
    case VentilationMode.hfnc:
      return <HFNCControlGrid />;
    case VentilationMode.pc_ac:
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return <HFNCControlGrid />;
  }
};

/**
 * ConfigureValueMode
 *
 * @component A component for configuring value grid wrapper based on ventilation mode.
 *
 * @param {VentilationMode | null} mode - Ventilation mode
 *
 * @returns {JSX.Element}
 */
const ConfigureValueMode = ({ mode }: { mode: VentilationMode | null }): JSX.Element => {
  switch (mode) {
    case VentilationMode.hfnc:
      return <HFNCValueGrid />;
    case VentilationMode.pc_ac:
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return <HFNCValueGrid />;
  }
};

/**
 * ScreensaverPage
 *
 * @component A farview screensaver page.
 *
 * @returns {JSX.Element}
 */
export const ScreensaverPage = (): JSX.Element => {
  const classes = useStyles();
  const currentMode = useSelector(getParametersRequestMode);

  return (
    <Grid container direction="column" justify="space-between" className={classes.screenSavergrid}>
      <Grid
        container
        justify="space-evenly"
        alignItems="stretch"
        className={classes.root}
        wrap="nowrap"
      >
        <Grid item xs className={classes.screenSaverItem}>
          Values
        </Grid>
        <ConfigureValueMode mode={currentMode} />
      </Grid>
      <Grid
        container
        justify="space-evenly"
        alignItems="stretch"
        className={classes.root}
        wrap="nowrap"
      >
        <Grid item xs className={classes.screenSaverItem}>
          Control
        </Grid>
        <ConfigureControlMode mode={currentMode} />
      </Grid>
      <Grid item>
        <ModeBanner bannerType="screenSaver" />
      </Grid>
    </Grid>
  );
};

export default ScreensaverPage;
