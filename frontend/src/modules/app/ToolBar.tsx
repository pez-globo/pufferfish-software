/**
 * @summary Toolbar
 *
 * @file Header toolbar with dynamic sub components
 *
 */
import { AppBar, Button, Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React, { useEffect, useState } from 'react';
import { batch, shallowEqual, useDispatch, useSelector } from 'react-redux';
import { useHistory, useLocation } from 'react-router-dom';
import { getBackendConnected } from '../../store/connection/selectors';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import {
  ParametersRequest,
  VentilationMode,
  AlarmLimitsRequest,
  Range,
} from '../../store/proto/mcu_pb';
import {
  getBatteryPowerLeft,
  getChargingStatus,
  getParametersIsVentilating,
  getParametersRequestMode,
  getParametersRequestDraft,
  getAlarmLimitsRequestDraft,
  getStoreReady,
  getAlarmLimitsRequestUnsaved,
  getAlarmLimitsRequest,
  getVentilatingStatusChanging,
  getFirmwareConnected,
} from '../../store/controller/selectors';
import { MessageType } from '../../store/proto/types';
import { ModalPopup } from '../modals/ModalPopup';
import ViewDropdown from '../dashboard/views/ViewDropdown';
import ClockIcon from '../icons/ClockIcon';
import Power25Icon from '../icons/Power25Icon';
import Power50Icon from '../icons/Power50Icon';
import Power75Icon from '../icons/Power75Icon';
import PowerChargingIcon from '../icons/PowerChargingIcon';
import PowerFullIcon from '../icons/PowerFullIcon';
import { PERCENT } from '../info/units';
import ModesDropdown from '../modes/ModesDropdown';
import { DASHBOARD_ROUTE, QUICKSTART_ROUTE } from '../navigation/constants';
import Alarms from '../alarms/Alarms';
import { DiscardAlarmLimitsContent } from '../modals';
import { getModalPopupOpenState, setModalPopupOpen } from './Service';
import { Subscription } from 'rxjs';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    // border: '1px solid red',
  },
  marginRight: {
    marginRight: theme.spacing(0.5),
  },
  clockPadding: {
    paddingRight: theme.spacing(1),
    paddingLeft: theme.spacing(1),
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
 * shows the current time in toolbar
 *
 * @returns {JSX.Element} current time
 */
export const HeaderClock = (): JSX.Element => {
  const classes = useStyles();
  // we can initialize with something more specific like wall clock time
  const [clockTime, setClockTime] = useState(
    new Date().toLocaleTimeString([], {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: true,
    }),
  );

  /**
   * UseEffect to run the clock every second
   */
  useEffect(() => {
    const clockTimer = setInterval(() => {
      setClockTime(
        new Date().toLocaleTimeString([], {
          hour: '2-digit',
          minute: '2-digit',
          second: '2-digit',
          hour12: true,
        }),
      );
    }, 100);
    return () => {
      clearInterval(clockTimer);
    };
  }, []);
  return <span className={classes.clockPadding}>{clockTime}</span>;
};

/**
 * PowerIndicator
 *
 * @component  A container for displaying the battery and power management status of the ventilator.
 *
 * @returns {JSX.Element} battery percentage remain
 */

export const PowerIndicator = (): JSX.Element => {
  const classes = useStyles();
  const batteryPower = useSelector(getBatteryPowerLeft);
  const chargingStatus = useSelector(getChargingStatus);
  const [icon, setIcon] = useState(<PowerFullIcon style={{ fontSize: '2.5rem' }} />);
  /**
   * Updates Icons based on batteryPower/chargingStatus
   */
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

const Tools = (): JSX.Element => {
  const classes = useStyles();
  const location = useLocation();
  /**
   * Display buttons dynamically on the toolbar
   */
  const tools: JSX.Element[] = [];
  if (location.pathname === DASHBOARD_ROUTE.path) {
    tools.push(<ViewDropdown />);
  } else if (location.pathname === QUICKSTART_ROUTE.path) {
    // tools.push(
    //   <Button variant="contained" color="primary" disabled>
    //     Last Patient Settings
    //   </Button>,
    // );
  }
  if (location.pathname !== '/') {
    tools.push(<ModesDropdown />, <Alarms />);
  }
  return (
    <>
      {tools.map((tool) => (
        <Grid item className={classes.marginRight}>
          {tool}
        </Grid>
      ))}
    </>
  );
};

const StartButton = ({ staticStart }: { staticStart?: boolean }): JSX.Element => {
  const dispatch = useDispatch();
  const storeReady = useSelector(getStoreReady);
  const ventilating = useSelector(getParametersIsVentilating);
  const ventilatingStatusChanging = useSelector(getVentilatingStatusChanging);
  const alarmLimitsRequestUnsaved = useSelector(getAlarmLimitsRequestUnsaved);
  const firmwareConnected = useSelector(getFirmwareConnected);
  const backendConnected = useSelector(getBackendConnected);
  const currentMode = useSelector(getParametersRequestMode);
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsRequestDraft = (alarmLimitsRequestDraftSelect as unknown) as Record<
    string,
    Range
  >;
  const parameterRequestDraft = useSelector(getParametersRequestDraft, shallowEqual);
  const history = useHistory();

  const dispatchParameterRequest = (update: Partial<ParametersRequest>) => {
    dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  /**
   * State to manage ventilation label
   * Label is Dynamic based on ventilation state
   */
  const [label, setLabel] = useState('Start');
  /**
   * State to toggle if Ventilating isDisabled
   */
  const [isDisabled, setIsDisabled] = useState(false);

  /**
   * Updates Ventilation status on clicking Start/Pause ventilation
   */
  const updateVentilationStatus = () => {
    if (ventilating) {
      // if ventilating and there are unsaved alarm limit changes then open modal popup
      if (alarmLimitsRequestUnsaved) {
        setModalPopupOpen(true);
        return;
      }
      // if both firmware and backend are connected, and response.ventilating is true
      // then on pressing 'Pause Ventilation' we go back to QuickStart page
      // instead of the page we are on in ventilating mode (eg: settings, alarms screen)
      if (firmwareConnected && backendConnected) history.push(QUICKSTART_ROUTE.path);
    } else {
      initParameterUpdate();
    }
    dispatchParameterRequest({ ventilating: !ventilating });
  };

  /**
   * Update Paramters to redux store when ventilation starts
   */
  const initParameterUpdate = () => {
    if (parameterRequestDraft === null || alarmLimitsRequestDraft === null) {
      return;
    }

    switch (currentMode) {
      case VentilationMode.hfnc:
        batch(() => {
          dispatch(
            commitRequest<ParametersRequest>(MessageType.ParametersRequest, {
              fio2: parameterRequestDraft.fio2,
              flow: parameterRequestDraft.flow,
            }),
          );
          dispatch(
            commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, {
              spo2: alarmLimitsRequestDraft.spo2,
              hr: alarmLimitsRequestDraft.hr,
            }),
          );
        });
        break;
      case VentilationMode.pc_ac:
      case VentilationMode.vc_ac:
      case VentilationMode.niv_pc:
      case VentilationMode.niv_ps:
      case VentilationMode.psv:
        dispatchParameterRequest({
          peep: parameterRequestDraft.peep,
          vt: parameterRequestDraft.vt,
          rr: parameterRequestDraft.rr,
          fio2: parameterRequestDraft.fio2,
        });
        break;
      default:
        break;
    }
  };

  /**
   * Enable/Disable and set label for Start/Pause Ventilation button
   */
  useEffect(() => {
    if (ventilating) {
      setIsDisabled(ventilatingStatusChanging);
      setLabel(ventilatingStatusChanging ? 'Pausing...' : 'Pause');
      return;
    }
    setIsDisabled(ventilatingStatusChanging || !firmwareConnected || !backendConnected);
    if (ventilatingStatusChanging) {
      setLabel('Starting...');
      return;
    }
    setLabel(!firmwareConnected || !backendConnected ? 'Connecting...' : 'Start');
  }, [ventilatingStatusChanging, backendConnected, firmwareConnected, ventilating]);

  /**
   * Switch to dashboard page if ventilating
   */
  useEffect(() => {
    if (storeReady && ventilating) {
      history.push(DASHBOARD_ROUTE.path);
    }
  }, [ventilating, history, storeReady]);

  const StartPauseVentilationButton = (
    <Button
      style={{ marginRight: 10, marginLeft: 10 }}
      onClick={updateVentilationStatus}
      variant="contained"
      color="secondary"
      disabled={isDisabled}
    >
      {label}
    </Button>
  );

  const LandingPageStartButton = (
    <Button
      onClick={() => history.push(QUICKSTART_ROUTE.path)}
      variant="contained"
      color="secondary"
      disabled={!storeReady}
    >
      {storeReady ? 'Start' : 'Loading...'}
    </Button>
  );

  return (
    <>
      <Grid item>{staticStart ? LandingPageStartButton : StartPauseVentilationButton}</Grid>
      <StartButtonModalPopup />
    </>
  );
};

const StartButtonModalPopup = (): JSX.Element => {
  const dispatch = useDispatch();
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);
  const alarmLimitsRequestSelect = useSelector(getAlarmLimitsRequest);
  const alarmLimitsRequest = (alarmLimitsRequestSelect as unknown) as Record<string, Range>;

  const setAlarmLimitsRequestDraft = (data: Partial<AlarmLimitsRequest>) => {
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, data));
  };
  const dispatchParameterRequest = (update: Partial<ParametersRequest>) => {
    dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  const [discardOpen, setDiscardOpen] = useState(false);

  useEffect(() => {
    const popupEventSubscription: Subscription = getModalPopupOpenState().subscribe(
      (state: boolean) => {
        setDiscardOpen(state);
      },
    );
    return () => {
      if (popupEventSubscription) {
        popupEventSubscription.unsubscribe();
      }
    };
  }, []);

  /**
   * onClickHandler for Start ModalPopup 'Confirm' button
   */
  const handleStartDiscardConfirm = () => {
    setAlarmLimitsRequestDraft(alarmLimitsRequest);
    dispatchParameterRequest({ ventilating: !ventilating });
    setModalPopupOpen(false);
    history.push(QUICKSTART_ROUTE.path);
  };

  return (
    <ModalPopup
      withAction={true}
      label="Set Alarms"
      open={discardOpen}
      onClose={() => setModalPopupOpen(false)}
      onConfirm={handleStartDiscardConfirm}
    >
      <DiscardAlarmLimitsContent />
    </ModalPopup>
  );
};

/**
 * ToolBar
 *
 * @component A container for displaying buttons that handle system changes based on
 * various states and conditions such as ventilator state and current page route.
 *
 * @param {React.ReactNode} children Adds dynamic items as children for SidebarSlideRoute (eg: Pufferfish Icon & Drawer icon)
 * @param {boolean} staticStart staticStart is used as a config for reusing Start/Pause Ventilation button for LandingPage Layout
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
          <Tools />
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
          <StartButton staticStart={staticStart} />
        </Grid>
      </Grid>
    </AppBar>
  );
};

export default ToolBar;
