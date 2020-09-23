import React, { useState, useEffect, useRef, RefObject } from 'react';
import { Link } from 'react-router-dom';
import { Button, Snackbar, makeStyles, Theme, Grid, Typography, Popover } from '@material-ui/core';
import { Alert } from '@material-ui/lab';
import { useSelector } from 'react-redux';
import { BellIcon } from '../icons';
import { getPatientAlarmEvent } from '../../store/controller/selectors';
import { AlarmCode } from '../../store/controller/proto/mcu_pb';

export const ALARM_EVENT_PATIENT = 'Patient';
export const ALARM_EVENT_SYSTEM = 'System';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100%',
    paddingTop: theme.spacing(1),
    paddingBottom: theme.spacing(2),
    paddingLeft: theme.spacing(2),
    paddingRight: theme.spacing(2),
  },
  controlPanel: {
    justifyContent: 'space-between',
    alignItems: 'center',
    minHeight: 40,
  },
  marginRight: {
    marginRight: theme.spacing(1),
  },
  marginTop: {
    marginTop: theme.spacing(1),
  },
  openButton: {
    width: '100%',
    border: `1px solid ${theme.palette.common.black}`,
  },
}));

export const getEventType = (code: AlarmCode): { type: string; label: string } => {
  switch (code) {
    case AlarmCode.fio2_too_low:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'fiO2 is too low',
      };
    case AlarmCode.fio2_too_high:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'fiO2 is too high',
      };
    case AlarmCode.rr_too_low:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'Respiratory Rate is too low',
      };
    case AlarmCode.rr_too_high:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'Respiratory Rate is too high',
      };
    case AlarmCode.spo2_too_low:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'spO2 is too low',
      };
    case AlarmCode.spo2_too_high:
      return {
        type: ALARM_EVENT_PATIENT,
        label: 'spO2 is too high',
      };
    case AlarmCode.battery_low:
      return {
        type: ALARM_EVENT_SYSTEM,
        label: 'Battery power is low',
      };
    case AlarmCode.screen_locked:
      return {
        type: ALARM_EVENT_SYSTEM,
        label: 'Battery power is low',
      };
    default:
      return { type: '', label: '' };
  }
};

interface Props {
  path: string;
  label: string;
}

export const AlertToast = ({
  onClose,
  label,
}: {
  onClose(close: React.SyntheticEvent): void;
  label: string;
}): JSX.Element => {
  const classes = useStyles();
  return (
    <Alert icon={false} onClose={onClose} variant="filled" severity="error">
      <Grid container direction="column" className={classes.root}>
        <Grid item xs style={{ width: '100%' }}>
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

export const EventAlerts = ({ path, label }: Props): JSX.Element => {
  const classes = useStyles();
  const [alert, setAlert] = useState({ label: 'Peep Above upper boundary!' });
  const [open, setOpen] = React.useState(false);
  const buttonRef = useRef(null);
  const handleClose = (event?: React.SyntheticEvent, reason?: string) => {
    setOpen(false);
  };
  const patientAlarmEvent = useSelector(getPatientAlarmEvent);
  // Generate Random Alerts
  useEffect(() => {
    if (patientAlarmEvent.header && patientAlarmEvent.header.id) {
      const eventType = getEventType(patientAlarmEvent.header.code);
      if (eventType.type) {
        setOpen(true);
        setAlert({ label: eventType.label });
      }
    }
    // const randomAlertInterval = setInterval(() => {
    // }, 30000);
    // return () => clearInterval(randomAlertInterval);
  }, [patientAlarmEvent]);

  return (
    <div>
      <Button
        style={{ marginRight: 12 }}
        component={Link}
        to={path}
        variant="contained"
        color="primary"
        ref={buttonRef}
      >
        <BellIcon />
      </Button>
      {label}
      {buttonRef && (
        <Popover
          className={classes.marginTop}
          open={open}
          anchorEl={buttonRef.current}
          anchorOrigin={{
            vertical: 'bottom',
            horizontal: 'left',
          }}
          transformOrigin={{
            vertical: 'top',
            horizontal: 'left',
          }}
        >
          <AlertToast label={alert.label} onClose={handleClose} />
        </Popover>
      )}
    </div>
  );
};

export default EventAlerts;
