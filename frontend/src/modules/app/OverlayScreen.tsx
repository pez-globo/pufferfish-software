import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Subscription } from 'rxjs';
import { BACKEND_CONNECTION_DOWN, RED_BORDER } from '../../store/app/types';
import {
  getAlarmMuteActive,
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

export const BACKEND_CONNECTION_TIMEOUT = 3000;

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

const AudioAlarm = (): JSX.Element => {
  const dispatch = useDispatch();
  const activeAlarms = useSelector(getHasActiveAlarms, shallowEqual);
  const alarmMuteActive = useSelector(getAlarmMuteActive, shallowEqual);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;
  const [playing, setPlaying] = useState(false);
  console.log(playing);

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
    if (activeAlarms) setPlaying(!alarmMuteActive);
  }, [activeAlarms, alarmMuteActive]);

  useEffect(() => {
    if (activeAlarms) {
      dispatch({ type: RED_BORDER, status: true });
    } else {
      dispatch({ type: RED_BORDER, status: false });
    }
  }, [activeAlarms, dispatch]);

  useEffect(() => {
    if (activeAlarms) {
      dispatch({ type: RED_BORDER, status: !alarmMuteActive });
    }
  }, [alarmMuteActive, activeAlarms, dispatch]);
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
