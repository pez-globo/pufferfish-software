/**
 * @summary Page where range of all alarms in system can be configured
 *
 * @file Alarms page has capablity to paginate if there are many alarms
 * Alarms listed are based on current Ventialtion Mode selected
 * TODO: move this into modules/alarms/limits and rename this file, to reflect
 * that this page is only for alarm limits, not for alarms!
 */
import { Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import Pagination from '@material-ui/lab/Pagination';
import React, { useEffect } from 'react';
import { useSelector } from 'react-redux';
import { VentilationMode } from '../../../store/proto/mcu_pb';
import { getParametersRequestMode } from '../../../store/controller/selectors';
import { setActiveRotaryReference } from '../../app/Service';
import ModeBanner, { BannerType } from '../../modes/ModeBanner';
import { BPM, PERCENT } from '../../info/units';
import AlarmLimitsModalPopup from './AlarmLimitsModalPopup';
import ParamAlarms from './ParamAlarms';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    justifyContent: 'space-between',
    alignItems: 'stretch',
    flexWrap: 'nowrap',
  },
  panel: {
    borderRadius: theme.panel.borderRadius,
    flexWrap: 'nowrap',
    marginBottom: theme.spacing(2),
  },
  leftContainer: {
    justifyContent: 'space-between',
    marginRight: theme.spacing(2),
  },
  paginationContainer: {
    alignItems: 'center',
    width: 'auto',
    marginLeft: 'auto',
    // border: '1px solid blue'
  },
  infoPanel: {
    flexDirection: 'column',
    justifyContent: 'center',
    borderRadius: theme.panel.borderRadius,
    padding: theme.spacing(2),
    backgroundColor: theme.palette.background.paper,
    marginBottom: theme.spacing(2),
  },
  numberButton: {
    padding: '5px 0px',
    backgroundColor: 'transparent',
    border: `1px solid ${theme.palette.common.white}`,
    marginRight: theme.spacing(1),
    borderRadius: 8,
    height: '100%',
    minWidth: 55,

    '&:hover': {
      backgroundColor: 'transparent',
    },
  },
}));

/**
 * @typedef AlarmConfiguration
 *
 * Interface for the alarm configuration
 *
 * @prop {string} label Alarm label to display in UI
 * @prop {number} min Minimum Range of an Alarm (Default to 0)
 * @prop {number} max Minimum Range of an Alarm (Default to 100)
 * @prop {string} stateKey Unique identifier of alarm (eg spo2, fio2...)
 * @prop {number} step Alarm step difference between Range (Defaults to 1)
 */
export interface AlarmConfiguration {
  label: string;
  min?: number;
  max?: number;
  units: string;
  stateKey: string;
  step?: number;
}

/**
 * @param {VentilationMode | null} ventilationMode - List of Alarms displayed in UI based on selected ventilation mode
 *
 * @returns {AlarmConfiguration[]} List of alarms
 *
 */
export const alarmConfiguration = (
  ventilationMode: VentilationMode | null,
): Array<AlarmConfiguration> => {
  switch (ventilationMode) {
    case VentilationMode.hfnc:
      return [
        { label: 'SpO2', stateKey: 'spo2', units: PERCENT },
        { label: 'HR', stateKey: 'hr', max: 200, units: BPM },
      ];
    case VentilationMode.pc_ac:
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return [];
  }
};

/**
 * Main container of Alarms page
 *
 * @component A container for housing all alarm configurations.
 *
 * @returns {JSX.Element}
 */
export const AlarmsPage = (): JSX.Element => {
  const classes = useStyles();

  const currentMode = useSelector(getParametersRequestMode);
  const alarmConfig = alarmConfiguration(currentMode);
  const itemsPerPage = 5;
  const [page, setPage] = React.useState(1);
  const [pageCount, setPageCount] = React.useState(1);

  /**
   * Trigger on Pagination click event
   *
   * @param {React.ChangeEvent<unknown>} event - Default Change event object
   * @param {number} value - Page number value to update current page
   */
  const handleChange = (event: React.ChangeEvent<unknown>, value: number) => {
    setPage(value);
  };

  /**
   * Calls everytime when `alarmConfig` variable changes
   * Updates the page count
   */
  useEffect(() => {
    setPageCount(Math.ceil(alarmConfig.length / itemsPerPage));
  }, [alarmConfig]);

  /**
   * Triggers an event to reset active UI wrapper of Alarm Controls
   * Resets highlighting border around alarm container when clicked across the page
   * Border is usually added on `ValueClicker` button click
   * This allows to reset rotary reference border when user clic
   */
  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid container direction="column" className={classes.root} onClick={OnClickPage}>
      <Grid container item xs direction="column" className={classes.panel}>
        <Grid container item xs direction="row">
          <Grid container spacing={3} style={{ margin: '-10px -12px' }}>
            {/* Splits Alarms based on page number & itemsPerPage count to show in the page */}
            <ParamAlarms alarmConfig={alarmConfig} itemsPerPage={itemsPerPage} page={page} />
          </Grid>
        </Grid>
        <Grid container item xs>
          {/* <Grid item>
            <Typography variant="h3">Alarms</Typography>
          </Grid> */}
          <Grid container className={classes.paginationContainer}>
            {pageCount > 1 && (
              <Grid item>
                <Pagination
                  count={pageCount}
                  page={page}
                  onChange={handleChange}
                  defaultPage={1}
                  variant="outlined"
                  shape="rounded"
                  size="large"
                />
              </Grid>
            )}
            <Grid item style={{ textAlign: 'right' }}>
              <AlarmLimitsModalPopup />
            </Grid>
          </Grid>
        </Grid>
        {/* Right Container for Storing Alarm Slides */}
        <Grid item xs>
          <ModeBanner bannerType={BannerType.NORMAL} />
        </Grid>
      </Grid>
      {/* Title */}
    </Grid>
  );
};

export default AlarmsPage;
