import React, { useState, useEffect } from 'react';
import { useDispatch, useSelector, shallowEqual } from 'react-redux';
import { RED_BORDER } from '../../store/app/types';
import { getBackendConnected } from '../../store/connection/selectors';
import {
  getHasActiveAlarms,
  getAlarmMuteActive,
  getAlarmMuteRequestActive,
} from '../../store/controller/selectors';

/**
 * Dispatches RED_BORDER event & toggles Audio Alarm
 *
 * @component Dispatches RED_BORDER event if any activeAlarms are present
 *
 * @returns {JSX.Element}
 */
const AudioAlarm = (): JSX.Element => {
  const dispatch = useDispatch();
  const activeAlarms = useSelector(getHasActiveAlarms, shallowEqual);
  const alarmMuteActive = useSelector(getAlarmMuteActive);
  const backendConnected = useSelector(getBackendConnected);
  const alarmMuteRequestActive = useSelector(getAlarmMuteRequestActive, shallowEqual);
  const [audio] = useState(new Audio(`${process.env.PUBLIC_URL}/alarm.mp3`));
  audio.loop = true;

  /**
   * Toggle between Play/Pause
   * UseEffect causes status change to Play/Pause based on `playing` state change
   * useEffect also pasues audio whenever component instance is out of context or destroyed
   * So Based on local state Audio is played or paused.
   */
  useEffect(() => {
    if (activeAlarms) {
      if (backendConnected) {
        if (alarmMuteActive) {
          audio.pause();
        } else {
          audio.play();
        }
      } else if (!backendConnected) {
        if (alarmMuteRequestActive) {
          audio.pause();
        } else {
          audio.play();
        }
      }
    }
    return () => {
      audio.pause();
    };
  }, [activeAlarms, alarmMuteActive, audio, backendConnected, alarmMuteRequestActive]);

  /**
   * On activeAlarms redux store changes, update RED_BORDER & Audio Play state
   */
  useEffect(() => {
    if (activeAlarms || !backendConnected) {
      dispatch({ type: RED_BORDER, status: true });
    } else {
      dispatch({ type: RED_BORDER, status: false });
    }
  }, [activeAlarms, backendConnected, dispatch]);

  /**
   * On alarmMuteStatus redux store changes, update RED_BORDER & Audio Play state
   */
  useEffect(() => {
    if (activeAlarms) {
      dispatch({ type: RED_BORDER, status: !alarmMuteActive });
    }
    if (!backendConnected) {
      dispatch({ type: RED_BORDER, status: !alarmMuteRequestActive });
    }
  }, [alarmMuteActive, activeAlarms, dispatch, alarmMuteRequestActive, backendConnected]);
  return <React.Fragment />;
};

export default AudioAlarm;
