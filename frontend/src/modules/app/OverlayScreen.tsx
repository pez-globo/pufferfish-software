import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Subscription } from 'rxjs';
import { BACKEND_CONNECTION_DOWN, RED_BORDER } from '../../store/app/types';
import {
  getAlarmMuteStatus,
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
import { getBackendHeartBeat, getClock } from '../../store/app/selectors';
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

export const BACKEND_CONNECTION_LOST_ALARM_TIMEOUT = 3000;
export const BACKEND_CONNECTION_LOST_TIMEOUT = 400;

export const HeartbeatBackendListener = (): JSX.Element => {
  const clock = useSelector(getClock);
  const dispatch = useDispatch();
  const heartbeat = useSelector(getBackendHeartBeat);
  const events = useSelector(getNextLogEvents);

  useEffect(() => {
    // TODO: can we just use a selector here, or do we need to access the store directly?
    const diff = Math.abs(new Date().valueOf() - new Date(heartbeat).valueOf());
    const lostConnectionAlarm = events.find(
      (el: LogEvent) => (el.code as number) === LogEventCode.backend_connection_down,
    );

    if (diff > BACKEND_CONNECTION_LOST_ALARM_TIMEOUT) {
      if (!lostConnectionAlarm) {
        dispatch({
          type: BACKEND_CONNECTION_DOWN,
          clock: new Date(),
          backendConnection: false,
          update: {
            code: LogEventCode.backend_connection_down,
            type: LogEventType.system,
            time: new Date().getTime(),
          },
        });
      }
    }
  }, [clock, dispatch, events, heartbeat]);

  useEffect(() => {
    // TODO: can we just use a selector here, or do we need to access the store directly?
    const diff = Math.abs(new Date().valueOf() - new Date(heartbeat).valueOf());
    const lostConnectionAlarm = events.find(
      (el: LogEvent) => (el.code as number) === LogEventCode.backend_connection_up,
    );

    if (diff < BACKEND_CONNECTION_LOST_TIMEOUT) {
      if (!lostConnectionAlarm) {
        dispatch(establishedBackendConnection(true, new Date()));
      }
    }
  }, [clock, dispatch, events, heartbeat]);

  return <React.Fragment />;
};

const AudioAlarm = (): JSX.Element => {
  const dispatch = useDispatch();
  const activeAlarms = useSelector(getHasActiveAlarms, shallowEqual);
  const alarmMuteStatus = useSelector(getAlarmMuteStatus, shallowEqual);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;
  const [playing, setPlaying] = useState(alarmMuteStatus !== null && alarmMuteStatus.active);

  useEffect(() => {
    if (playing) {
      audio.play();
    } else {
      audio.pause();
    }
    return () => {
      audio.pause();
    };
  }, [playing, audio]);

  useEffect(() => {
    if (activeAlarms) {
      setPlaying(true);
      dispatch({ type: RED_BORDER, status: true });
    } else {
      setPlaying(false);
      dispatch({ type: RED_BORDER, status: false });
    }
  }, [activeAlarms, dispatch]);

  useEffect(() => {
    if (activeAlarms) {
      dispatch({ type: RED_BORDER, status: alarmMuteStatus !== null && !alarmMuteStatus.active });
      setPlaying(alarmMuteStatus !== null && !alarmMuteStatus.active);
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [alarmMuteStatus !== null && alarmMuteStatus.active, dispatch]);

  return <React.Fragment />;
};

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

export const OverlayScreen = (): JSX.Element => {
  const classes = useStyles();
  const screenStatus = useSelector(getScreenStatusLock);
  const [overlay, setOverlay] = useState(screenStatus || false);

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
