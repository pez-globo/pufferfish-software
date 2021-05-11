/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { AppBar, Button, Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React, { useCallback, useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Link, useHistory, useLocation } from 'react-router-dom';
import { getClockTime } from '../../store/app/selectors';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { VentilationMode } from '../../store/controller/proto/mcu_pb';
import {
  getBatteryPower,
  getChargingStatus,
  getIsVentilating,
  getParametersRequestMode,
  getParametersRequestStandby,
  getAlarmLimitsRequestStandby,
  getPopupEventLog,
} from '../../store/controller/selectors';
import { BACKEND_CONNECTION_LOST_CODE, ALARM_LIMITS } from '../../store/controller/types';
import ViewDropdown from '../dashboard/views/ViewDropdown';
import { BackIcon } from '../icons';
import ClockIcon from '../icons/ClockIcon';
import Power25Icon from '../icons/Power25Icon';
import Power50Icon from '../icons/Power50Icon';
import Power75Icon from '../icons/Power75Icon';
import PowerChargingIcon from '../icons/PowerChargingIcon';
import PowerFullIcon from '../icons/PowerFullIcon';
import { PERCENT } from '../info/units';
import ModesDropdown from '../modes/ModesDropdown';
import {
  DASHBOARD_ROUTE,
  LOGS_ROUTE,
  QUICKSTART_ROUTE,
  SCREENSAVER_ROUTE,
} from '../navigation/constants';
import EventAlerts from './EventAlerts';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    // border: '1px solid red',
  },
  marginRight: {
    marginRight: theme.spacing(0.5),
  },
  paddingRight: {
    paddingRight: theme.spacing(1),
  },
  toolContainer: {
    height: '100%',
    // border: '1px solid red'
  },
}));

/**
 * HeaderClock
 *
 * @component A container for displaying header clock of the page.
 *
 * @returns {JSX.Element} current time
 */
export const HeaderClock = (): JSX.Element => {
  const classes = useStyles();
  const clockTime = useSelector(getClockTime);
  return <span className={classes.paddingRight}>{clockTime}</span>;
};

/**
 * PowerIndicator
 *
 * @component  A container for displaying battery indicator of the page.
 *
 * @returns {JSX.Element} battery percentage remain
 */

export const PowerIndicator = (): JSX.Element => {
  const classes = useStyles();
  const batteryPower = useSelector(getBatteryPower);
  const chargingStatus = useSelector(getChargingStatus);
  const [icon, setIcon] = useState(<PowerFullIcon style={{ fontSize: '2.5rem' }} />);

  useEffect(() => {
    if (batteryPower >= 0 && batteryPower <= 25) {
      setIcon(<Power25Icon style={{ fontSize: '2.5rem' }} />);
    } else if (batteryPower > 25 && batteryPower <= 50) {
      setIcon(<Power50Icon style={{ fontSize: '2.5rem' }} />);
    } else if (batteryPower > 50 && batteryPower <= 75) {
      setIcon(<Power75Icon style={{ fontSize: '2.5rem' }} />);
    } else {
      setIcon(<PowerFullIcon style={{ fontSize: '2.5rem' }} />);
    }

    if (chargingStatus) {
      setIcon(<PowerChargingIcon style={{ fontSize: '2.5rem' }} />);
    }
  }, [batteryPower, chargingStatus]);

  return (
    <React.Fragment>
      <span className={classes.paddingRight}>{`${
        batteryPower !== undefined ? batteryPower.toFixed(0) : '--'
      }${PERCENT}`}</span>
      {icon}
    </React.Fragment>
  );
};

/**
 * ToolBar
 *
 * @component A container for displaying buttons that handle system changes based on
 * various states and conditions such as ventilator state and current page route.
 *
 * @param {React.ReactNode} children desc for children
 * @param {boolean} staticStart desc for staticStart
 *
 * @returns {JSX.Element}
 */
