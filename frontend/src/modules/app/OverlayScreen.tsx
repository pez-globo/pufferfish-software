/**
 * @summary All components which runs based on Events or in background
 *
 * @file These components are included in higher order file like Layout files
 *
 */
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { Subscription } from 'rxjs';
import { getScreenStatusLock } from '../../store/controller/selectors';
import { MessageType } from '../../store/proto/types';
import ModalPopup from '../shared/ModalPopup';
import MultiStepWizard from '../ventilation-wizard/MultiStepWizard';
import { getScreenLockPopup, setScreenLockPopup } from './Service';
import { commitRequest } from '../../store/controller/actions';
import { ScreenStatusRequest } from '../../store/proto/mcu_pb';
import AudioAlarm from '../alarms/AudioAlarm';

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
    dispatch(
      commitRequest<ScreenStatusRequest>(MessageType.ScreenStatusRequest, { lock: false }),
    );
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
      <AudioAlarm />
      <MultiStepWizard />
    </React.Fragment>
  );
};

export default OverlayScreen;
