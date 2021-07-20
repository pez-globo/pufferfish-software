/**
 * @summary UI component to display Event highlight in toolbar
 *
 * @file More Specifically shows Active Event count, Event title & Alarm mute status
 *
 */
import React, { useState, useEffect } from 'react';
import { createSelector } from 'reselect';
import { Alert } from '@material-ui/lab';
import { useDispatch, useSelector } from 'react-redux';
import { Button, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import VolumeOffIcon from '@material-ui/icons/VolumeOff';
import VolumeUpIcon from '@material-ui/icons/VolumeUp';
import {
  getActiveLogEventIds,
  getAlarmMuteStatus,
  getAlarmMuteActive,
  getAlarmMuteSeqNum,
  getPopupEventLog,
  getAlarmMuteRemaining,
  getFirmwareConnected,
} from '../../store/controller/selectors';
import ModalPopup from '../controllers/ModalPopup';
import LogsPage from '../logs/LogsPage';
import { BellIcon } from '../icons';
import {
  updateState,
  commitRequest,
  createEphemeralLogEvent,
} from '../../store/controller/actions';
import {
  LogEventCode,
  LogEventType,
  LogEvent,
  AlarmMuteRequest,
  AlarmMuteSource,
} from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/proto/types';
import { getEventType } from '../logs/EventType';
import { getBackendConnected } from '../../store/connection/selectors';

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
  alertButton: {
    minWidth: 0,
    borderRadius: 5,
    lineHeight: 'normal',
    padding: '6px 10px',
    backgroundColor: '#FF0000',
    color: '#fff',
    '&:hover': {
      backgroundColor: '#FF0000',
    },
  },
  alertButtonSpan: {
    width: '300px',
    borderRadius: 5,
    marginRight: '10px',
    backgroundColor: '#FF0000',
    color: '#fff',
    '&:hover': {
      backgroundColor: '#FF0000',
    },
  },
  eventLogButton: {
    minWidth: 0,
    borderRadius: 5,
    lineHeight: 'normal',
    marginRight: '16px',
    padding: '6px 10px',
  },
  timer: {
    justifyContent: 'flex-end',
    padding: '6px 20px',
    backgroundColor: '#88211b',
    borderRadius: '0px 4px 4px 0px',
  },
  alertTimer: {
    width: '300px',
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
  routeLabel: string;
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

// Selector to generate the alarm message text to show in the topbar
const getAlertLabel = createSelector(
  getPopupEventLog,
  getAlarmMuteActive,
  (popupEventLog: LogEvent | undefined, alarmMuteActive: boolean): string => {
    if (popupEventLog !== undefined) {
      const eventType = getEventType(popupEventLog.code);
      return eventType.label;
    }

    if (alarmMuteActive) {
      return 'New alarms will be muted';
    }

    // Currently this isn't used because the giant component return block
    // in EventAlerts does a check on eventCount, which will be zero exactly
    // when popupEventLog is undefined; when eventCount is zero, getAlertLabel
    // isn't used.
    return '';
  },
);

// Selector to determine the number of active alerts
const getAlertCount = createSelector(
  getActiveLogEventIds,
  (activeLogEventIds: number[]): number => activeLogEventIds.length,
);

/**
 * EventAlerts
 *
 * @component Component to display Event log details
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const EventAlerts = ({ routeLabel }: Props): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  /**
   * State to toggle opening logsPage popup
   */
  const [open, setOpen] = useState<boolean>(false);
  /**
   * Stores whether Active events filter is applied on LogsPage listing
   */
  const [activeFilter, setActiveFilter] = useState<boolean>(false);
  /**
   * Selectors to get all Events, Active Event Ids & Alarm mute Status
   */
  const alarmMuteStatus = useSelector(getAlarmMuteStatus);
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteSeqNum = useSelector(getAlarmMuteSeqNum);
  const alarmMuteRemaining = useSelector(getAlarmMuteRemaining);
  const backendConnected = useSelector(getBackendConnected);
  const firmwareConnected = useSelector(getFirmwareConnected);
  const alertLabel = useSelector(getAlertLabel);
  const alertCount = useSelector(getAlertCount);

  /**
   * Update mute AlarmStatus in redux store
   *
   * @param {boolean} state desc for state
   */
  const muteAlarmState = (state: boolean, source: AlarmMuteSource) => {
    if (alarmMuteSeqNum === null) {
      console.error('Alarm mute/unmute button reached illegal state!');
      return;
    }

    dispatch(
      commitRequest<AlarmMuteRequest>(MessageType.AlarmMuteRequest, {
        active: state,
        seqNum: alarmMuteSeqNum + 1,
        source,
      }),
    );
  };

  /**
   * Cancel alarm mute whenever backend disconnects.
   */
  useEffect(() => {
    if (backendConnected || alarmMuteStatus === null || !alarmMuteActive) {
      return;
    }

    // The backend will override this temporary cancellation when it reconnects.
    // Note that the backend and firmware will also cancel any alarm mute when
    // they detect a disconnection, and an alarm mute cannot be initiated during
    // a disconnection, so the entire system will always end up in a consistent
    // state where any alarm mute is cancelled
    dispatch(updateState(MessageType.AlarmMute, { ...alarmMuteStatus, active: false }));
    dispatch(
      createEphemeralLogEvent(
        LogEventCode.alarms_unmuted_frontend_backend_loss,
        LogEventType.system,
      ),
    );
    // It will be overwritten by a persistent log event from the firmware about
    // alarm mute cancellation due to loss of the backend.
    if (alarmMuteStatus.seqNum === null) {
      console.error('Alarm mute/unmute button reached illegal state!');
      return;
    }

    // Also request the backend to cancel the mute. This is needed because if the
    // backend is able to receive data from the frontend but unable to send data
    // to the frontend, then only the frontend will be aware of a connection problem,
    // and thus only the frontend can make the backend cancel the alarm mute.
    dispatch(
      commitRequest<AlarmMuteRequest>(MessageType.AlarmMuteRequest, {
        active: false,
        seqNum: alarmMuteStatus.seqNum + 1,
        source: AlarmMuteSource.frontend_backend_loss,
      }),
    );
  }, [dispatch, backendConnected, alarmMuteActive, alarmMuteStatus]);

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
              {alarmMuteActive && alarmMuteRemaining > 0 && (
                <div className={classes.timerText}>
                  {new Date(alarmMuteRemaining * 1000).toISOString().substr(14, 5)}
                </div>
              )}
              <Button
                style={{ marginLeft: 12, marginRight: 12 }}
                onClick={() => muteAlarmState(!alarmMuteActive, AlarmMuteSource.user_software)}
                variant="contained"
                color="primary"
                disabled={!backendConnected || !firmwareConnected || getAlarmMuteSeqNum === null}
                className={classes.alertButton}
              >
                {!alarmMuteActive ? <VolumeOffIcon /> : <VolumeUpIcon />}
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
      <Button
        style={{ marginLeft: 10, marginRight: 10 }}
        onClick={() => muteAlarmState(!alarmMuteActive, AlarmMuteSource.user_software)}
        variant="contained"
        color="primary"
        disabled={!backendConnected || !firmwareConnected || getAlarmMuteSeqNum === null}
        className={classes.alertButton}
      >
        {!alarmMuteActive ? <VolumeOffIcon /> : <VolumeUpIcon />}
      </Button>
      <Button
        style={{ marginLeft: 10, marginRight: 10, margin: '0px 10px', padding: 0 }}
        variant="contained"
        color="primary"
        className={alertCount > 0 ? classes.alertButtonSpan : classes.alertTimer}
        onClick={alertCount > 0 ? onActiveAlarmClick : undefined}
      >
        <span style={{ padding: '6px 16px', width: 250 }}>{alertLabel}</span>
        {alertCount > 1 && (
          <div
            className={classes.iconBadge}
            style={{ left: -6, right: 'auto', backgroundColor: '#FFF', color: '#ff0000' }}
          >
            {alertCount}
          </div>
        )}
        {alarmMuteActive && alarmMuteRemaining !== undefined && (
          <div className={classes.timer} style={{ right: 'auto' }}>
            {new Date(alarmMuteRemaining * 1000).toISOString().substr(14, 5)}
          </div>
        )}
      </Button>
      <Grid>
        <Button
          style={{ marginLeft: 10, marginRight: 12 }}
          variant="contained"
          color="primary"
          onClick={openPopup}
          className={classes.eventLogButton}
        >
          <BellIcon />
        </Button>
        {routeLabel}
      </Grid>
    </div>
  );
};

export default EventAlerts;
