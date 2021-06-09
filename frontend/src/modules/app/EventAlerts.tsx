/**
 * @summary UI component to display Event highlight in toolbar
 *
 * @file More Specifically shows Active Event count, Event title & Alarm mute status
 *
 */
import React, { useState, useEffect } from 'react';
import { Alert } from '@material-ui/lab';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Button, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import VolumeOffIcon from '@material-ui/icons/VolumeOff';
import VolumeUpIcon from '@material-ui/icons/VolumeUp';
import {
  getActiveLogEventIds,
  getAlarmMuteActive,
  getAlarmMuteRequestActive,
  getPopupEventLog,
  getAlarmMuteRemaining,
  getAlarmMuteRequestRemaining,
  getParametersIsVentilating,
} from '../../store/controller/selectors';
import ModalPopup from '../controllers/ModalPopup';
import LogsPage from '../logs/LogsPage';
import { BellIcon } from '../icons';
import { commitRequest } from '../../store/controller/actions';
import { AlarmMuteRequest } from '../../store/controller/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { getEventType } from '../logs/EventType';
import { getBackendConnected } from '../../store/app/selectors';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100%',
  },
  alertStyles: {
    '& .MuiAlert-message': {
      width: '100%',
    },
    '& .MuiAlert-action': {
      position: 'absolute',
      right: '15px',
    },
  },
  controlPanel: {
    justifyContent: 'space-between',
    alignItems: 'center',
  },
  alertMargin: {
    marginLeft: theme.spacing(3.7),
    marginRight: theme.spacing(3.7),
  },
  marginRight: {
    marginRight: theme.spacing(1),
  },
  marginTop: {
    marginTop: theme.spacing(1),
    '& .MuiPopover-paper': {
      width: '300px',
      borderRadius: '10px',
    },
  },
  iconBadge: {
    position: 'absolute',
    top: -6,
    right: -6,
    lineHeight: '16px',
    padding: '0 6px 0px 5.4px',
    borderRadius: 15,
    alignItems: 'center',
    justifyContent: 'center',
    fontSize: '11px',
    backgroundColor: '#FF0000',
  },
  openButton: {
    width: '100%',
    // border: `1px solid ${theme.palette.common.black}`,
    backgroundColor: '#e0e0e052',
    color: 'white',
    boxShadow: 'none',
    fontSize: '12px',

    '&:hover': {
      backgroundColor: '#e0e0e052',
      // boxShadow: 'none',
    },
  },
  alertColor: {
    backgroundColor: '#FF0000',
    color: '#fff',
    '&:hover': {
      backgroundColor: '#FF0000',
    },
  },
  timer: { padding: '6px 16px', backgroundColor: '#88211b', borderRadius: '0px 4px 4px 0px' },
  alertTimer: {
    margin: '0px 12px',
    backgroundColor: '#0053b336',
    '&:hover': {
      backgroundColor: '#0053b336',
    },
  },
  alertTextcolor: {
    color: '#ff0000',
    marginRight: '10px',
  },
  timerText: {
    fontSize: '.8rem',
    marginRight: '10px',
  },
}));

/**
 * @typedef Props
 *
 * Props for Event Alerts
 *
 * @prop {string} label active event label
 *
 */
interface Props {
  label: string;
}

/**
 * @deprecated
 *
 * AlertToast
 *
 * @component Alert Toaster showing on active event log
 *
 * @prop {function} onClose - Event on closing Alert Toaster
 * @prop {string} label - Label to show content inside Toaster
 *
 * @returns {JSX.Element}
 */
export const AlertToast = ({
  onClose,
  label,
}: {
  onClose(close: React.SyntheticEvent): void;
  label: string;
}): JSX.Element => {
  const classes = useStyles();
  return (
    <Alert
      className={classes.alertStyles}
      icon={false}
      onClose={onClose}
      variant="filled"
      severity="error"
    >
      <Grid container direction="column" className={classes.root}>
        <Grid item xs style={{ width: '100%', paddingBottom: '15px' }}>
          <Typography variant="h5">{label}</Typography>
        </Grid>
        <Grid container item direction="row" className={classes.controlPanel} wrap="nowrap">
          <Grid item xs={6} className={classes.marginRight}>
            <Button variant="contained" className={classes.openButton}>
              Reset Alarm
            </Button>
          </Grid>
          <Grid item xs={6}>
            <Button variant="contained" className={classes.openButton}>
              Silence for 2 min
            </Button>
          </Grid>
        </Grid>
      </Grid>
    </Alert>
  );
};

