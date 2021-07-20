/**
 * @summary UI components for alarms and alarm muting in the topbar
 *
 * @file Shows the number of alarms, message of the most recent alarm, and alarm mute status. Also allows toggling alarm muting and showing the event log modal.
 *
 */
import React, { useState, useEffect } from 'react';
import { createSelector } from 'reselect';
import { useDispatch, useSelector } from 'react-redux';
import { Button, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import VolumeOffIcon from '@material-ui/icons/VolumeOff';
import VolumeUpIcon from '@material-ui/icons/VolumeUp';
import {
  LogEventCode,
  LogEventType,
  LogEvent,
  AlarmMuteRequest,
  AlarmMuteSource,
} from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/proto/types';
import {
  getNumActiveAlarms,
  getHasActiveAlarms,
  getAlarmMuteStatus,
  getAlarmMuteActive,
  getAlarmMuteSeqNum,
  getMostRecentEvent,
  getAlarmMuteRemaining,
  getFirmwareConnected,
} from '../../store/controller/selectors';
import {
  updateState,
  commitRequest,
  createEphemeralLogEvent,
} from '../../store/controller/actions';
import { getBackendConnected } from '../../store/connection/selectors';
import ModalPopup from '../controllers/ModalPopup';
import { getEventType } from '../logs/EventType';
import LogsPage from '../logs/LogsPage';
import { LOGS_ROUTE } from '../navigation/constants';
import { BellIcon } from '../icons';

const useStyles = makeStyles((theme: Theme) => ({
  alarmMuteToggleButton: {
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
  activeAlarmButton: {
    width: '300px',
    borderRadius: 5,
    marginRight: '10px',
    backgroundColor: '#FF0000',
    color: '#fff',
    '&:hover': {
      backgroundColor: '#FF0000',
    },
  },
  inactiveAlarmButton: {
    width: '300px',
    margin: '0px 12px',
    backgroundColor: '#0053b336',
    '&:hover': {
      backgroundColor: '#0053b336',
    },
  },
  alarmMuteCountdown: {
    justifyContent: 'flex-end',
    padding: '6px 20px',
    backgroundColor: '#88211b',
    borderRadius: '0px 4px 4px 0px',
  },
  alarmMuteCountdownText: {
    fontSize: '0.8rem',
    fontWeight: 'bold',
    marginRight: '10px',
  },
  eventLogButton: {
    minWidth: 0,
    borderRadius: 5,
    lineHeight: 'normal',
    marginRight: '16px',
    padding: '6px 10px',
  },
}));

/**
 * @typedef EventLogModalProps
 *
 * Props for Event Log Modal
 *
 * @prop {boolean} open whether to show the modal
 * @prop {function} setOpen callback to set whether to show the modal
 * @prop {boolean} activeFilter whether to filter for active alarms
 * @prop {function} setActiveFilter callback to set whether to filter for active alarms
 *
 */
interface EventLogModalProps {
  open: boolean;
  setOpen: (open: boolean) => void;
  activeFilter: boolean;
  setActiveFilter: (activeFilter: boolean) => void;
}

/**
 * EventLogModal
 *
 * @component Modal to show the event log
 *
 * Uses the [[EventLogModalProps]] interface
 *
 * @returns {JSX.Element}
 */
const EventLogModal = ({
  open,
  setOpen,
  activeFilter,
  setActiveFilter,
}: EventLogModalProps): JSX.Element => {
  const classes = useStyles();

  // Selectors
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteRemaining = useSelector(getAlarmMuteRemaining);

  return (
    <>
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
                <div className={classes.alarmMuteCountdownText}>
                  {new Date(alarmMuteRemaining * 1000).toISOString().substr(14, 5)}
                </div>
              )}
              <AlarmMuteToggleButton />
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
    </>
  );
};

/**
 * AlarmMuteToggleButton
 *
 * @component Button to toggle alarm muting
 *
 * @returns {JSX.Element}
 */
const AlarmMuteToggleButton = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();

  // Selectors
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteSeqNum = useSelector(getAlarmMuteSeqNum);
  const backendConnected = useSelector(getBackendConnected);
  const firmwareConnected = useSelector(getFirmwareConnected);

  /**
   * Callback to change alarm mute activation in the redux store
   *
   * @param {boolean} mute Whether the alarm mute should be active
   * @param {AlarmMuteSource} source The reason for changing alarm mute activation
   */
  const setAlarmMute = (mute: boolean, source: AlarmMuteSource) => {
    if (alarmMuteSeqNum === null) {
      console.error('Alarm mute/unmute button reached illegal state!');
      return;
    }

    dispatch(
      commitRequest<AlarmMuteRequest>(MessageType.AlarmMuteRequest, {
        active: mute,
        seqNum: alarmMuteSeqNum + 1,
        source,
      }),
    );
  };

  return (
    <Button
      style={{ marginLeft: 12, marginRight: 12 }}
      onClick={() => setAlarmMute(!alarmMuteActive, AlarmMuteSource.user_software)}
      variant="contained"
      color="primary"
      disabled={!backendConnected || !firmwareConnected || getAlarmMuteSeqNum === null}
      className={classes.alarmMuteToggleButton}
    >
      {!alarmMuteActive ? <VolumeOffIcon /> : <VolumeUpIcon />}
    </Button>
  );
};

