/**
 * @summary UI components for alarm muting
 *
 * @file Provides buttons for toggling and showing alarm mute status.
 * TODO: move this file to a better place. Maybe it belongs in a feature slice
 * specifically for alarm muting.
 *
 */
import React, { useEffect } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { Button, makeStyles } from '@material-ui/core';
import VolumeOffIcon from '@material-ui/icons/VolumeOff';
import VolumeUpIcon from '@material-ui/icons/VolumeUp';
import {
  LogEventCode,
  LogEventType,
  AlarmMuteRequest,
  AlarmMuteSource,
} from '../../../store/proto/mcu_pb';
import { MessageType } from '../../../store/proto/types';
import {
  getAlarmMuteStatus,
  getAlarmMuteActive,
  getAlarmMuteSeqNum,
  getAlarmMuteRemaining,
  getFirmwareConnected,
} from '../../../store/controller/selectors';
import {
  commitRequest,
  createEphemeralLogEvent,
  receiveMessage,
} from '../../../store/controller/actions';
import { getBackendConnected } from '../../../store/connection/selectors';

const useStyles = makeStyles(() => ({
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
}));

/**
 * AlarmMuteToggleButton
 *
 * @component Button to toggle alarm muting
 *
 * @returns {JSX.Element}
 */
export const AlarmMuteToggleButton = (): JSX.Element => {
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
 * @typedef AlarmMuteCountdownProps
 *
 * Props for Alarm Mute Countdown
 *
 * @prop {string} className style class name for styling the countdown container
 *
 */
interface AlarmMuteCountdownProps {
  className: string;
}

/**
 * AlarmMuteCountdown
 *
 * @component Button  overlay to display alarm mute countdown
 *
 * @returns {JSX.Element}
 */
export const AlarmMuteCountdown = ({ className }: AlarmMuteCountdownProps): JSX.Element => {
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const alarmMuteRemaining = useSelector(getAlarmMuteRemaining);

  if (!alarmMuteActive) {
    return <></>;
  }

  return (
    <div className={className} style={{ right: 'auto' }}>
      {new Date(alarmMuteRemaining).toISOString().substr(14, 5)}
    </div>
  );
};

/**
 * AlarmMuteCanceller
 *
 * @component Component to cancel alarm mute upon connection loss
 *
 * @returns {JSX.Element}
 */
export const AlarmMuteCanceller = (): JSX.Element => {
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
    dispatch(receiveMessage(MessageType.AlarmMute, { ...alarmMuteStatus, active: false }));
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
