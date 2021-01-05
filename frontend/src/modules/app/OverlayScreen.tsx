import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Subscription } from 'rxjs';
import store from '../../store';
import { getClock } from '../../store/app/selectors';
import { RED_BORDER } from '../../store/app/types';
import { LogEvent } from '../../store/controller/proto/mcu_pb';
import {
  getAlarmMuteStatus,
  getPopupEventLog,
  getScreenStatus,
} from '../../store/controller/selectors';
import {
  BACKEND_CONNECTION_LOST,
  BACKEND_CONNECTION_LOST_CODE,
  MessageType,
} from '../../store/controller/types';
import ModalPopup from '../controllers/ModalPopup';
import MultiStepWizard from '../displays/MultiStepWizard';
import { getScreenLockPopup, setScreenLockPopup } from './Service';
import { updateState } from '../../store/controller/actions';

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

export const HeartbeatBackendListener = (): JSX.Element => {
  const clock = useSelector(getClock);
  const dispatch = useDispatch();

  useEffect(() => {
    const storeData = store.getState();
    const heartbeat: Date = storeData.controller.heartbeatBackend.time;
    const diff = Math.abs(new Date().valueOf() - new Date(heartbeat).valueOf());
    const events = storeData.controller.nextLogEvents.elements;
    const lostConnectionAlarm = events.find(
      (el: LogEvent) => (el.code as number) === BACKEND_CONNECTION_LOST_CODE,
    );
    // After 3 seconds of no connection
    if (diff > 3000) {
      if (!lostConnectionAlarm) {
        dispatch({
          type: BACKEND_CONNECTION_LOST,
          update: {
            code: BACKEND_CONNECTION_LOST_CODE,
            time: new Date().getTime() / 1000,
          },
        });
      }
    }
  }, [clock, dispatch]);

  return <React.Fragment />;
};

const AudioAlarm = (): JSX.Element => {
  const dispatch = useDispatch();
  const popupEventLog = useSelector(getPopupEventLog, shallowEqual);
  const alarmMuteStatus = useSelector(getAlarmMuteStatus, shallowEqual);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;
  const [playing, setPlaying] = useState(alarmMuteStatus.active);

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
    if (popupEventLog) {
      setPlaying(false);
      if (popupEventLog.code === BACKEND_CONNECTION_LOST_CODE) {
        setPlaying(true);
      }
      dispatch({ type: RED_BORDER, status: true });
    } else {
      setPlaying(false);
      dispatch({ type: RED_BORDER, status: false });
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [popupEventLog]);

  useEffect(() => {
    if (popupEventLog) {
      dispatch({ type: RED_BORDER, status: !alarmMuteStatus.active });
    }
    if (popupEventLog && popupEventLog.code === BACKEND_CONNECTION_LOST_CODE) {
      setPlaying(!alarmMuteStatus.active);
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [alarmMuteStatus.active, dispatch]);

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
  const screenStatus = useSelector(getScreenStatus);
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
