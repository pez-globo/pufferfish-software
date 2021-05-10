/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { Grid, makeStyles } from '@material-ui/core';
import React, { useEffect } from 'react';
import { useSelector } from 'react-redux';
import { useHistory } from 'react-router-dom';
import { VentilationMode } from '../../store/controller/proto/mcu_pb';
import { getIsVentilating, getParametersRequestMode } from '../../store/controller/selectors';
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
 * @component Component for handling layout of ventilation mode.
 * 
 * @param {VentilationMode} mode desc for mode
 *
 * @returns {JSX.Element}
 * 
 */
const renderModeLayout = (mode: VentilationMode): JSX.Element => {
  switch (mode) {
    case VentilationMode.hfnc:
      return <HFNCMainView />;
    case VentilationMode.pc_ac:
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return <PressureControlMainView />;
  }
};

/**
 * DashboardPage
 *
 * @component some description
 * 
 * TODO: Hook into the redux store to get the current dashboard `view` from the `ViewDropdown`
 * 
 * @returns {JSX.Element}
 * 
 */
export const DashboardPage = (): JSX.Element => {
  const classes = useStyles();
  const history = useHistory();
  const ventilating = useSelector(getIsVentilating);
  const currentMode = useSelector(getParametersRequestMode);

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
