/**
 * @summary Modal for showing the event log
 *
 * @file Shows the event log. Allows toggling showing of all events or only active alarms.
 *
 */
import React from 'react';
import { Button, Grid, makeStyles, Typography } from '@material-ui/core';
import EventLogs from './EventLogs';
import ModalPopup from '../shared/ModalPopup';
import { AlarmMuteCountdown, AlarmMuteToggleButton } from '../alarms/muting/AlarmMuting';

const useStyles = makeStyles(() => ({
  alarmMuteCountdownText: {
    fontSize: '0.8rem',
    fontWeight: 'bold',
    marginRight: '10px',
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
export const EventLogModal = ({
  open,
  setOpen,
  activeFilter,
  setActiveFilter,
}: EventLogModalProps): JSX.Element => {
  const classes = useStyles();

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
              <AlarmMuteCountdown className={classes.alarmMuteCountdownText} />
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
        <EventLogs filter={activeFilter} />
      </ModalPopup>
    </>
  );
};

export default EventLogModal;
