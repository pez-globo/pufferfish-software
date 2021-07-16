/**
 * @summary Main Wrapper component for Dashboard
 *
 */
import { Grid, makeStyles } from '@material-ui/core';
import React, { useEffect } from 'react';
import { useSelector } from 'react-redux';
import { useHistory } from 'react-router-dom';
import { VentilationMode } from '../../store/proto/mcu_pb';
import {
  getParametersIsVentilating,
  getParametersRequestMode,
} from '../../store/controller/selectors';
import { QUICKSTART_ROUTE } from '../navigation/constants';
import HFNCMainView from './views/HFNCMainView';
import PressureControlMainView from './views/PressureControlMainView';

const useStyles = makeStyles(() => ({
  root: {
    flexGrow: 1,
    justifyContent: 'center',
    alignItems: 'stretch',
    // marginBottom: theme.spacing(2),
  },
}));

/**
 * renderModeLayout
 *
 * @component Component for loading dashboard wrapper based on ventilation mode.
 *
 * @param {VentilationMode | null} mode Ventilatiom mode
 *
 * @returns {JSX.Element}
 *
 */
const renderModeLayout = (mode: VentilationMode | null): JSX.Element => {
  switch (mode) {
    case VentilationMode.hfnc:
      return <HFNCMainView />;
    case VentilationMode.pc_ac:
      return <PressureControlMainView />;
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return <HFNCMainView />;
  }
};

/**
 * DashboardPage
 *
 * @component Main Wrapper component for Dashboard
 *
 * TODO: Hook into the redux store to get the current dashboard `view` from the `ViewDropdown`
 *
 * @returns {JSX.Element}
 *
 */
export const DashboardPage = (): JSX.Element => {
  const classes = useStyles();
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);
  const currentMode = useSelector(getParametersRequestMode);
  /**
   * Triggers whenever vetilation status in redux store changes
   */
  useEffect(() => {
    if (!ventilating) {
      history.push(QUICKSTART_ROUTE.path);
    }
  }, [ventilating, history]);

  return (
    <Grid container className={classes.root}>
      {/* TODO: Switch views based on `view` state. */}
      {renderModeLayout(currentMode)}
    </Grid>
  );
};

export default DashboardPage;