export const ToolBar = ({
  children,
  staticStart = false,
}: {
  children?: React.ReactNode;
  staticStart?: boolean;
}): JSX.Element => {
  const classes = useStyles();
  // Store the route location so we can change button/breadcrumb displays
  // depending on the current route.
  const location = useLocation();
  const dispatch = useDispatch();
  const history = useHistory();
  const currentMode = useSelector(getParametersRequestMode);
  const popupEventLog = useSelector(getPopupEventLog, shallowEqual);
  const parameterRequestStandby = useSelector(getParametersRequestStandby, shallowEqual);
  const alarmLimitsRequestStandby = useSelector(getAlarmLimitsRequestStandby, shallowEqual);
  const ventilating = useSelector(getIsVentilating);
  const [isVentilatorOn, setIsVentilatorOn] = React.useState(ventilating);
  const [label, setLabel] = useState('Start Ventilation');
  const [isDisabled, setIsDisabled] = useState(false);
  // const isDisabled = !isVentilatorOn && location.pathname !== QUICKSTART_ROUTE.path;

  /**
   * some description
   */
  const updateVentilationStatus = () => {
    if (!staticStart) {
      dispatch(updateCommittedParameter({ ventilating: !isVentilatorOn }));
      setIsVentilatorOn(!isVentilatorOn);
    }
    if (isVentilatorOn || staticStart) {
      history.push(QUICKSTART_ROUTE.path);
    }
  };

  /**
   * some description
   */
  const initParameterUpdate = useCallback(() => {
    if (isVentilatorOn) {
      switch (currentMode) {
        case VentilationMode.hfnc:
          dispatch(
            updateCommittedParameter({
              fio2: parameterRequestStandby.fio2,
              flow: parameterRequestStandby.flow,
            }),
          );
          dispatch(
            updateCommittedState(ALARM_LIMITS, {
              spo2: alarmLimitsRequestStandby.spo2,
              hr: alarmLimitsRequestStandby.hr,
            }),
          );
          break;
        case VentilationMode.pc_ac:
        case VentilationMode.vc_ac:
        case VentilationMode.niv_pc:
        case VentilationMode.niv_ps:
        case VentilationMode.psv:
          dispatch(
            updateCommittedParameter({
              peep: parameterRequestStandby.peep,
              vt: parameterRequestStandby.vt,
              rr: parameterRequestStandby.rr,
              fio2: parameterRequestStandby.fio2,
            }),
          );
          break;
        default:
          break;
      }
    }
  }, [isVentilatorOn, parameterRequestStandby, alarmLimitsRequestStandby, currentMode, dispatch]);

  useEffect(() => {
    if (popupEventLog && popupEventLog.code === BACKEND_CONNECTION_LOST_CODE) {
      setIsDisabled(true);
    } else {
      setIsDisabled(false);
    }
  }, [popupEventLog]);

  useEffect(() => {
    if (!ventilating) initParameterUpdate();
  }, [ventilating, initParameterUpdate]);

  useEffect(() => {
    if (ventilating) {
      history.push(DASHBOARD_ROUTE.path);
    }
    setIsVentilatorOn(ventilating);
    setLabel(ventilating ? 'Pause Ventilation' : 'Start Ventilation');
  }, [ventilating, history]);

  const StartPauseButton = (
    <Button
      onClick={updateVentilationStatus}
      variant="contained"
      color="secondary"
      disabled={staticStart ? false : isDisabled}
    >
      {staticStart ? 'Start' : label}
    </Button>
  );
  const tools = [<ModesDropdown />];
  if (location.pathname === DASHBOARD_ROUTE.path) {
    tools.push(<ViewDropdown />);
  } else if (location.pathname === QUICKSTART_ROUTE.path) {
    // tools.push(
    //   <Button variant="contained" color="primary" disabled>
    //     Last Patient Settings
    //   </Button>,
    // );
  } else if (isVentilatorOn && location.pathname !== SCREENSAVER_ROUTE.path) {
    tools.push(
      <Button component={Link} to={DASHBOARD_ROUTE.path} variant="contained" color="primary">
        <BackIcon style={{ paddingRight: 8 }} />
        {DASHBOARD_ROUTE.label}
      </Button>,
    );
  }
  if (location.pathname !== '/') {
    tools.push(<EventAlerts label={LOGS_ROUTE.label} />);
  }

  return (
    <AppBar color="transparent" elevation={0} position="static">
      <Grid
        container
        direction="row"
        justify="space-between"
        alignItems="center"
        className={classes.root}
        wrap="nowrap"
      >
        {children}
        <Grid
          container
          item
          xs={8}
          direction="row"
          justify="flex-start"
          alignItems="center"
          wrap="nowrap"
          zeroMinWidth
          className={classes.toolContainer}
        >
          {tools.map((tool) => (
            <Grid item className={classes.marginRight}>
              {tool}
            </Grid>
          ))}
        </Grid>
        <Grid
          container
          item
          xs
          direction="row"
          justify="flex-end"
          alignItems="center"
          zeroMinWidth
          className={classes.toolContainer}
        >
          <Grid container item xs justify="flex-end" alignItems="center">
            <PowerIndicator />
            <HeaderClock />
            <ClockIcon style={{ fontSize: '2.5rem' }} />
          </Grid>
          <Grid item>{StartPauseButton}</Grid>
        </Grid>
      </Grid>
    </AppBar>
  );
};

export default ToolBar;
