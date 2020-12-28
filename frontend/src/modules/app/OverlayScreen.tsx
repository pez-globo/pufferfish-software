import { Grid, makeStyles, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import store from '../../store';
import { getAlarmNotifyStatus, getClock } from '../../store/app/selectors';
import { LogEvent } from '../../store/controller/proto/mcu_pb';
import { getPopupEventLog, getScreenStatus } from '../../store/controller/selectors';
import {
  BACKEND_CONNECTION_LOST,
  BACKEND_CONNECTION_LOST_CODE,
} from '../../store/controller/types';
import ModalPopup from '../controllers/ModalPopup';
import MultiStepWizard from '../displays/MultiStepWizard';

const useStyles = makeStyles(() => ({
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
    // After 2 seconds of no connection
    if (diff > 2000) {
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
  const popupEventLog = useSelector(getPopupEventLog, shallowEqual);
  const notifyAlarm = useSelector(getAlarmNotifyStatus);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;
  const [playing, setPlaying] = useState(false);

  useEffect(() => {
    // if (playing) {
    //   audio.play();
    // } else {
    //   audio.pause();
    // }
    // return () => {
    //   audio.pause();
    // };
  }, [playing, audio]);

  useEffect(() => {
    if (popupEventLog) {
      setPlaying(false);
      if (popupEventLog.code === BACKEND_CONNECTION_LOST_CODE) {
        setPlaying(true);
      }
    } else {
      setPlaying(false);
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [popupEventLog]);

  useEffect(() => {
    setPlaying(notifyAlarm as boolean);
  }, [notifyAlarm]);

  return <React.Fragment />;
};

export const OverlayScreen = (): JSX.Element => {
  const classes = useStyles();
  const screenStatus = useSelector(getScreenStatus);
  const [open, setOpen] = React.useState(false);
  const [overlay, setOverlay] = useState(screenStatus || false);

  useEffect(() => {
    setOverlay(screenStatus);
  }, [screenStatus]);

  const alertPopup = () => {
    if (open) {
      return;
    }
    setOpen(true);
    setTimeout(() => {
      setOpen(false);
    }, 3000);
  };

  return (
    <React.Fragment>
      {overlay && (
        <div
          role="button"
          tabIndex={0}
          aria-label="Screenlock Alert"
          className={classes.overlay}
          onClick={alertPopup}
          onKeyDown={() => null}
        />
      )}
      <ModalPopup withAction={false} label="Alert" open={open}>
        <Grid container direction="row">
          <Grid container item xs direction="column">
            <Grid container item xs wrap="nowrap">
              <Grid container item alignItems="baseline">
                <Typography variant="h5" align="center">
                  Screen is locked
                </Typography>
              </Grid>
            </Grid>
          </Grid>
        </Grid>
      </ModalPopup>
      <HeartbeatBackendListener />
      <AudioAlarm />
      <MultiStepWizard />
    </React.Fragment>
  );
};

export default OverlayScreen;
