/**
 * @summary Tracks User Activity
 * events : ['load', 'mousemove', 'mousedown', 'click', 'scroll', 'keypress']
 *
 * @file Component to track User Activity based on user idle time
 *
 */
import React, { useCallback, useEffect, useState } from 'react';
import { useSelector } from 'react-redux';
import { useHistory } from 'react-router-dom';
import { getParametersIsVentilating } from '../../store/controller/selectors/measurements';
import { setMultiPopupOpen } from './Service';

/**
 * IdleTimer
 *
 * @component Idle Timer tracker
 *
 * @param {number} timeout desc for timeout
 * @param {function} onTimeOut desc for onTimeOut
 *
 * @returns {JSX.Element}
 */
const IdleTimer = ({ timeout, onTimeOut }: { timeout: number; onTimeOut(): void }): JSX.Element => {
  const idle = useCallback(() => {
    if (onTimeOut) {
      onTimeOut();
    }
  }, [onTimeOut]);

  useEffect(() => {
    let idleTimeout: NodeJS.Timeout;

    /**
     * The function handles for idle timeout.
     */
    const setTimeouts = () => {
      idleTimeout = setTimeout(idle, timeout);
    };

    /**
     * The function handles for clering timeout if it finds idle value.
     */
    const clearTimeouts = () => {
      if (idleTimeout) {
        clearTimeout(idleTimeout);
      }
    };

    const events = ['load', 'mousemove', 'mousedown', 'click', 'scroll', 'keypress'];

    /**
     * The function handles reset timeout.
     */
    const resetTimeout = () => {
      clearTimeouts();
      setTimeouts();
    };

    events.forEach((event: string) => {
      window.addEventListener(event, resetTimeout);
    });

    setTimeouts();
    return () => {
      events.forEach((event: string) => {
        window.removeEventListener(event, resetTimeout);
        clearTimeouts();
      });
    };
  }, [idle, timeout]);
  return <React.Fragment />;
};

/**
 * UserActivity
 *
 * @component For tracking the user activity.
 *
 * @returns {JSX.Element}
 */
export const UserActivity = (): JSX.Element => {
  // 10 year timeout, so that we can temporarily disable the screensaver for v0.7
  const [idleTimeout] = useState(10 * 52 * 7 * 24 * 60 * 60 * 1000);
  // const [idleTimeout] = useState(10 * 60 * 1000);
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);

  /**
   * Handle callback once `IdleTimer` is timed out
   */
  const onTimeOut = () => {
    if (ventilating) {
      setMultiPopupOpen(false);
      history.push('/screensaver');
    }
  };
  return <IdleTimer timeout={idleTimeout} onTimeOut={onTimeOut} />;
};

export default UserActivity;
