/**
 * @summary UI components for alarms and alarm muting in the topbar
 *
 * @file Shows the number of alarms, message of the most recent alarm, and alarm mute status. Also allows toggling alarm muting and showing the event log modal.
 */
import React, { useState } from 'react';
import { createSelector } from 'reselect';
import { useSelector } from 'react-redux';
import { Button, Grid, makeStyles } from '@material-ui/core';
import { LogEvent } from '../../../store/proto/mcu_pb';
import {
  getNumActiveAlarms,
  getHasActiveAlarms,
  getAlarmMuteActive,
  getMostRecentEvent,
} from '../../../store/controller/selectors';
import { getEventType } from '../../event-logs/EventType';
import { LOGS_ROUTE } from '../navigation/constants';
import BellIcon from './icons/BellIcon';
import {
  AlarmMuteCountdown,
  AlarmMuteToggleButton,
  AlarmMuteCanceller,
} from '../../alarms/muting/AlarmMuting';
import EventLogsModal from '../../event-logs/EventLogsModal';

const useStyles = makeStyles(() => ({
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
  eventLogButton: {
    minWidth: 0,
    borderRadius: 5,
    lineHeight: 'normal',
    marginRight: '16px',
    padding: '6px 10px',
  },
}));

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
      <AlarmMuteCountdown className={classes.alarmMuteCountdown} />
    </Button>
  );
};

/**
 * Alarms
 *
 * @component Component for alarms and event log functionality in the topbar
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 */
export const Alarms = (): JSX.Element => {
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
  const openEventLogsModal = (filter: boolean) => {
    setOpen(true);
    setActiveFilter(filter);
  };

  return (
    <div style={{ display: 'flex' }}>
      <EventLogsModal
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
            openEventLogsModal(false);
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

export default Alarms;