/**
 * AlarmCountBadge
 *
 * @component Button to display alarm message and open event log modal
 *
 * @returns {JSX.Element}
 */
const AlarmCountBadge = (): JSX.Element => {
  const classes = useStyles();

  const numActiveAlarms = useSelector(getNumActiveAlarms);

  if (numActiveAlarms <= 1) {
    return <></>;
  }

  return (
    <div
      className={classes.iconBadge}
      style={{ left: -6, right: 'auto', backgroundColor: '#FFF', color: '#ff0000' }}
    >
      {numActiveAlarms}
    </div>
  );
};

/**
 * AlarmMuteCountdown
 *
 * @component Button  overlay to display alarm mute countdown
 *
 * @returns {JSX.Element}
 */
const AlarmMuteCountdown = (): JSX.Element => {
  const classes = useStyles();

  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteRemaining = useSelector(getAlarmMuteRemaining);

  if (!alarmMuteActive) {
    return <></>;
  }

  return (
    <div className={classes.alarmMuteCountdown} style={{ right: 'auto' }}>
      {new Date(alarmMuteRemaining * 1000).toISOString().substr(14, 5)}
    </div>
  );
};

// Selector to generate the alarm message text to show in the topbar
const getAlarmButtonLabel = createSelector(
  getMostRecentEvent,
  getAlarmMuteActive,
  (mostRecentEvent: LogEvent | undefined, alarmMuteActive: boolean): string => {
    if (mostRecentEvent !== undefined) {
      const eventType = getEventType(mostRecentEvent.code);
      return eventType.label;
    }

    if (alarmMuteActive) {
      return 'New alarms will be muted';
    }

    return '';
  },
);

/**
 * @typedef AlarmButtonProps
 *
 * Props for Event Log Modal
 *
 * @prop {function} setOpen callback to set whether to show the events log modal
 * @prop {function} setActiveFilter callback to set whether to filter for active alarms
 *
 */
interface AlarmButtonProps {
  setOpen: (open: boolean) => void;
  setActiveFilter: (activeFilter: boolean) => void;
}

/**
 * AlarmButton
 *
 * @component Button to display alarm message and open event log modal
 *
 * Uses the [[AlarmButtonProps]] interface
 *
 * @returns {JSX.Element}
 */
const AlarmButton = ({ setOpen, setActiveFilter }: AlarmButtonProps): JSX.Element => {
  const classes = useStyles();

  const hasActiveAlarms = useSelector(getHasActiveAlarms);
  const alarmButtonLabel = useSelector(getAlarmButtonLabel);

  // Opens event log modal and sets it to show active alarms
  const openActiveAlarmsModal = () => {
    if (!hasActiveAlarms) {
      return;
    }

    setOpen(true);
    setActiveFilter(true);
  };

  return (
    <Button
      style={{ marginLeft: 10, marginRight: 10, margin: '0px 10px', padding: 0 }}
      variant="contained"
      color="primary"
      className={hasActiveAlarms ? classes.activeAlarmButton : classes.inactiveAlarmButton}
      onClick={openActiveAlarmsModal}
    >
      <span style={{ padding: '6px 16px', width: 250 }}>{alarmButtonLabel}</span>
      <AlarmCountBadge />
      <AlarmMuteCountdown />
    </Button>
  );
};

/**
 * AlarmMuteCanceller
 *
 * @component Component to cancel alarm mute upon connection loss
 *
 * @returns {JSX.Element}
 */
const AlarmMuteCanceller = (): JSX.Element => {
  const dispatch = useDispatch();

  // Selectors
  const alarmMuteStatus = useSelector(getAlarmMuteStatus);
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const backendConnected = useSelector(getBackendConnected);

  // We use a separate component with only this useEffect so that we don't need to
  // redraw anything else when this useEffect runs.
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

  return <></>;
};

/**
 * EventAlerts
 *
 * @component Component for alarms and event log functionality in the topbar
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const EventAlerts = (): JSX.Element => {
  const classes = useStyles();

  // Local State
  // State to toggle opening the events log modal
  const [open, setOpen] = useState<boolean>(false);
  // State to toggle filtering only for active alarms on the events log modal
  const [activeFilter, setActiveFilter] = useState<boolean>(false);

  /**
   * Opens LogsPage popup listing event log details
   *
   * @param {boolean} filter Shows only active events if set true
   */
  const openEventLogModal = (filter: boolean) => {
    setOpen(true);
    setActiveFilter(filter);
  };

  return (
    <div style={{ display: 'flex' }}>
      <EventLogModal
        open={open}
        setOpen={setOpen}
        activeFilter={activeFilter}
        setActiveFilter={setActiveFilter}
      />
      <AlarmMuteToggleButton />
      <AlarmButton setOpen={setOpen} setActiveFilter={setActiveFilter} />
      <Grid>
        <Button
          style={{ marginLeft: 10, marginRight: 12 }}
          variant="contained"
          color="primary"
          onClick={() => {
            openEventLogModal(false);
          }}
          className={classes.eventLogButton}
        >
          <BellIcon />
        </Button>
        {LOGS_ROUTE.label}
      </Grid>
      <AlarmMuteCanceller />
    </div>
  );
};

export default EventAlerts;
