import React, { useState, useEffect } from 'react';
import { Alert } from '@material-ui/lab';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { Button, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import VolumeOffIcon from '@material-ui/icons/VolumeOff';
import VolumeUpIcon from '@material-ui/icons/VolumeUp';
import { LogEventCode, LogEventType } from '../../store/controller/proto/mcu_pb';
import { BMIN, BPM, LMIN, PERCENT } from '../info/units';
import {
  getActiveLogEventIds,
  getAlarmMuteStatus,
  getPopupEventLog,
} from '../../store/controller/selectors';
import ModalPopup from '../controllers/ModalPopup';
import LogsPage from '../logs/LogsPage';
import { BellIcon } from '../icons';
import { updateCommittedState } from '../../store/controller/actions';
import { ALARM_MUTE, BACKEND_CONNECTION_LOST_CODE } from '../../store/controller/types';

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

export interface EventType {
  type: LogEventType;
  label: string;
  unit: string;
  head?: string;
  stateKey?: string;
}
export const getEventType = (code: LogEventCode): EventType => {
  switch (code) {
    case LogEventCode.fio2_too_low:
      return {
        type: LogEventType.patient,
        label: 'fiO2 is too low',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.fio2_too_high:
      return {
        type: LogEventType.patient,
        label: 'fiO2 is too high',
        head: 'FiO2',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.rr_too_low:
      return {
        type: LogEventType.patient,
        label: 'Respiratory Rate is too low',
        stateKey: 'rr',
        unit: BMIN,
      };
    case LogEventCode.rr_too_high:
      return {
        type: LogEventType.patient,
        label: 'Respiratory Rate is too high',
        stateKey: 'rr',
        unit: BMIN,
      };
    case LogEventCode.hr_too_low:
      return {
        type: LogEventType.patient,
        label: 'Heart Rate is too low',
        stateKey: 'hr',
        unit: BPM,
      };
    case LogEventCode.hr_too_high:
      return {
        type: LogEventType.patient,
        label: 'Heart Rate is too high',
        stateKey: 'hr',
        unit: BPM,
      };
    case LogEventCode.spo2_too_low:
      return {
        type: LogEventType.patient,
        label: 'spO2 is too low',
        stateKey: 'spo2',
        unit: PERCENT,
      };
    case LogEventCode.spo2_too_high:
      return {
        type: LogEventType.patient,
        label: 'spO2 is too high',
        stateKey: 'spo2',
        unit: PERCENT,
      };
    case LogEventCode.fio2_setting_changed:
      return {
        type: LogEventType.control,
        label: 'Fio2 Settings changed',
        stateKey: 'fio2',
        unit: PERCENT,
      };
    case LogEventCode.flow_setting_changed:
      return {
        type: LogEventType.control,
        label: 'Flow Settings changed',
        stateKey: 'flow',
        unit: LMIN,
      };
    case LogEventCode.battery_low:
      return {
        type: LogEventType.system,
        label: 'Battery power is low',
        unit: PERCENT,
      };
    case LogEventCode.screen_locked:
      return {
        type: LogEventType.system,
        label: 'Screen is locked',
        unit: '',
      };
    case BACKEND_CONNECTION_LOST_CODE:
      return {
        type: LogEventType.system,
        label: 'Software connectivity lost',
        unit: '',
      };
    default:
      return { type: LogEventType.system, label: '', unit: '' };
  }
};

interface Props {
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

export const EventAlerts = ({ label }: Props): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  const [alert, setAlert] = useState({ label: '' });
  const [open, setOpen] = useState<boolean>(false);
  const [activeFilter, setActiveFilter] = useState<boolean>(false);
  const [alertCount, setAlertCount] = useState<number>(0);
  const popupEventLog = useSelector(getPopupEventLog, shallowEqual);
  const activeLog = useSelector(getActiveLogEventIds, shallowEqual);
  const alarmMuteStatus = useSelector(getAlarmMuteStatus, shallowEqual);
  const [isMuted, setIsMuted] = useState<boolean>(!alarmMuteStatus.active);
  useEffect(() => {
    if (popupEventLog) {
      const eventType = getEventType(popupEventLog.code);
      if (eventType.label) {
        setAlertCount(activeLog.length);
        setAlert({ label: eventType.label });
      }
    } else {
      setAlertCount(0);
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [popupEventLog, JSON.stringify(activeLog)]);

  useEffect(() => {
    setIsMuted(!alarmMuteStatus.active);
  }, [alarmMuteStatus.active]);

  const muteAlarmState = (state: boolean) => {
    dispatch(updateCommittedState(ALARM_MUTE, { active: state }));
  };

  const onActiveAlarmClick = () => {
    setOpen(true);
    setActiveFilter(true);
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
            style={{ padding: '15px' }}
          >
            <Grid item xs={6}>
              <Typography variant="h4" style={{ fontWeight: 'normal' }}>
                {!activeFilter ? 'Events Log' : 'Active Alarms'}
              </Typography>
            </Grid>
            <Grid container item xs justify="flex-end" alignItems="center">
              {!isMuted && (
                <div className={classes.timerText}>
                  {new Date(alarmMuteStatus.remaining * 1000).toISOString().substr(14, 5)}
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
                style={{ padding: '6px 3rem' }}
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
          style={{ marginLeft: 12 }}
          onClick={() => muteAlarmState(isMuted)}
          variant="contained"
          color="primary"
          className={classes.alertColor}
        >
          {isMuted ? <VolumeOffIcon /> : <VolumeUpIcon />}
        </Button>
        <Button
          style={{ margin: '0px 12px', padding: 0 }}
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
          {!isMuted && (
            <div className={classes.timer}>
              {new Date(alarmMuteStatus.remaining * 1000).toISOString().substr(14, 5)}
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
        <Button
          style={{ marginRight: 12 }}
          variant="contained"
          color="primary"
          onClick={() => setOpen(true)}
        >
          <BellIcon />
        </Button>
        {label}
      </Grid>
    </div>
  );
};

export default EventAlerts;
