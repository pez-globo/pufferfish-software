/**
 * @summary All components which runs based on Events or in background
 *
 * @file These components are included in higher order file like Layout files
 *
 */
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Subscription } from 'rxjs';
import { BACKEND_CONNECTION_DOWN, RED_BORDER } from '../../store/app/types';
import {
  getAlarmMuteActive,
  getAlarmMuteRequestActive,
  getHasActiveAlarms,
  getNextLogEvents,
  getScreenStatusLock,
} from '../../store/controller/selectors';
import { MessageType } from '../../store/controller/types';

import ModalPopup from '../controllers/ModalPopup';
import MultiStepWizard from '../displays/MultiStepWizard';
import { getScreenLockPopup, setScreenLockPopup } from './Service';
import { updateState } from '../../store/controller/actions';
import { LogEvent, LogEventCode, LogEventType } from '../../store/controller/proto/mcu_pb';
import { getBackendConnected, getBackendHeartBeat, getClock } from '../../store/app/selectors';
import { establishedBackendConnection } from '../../store/app/actions';

const useStyles = makeStyles((theme: Theme) => ({
  overlay: {
    width: '100%',
    height: '100%',
    background: 'rgb(0 0 0 / 39%)',
    position: 'absolute',
    zIndex: 9999,
  },
  label: {
    fontSize: '10px',
  },
  marginHeader: {
    textAlign: 'center',
    marginTop: theme.spacing(3),
    marginBottom: theme.spacing(1),
  },
}));

/**
 * variable to define backend connection timeout
 */
export const BACKEND_CONNECTION_TIMEOUT = 3000;

/**
 * HeartbeatBackendListener
 *
 * @component Dispatches BACKEND_CONNECTION_DOWN event if no backendHeartbeat updates before timeout
 *
 * @returns {JSX.Element}
 */
export const HeartbeatBackendListener = (): JSX.Element => {
  const clock = useSelector(getClock);
  const dispatch = useDispatch();
  const heartbeat = useSelector(getBackendHeartBeat);
  const events = useSelector(getNextLogEvents);
  const diff = Math.abs(new Date().valueOf() - new Date(heartbeat).valueOf());

  useEffect(() => {
    const lostConnectionAlarm = events.find(
      (el: LogEvent) => (el.code as number) === LogEventCode.backend_connection_down,
    );

    if (diff > BACKEND_CONNECTION_TIMEOUT) {
      if (!lostConnectionAlarm) {
        dispatch({
          type: BACKEND_CONNECTION_DOWN,
          clock: new Date(),
          update: {
            code: LogEventCode.backend_connection_down,
            type: LogEventType.system,
            time: new Date().getTime(),
          },
        });
      }
    } else {
      dispatch(establishedBackendConnection(new Date()));
    }
  }, [clock, diff, dispatch, events, heartbeat]);

  return <React.Fragment />;
};

/**
 * Dispatches RED_BORDER event & toggles Audio Alarm
 *
 * @component Dispatches RED_BORDER event if any activeAlarms are present
 *
 * @returns {JSX.Element}
 */