/**
 * EventAlerts
 *
 * @component Component to display Event log details
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const EventAlerts = ({ label }: Props): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  /**
   * State to show active event's label
   * Defaults to empty as no active event while initalization
   */
  const [alert, setAlert] = useState({ label: '' });
  /**
   * State to toggle opening logsPage popup
   */
  const [open, setOpen] = useState<boolean>(false);
  /**
   * Stores whether Active events filter is applied on LogsPage listing
   */
  const [activeFilter, setActiveFilter] = useState<boolean>(false);
  /**
   * Stores the number of active alert count
   */
  const [alertCount, setAlertCount] = useState<number>(0);
  /**
   * Selectors to get all Events, Active Event Ids & Alarm mute Status
   */
  const popupEventLog = useSelector(getPopupEventLog, shallowEqual);
  const activeLog = useSelector(getActiveLogEventIds, shallowEqual);
  const activeLogString = JSON.stringify(activeLog);
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteRemaining = useSelector(getAlarmMuteRemaining);
  const backendConnected = useSelector(getBackendConnected);
  const alarmMuteRequestRemaining = useSelector(getAlarmMuteRequestRemaining);
  const alarmMuteRequestActive = useSelector(getAlarmMuteRequestActive);
  const ventilating = useSelector(getParametersIsVentilating);
  /**
   * Stores the state which toggles AlarmMute/AlarmMuteRequest Status
   */
  const [isMuted, setIsMuted] = useState(alarmMuteActive);
  /**
   * Local state to update the AlarmMuteRequest remaining time
   * when backend is disconnected
   */
  const [remaining, setRemaining] = useState(alarmMuteRequestRemaining);
  /**
   * Local variable that decides which timer to display depending on the
   * backend connection
   */
  const countdownTimer = backendConnected ? alarmMuteRemaining : remaining;
  /**
   * Triggers whenever Active or Event log is updated in redux
   */
  useEffect(() => {
    if (popupEventLog) {
      const eventType = getEventType(popupEventLog.code);
      if (eventType.label) {
        setAlertCount(activeLog.length);
        setAlert({ label: eventType.label });
      }
    } else if (alarmMuteActive && ventilating) {
      setAlertCount(1);
      setAlert({ label: 'No Active Alarms' });
    } else {
      setAlert({ label: '' });
      setAlertCount(0);
    }
  }, [popupEventLog, alarmMuteActive, activeLogString, activeLog.length, ventilating]);

  /**
   * Triggers whenever AlarmMute status is updated in redux store
   *
   * Triggers when AlarmMuteRequest is updated in redux store
   * when backend is disconnected
   *
   * starts timer when backend is disconnected
   */
  useEffect(() => {
    setIsMuted(!alarmMuteActive);

    if (!backendConnected) {
      // Update local state that controls the mute button
      setIsMuted(!alarmMuteRequestActive);
      // Reset the timer
      if (!alarmMuteRequestActive) setRemaining(120);
      // Start the timer
      const timer = setTimeout(() => {
        setRemaining(remaining - 1);
        if (remaining <= 0) {
          clearTimeout(timer);
        }
      }, 1000);
    }
  }, [alarmMuteActive, remaining, backendConnected, alarmMuteRequestActive]);

  /**
   * Update mute AlarmStatus in redux store
   *
   * @param {boolean} state desc for state
   */
  const muteAlarmState = (state: boolean) => {
    dispatch(
      commitRequest<AlarmMuteRequest>(MessageType.AlarmMuteRequest, { active: state }),
    );
  };

  /**
   * Dispatch unmute request after 2 min countdown
   */
  useEffect(() => {
    if (alarmMuteRemaining === 0 || remaining === 0) {
      dispatch(
        commitRequest<AlarmMuteRequest>(MessageType.AlarmMuteRequest, { active: false }),
      );
    }
  }, [dispatch, alarmMuteRemaining, remaining]);

  /**
   * Opens LogsPage popup listing event log details
   *
   * @param {boolean} filter Shows only active events if set true
   */
  const openEventLogPopup = (filter: boolean) => {
    setOpen(true);
    setActiveFilter(filter);
  };

  /**
   * Opens LogsPage popup listing event log details
   */
  const openPopup = () => {
    openEventLogPopup(false);
  };

  /**
   * Opens LogsPage popup listing event log details on toggling Active from LogsPage popup header
   */
  const onActiveAlarmClick = () => {
    openEventLogPopup(true);
  };

  return (
    <div style={{ display: 'flex' }}>
      <ModalPopup
        withAction={false}
        label={
          <Grid
            container
            item
            xs
            justify="flex-start"
            alignItems="center"
            wrap="nowrap"
            style={{ paddingRight: '15px' }}
          >
            <Grid item xs={6}>
              <Typography variant="h4" style={{ fontWeight: 'normal' }}>
                {!activeFilter ? 'Events Log' : 'Active Alarms'}
              </Typography>
            </Grid>
            <Grid container item xs justify="flex-end" alignItems="center">
              {!isMuted && countdownTimer !== undefined && (
                <div className={classes.timerText}>
                  {new Date(countdownTimer * 1000).toISOString().substr(14, 5)}
                </div>
              )}
              <Button
                style={{ marginLeft: 12, marginRight: 12 }}
                onClick={() => muteAlarmState(isMuted)}
                variant="contained"
                color="primary"
                className={classes.alertColor}
              >
                {isMuted ? <VolumeOffIcon /> : <VolumeUpIcon />}
              </Button>
              <Button
                onClick={() => setActiveFilter(!activeFilter)}
                variant="contained"
                color="primary"
                style={{ width: '10rem' }}
              >
                {activeFilter ? 'Events Log' : 'Active Alarms'}
              </Button>
            </Grid>
          </Grid>
        }
        open={open}
        fullWidth={true}
        onClose={() => setOpen(false)}
        showCloseIcon={true}
      >
        <LogsPage filter={activeFilter} />
      </ModalPopup>
      <Grid hidden={alertCount <= 0}>
        <Button
          style={{ marginLeft: 5 }}
          onClick={() => muteAlarmState(isMuted)}
          variant="contained"
          color="primary"
          className={classes.alertColor}
        >
          {isMuted ? <VolumeOffIcon /> : <VolumeUpIcon />}
        </Button>
        <Button
          style={{ margin: '0px 10px', padding: 0 }}
          variant="contained"
          color="primary"
          className={classes.alertColor}
          onClick={onActiveAlarmClick}
        >
          <span className={isMuted ? `${classes.alertMargin}` : ''} style={{ padding: '6px 16px' }}>
            {alert.label}
          </span>
          {alertCount > 1 && (
            <div
              className={classes.iconBadge}
              style={{ left: -6, right: 'auto', backgroundColor: '#FFF', color: '#ff0000' }}
            >
              {alertCount}
            </div>
          )}
          {!isMuted && countdownTimer !== undefined && (
            <div className={classes.timer}>
              {new Date(countdownTimer * 1000).toISOString().substr(14, 5)}
            </div>
          )}
        </Button>
      </Grid>
      <Grid hidden={alertCount > 0}>
        <Button variant="contained" color="primary" className={classes.alertTimer}>
          <VolumeUpIcon />
        </Button>
        <Button
          style={{ padding: 0 }}
          variant="contained"
          color="primary"
          className={classes.alertTimer}
        >
          <span style={{ padding: '6px 30px', visibility: 'hidden' }}>{label}</span>
        </Button>
      </Grid>
      <Grid>
        <Button style={{ marginRight: 12 }} variant="contained" color="primary" onClick={openPopup}>
          <BellIcon />
        </Button>
        {label}
      </Grid>
    </div>
  );
};

export default EventAlerts;