const AudioAlarm = (): JSX.Element => {
  const dispatch = useDispatch();
  const activeAlarms = useSelector(getHasActiveAlarms, shallowEqual);
  const alarmMuteActive = useSelector(getAlarmMuteActive, shallowEqual);
  const backendConnected = useSelector(getBackendConnected, shallowEqual);
  const alarmMuteRequestActive = useSelector(getAlarmMuteRequestActive, shallowEqual);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;

  /**
   * Toggle between Play/Pause
   * UseEffect causes status change to Play/Pause based on `playing` state change
   * useEffect also pasues audio whenever component instance is out of context or destroyed
   * So Based on local state Audio is played or paused.
   */
  useEffect(() => {
    if (activeAlarms) {
      if (backendConnected) {
        if (alarmMuteActive) {
          audio.pause();
        } else {
          audio.play();
        }
      } else if (!backendConnected) {
        if (alarmMuteRequestActive) {
          audio.pause();
        } else {
          audio.play();
        }
      }
    }
    return () => {
      audio.pause();
    };
  }, [activeAlarms, alarmMuteActive, audio, backendConnected, alarmMuteRequestActive]);

  /**
   * On activeAlarms redux store changes, update RED_BORDER & Audio Play state
   */
  useEffect(() => {
    if (activeAlarms || !backendConnected) {
      dispatch({ type: RED_BORDER, status: true });
    } else {
      dispatch({ type: RED_BORDER, status: false });
    }
  }, [activeAlarms, backendConnected, dispatch]);

  /**
   * On alarmMuteStatus redux store changes, update RED_BORDER & Audio Play state
   */
  useEffect(() => {
    if (activeAlarms) {
      dispatch({ type: RED_BORDER, status: !alarmMuteActive });
    }
    if (!backendConnected) {
      dispatch({ type: RED_BORDER, status: !alarmMuteRequestActive });
    }
  }, [alarmMuteActive, activeAlarms, dispatch, alarmMuteRequestActive, backendConnected]);
  return <React.Fragment />;
};

/**
 * ScreenLockModal
 *
 * @component Manages Locking/UnLocking Screen feature
 *
 * @returns {JSX.Element}
 */
export const ScreenLockModal = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  const [open, setOpen] = React.useState(false);

  useEffect(() => {
    const alertPopup = () => {
      if (open) {
        return;
      }
      setTimeout(() => {
        setOpen(false);
      }, 30000);
    };

    /**
     * Listens to screenLock status to close screenLock info popup in 'n' seconds (30s)
     */
    const popupEventSubscription: Subscription = getScreenLockPopup().subscribe(
      (state: boolean) => {
        setOpen(state);
        alertPopup();
      },
    );
    return () => {
      if (popupEventSubscription) {
        popupEventSubscription.unsubscribe();
      }
    };
  }, [open]);

  /**
   * On Confirming unlock screen, updates ScreenStatus to redux store
   */
  const onConfirm = () => {
    dispatch(updateState(MessageType.ScreenStatus, { lock: false }));
    setScreenLockPopup(false);
  };

  return (
    <ModalPopup
      withAction={true}
      maxWidth="xs"
      label="Screen is locked"
      open={open}
      onClose={() => setScreenLockPopup(false)}
      onConfirm={onConfirm}
    >
      <Grid container alignItems="center" className={classes.marginHeader}>
        <Grid item xs>
          <Typography variant="h6">Confirm unlock screen?</Typography>
        </Grid>
      </Grid>
    </ModalPopup>
  );
};

/**
 * OverlayScreen
 *
 * @component Showing overlay screen with an alert when user clicks anywhere while screen is locked
 *
 * @returns {JSX.Element}
 */
export const OverlayScreen = (): JSX.Element => {
  const classes = useStyles();
  const screenStatus = useSelector(getScreenStatusLock);
  const [overlay, setOverlay] = useState(screenStatus || false);
  /**
   * Listens to screenLock status changes & update overlay state accordingly
   */
  useEffect(() => {
    const popupEventSubscription: Subscription = getScreenLockPopup().subscribe(
      (state: boolean) => {
        if (screenStatus) {
          setOverlay(!state);
        }
      },
    );
    return () => {
      if (popupEventSubscription) {
        popupEventSubscription.unsubscribe();
      }
    };
  }, [screenStatus]);

  /**
   * On screenStatus redux store changes, update overlay state
   */
  useEffect(() => {
    setOverlay(screenStatus);
  }, [screenStatus]);

  return (
    <React.Fragment>
      <ScreenLockModal />
      {overlay && (
        <React.Fragment>
          <div
            role="button"
            tabIndex={0}
            aria-label="Screenlock Alert"
            className={classes.overlay}
            onClick={() => setScreenLockPopup(true)}
            onKeyDown={() => null}
          />
        </React.Fragment>
      )}
      <HeartbeatBackendListener />
      <AudioAlarm />
      <MultiStepWizard />
    </React.Fragment>
  );
};

export default OverlayScreen;
