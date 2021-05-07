import React, { useCallback, useEffect, useState } from 'react';
import { useSelector } from 'react-redux';
import { useHistory } from 'react-router-dom';
import { getParametersIsVentilating } from '../../store/controller/selectors';
import { setMultiPopupOpen } from './Service';

const IdleTimer = ({ timeout, onTimeOut }: { timeout: number; onTimeOut(): void }): JSX.Element => {
  const idle = useCallback(() => {
    if (onTimeOut) {
      onTimeOut();
    }
  }, [onTimeOut]);

  useEffect(() => {
    let idleTimeout: NodeJS.Timeout;

    const setTimeouts = () => {
      idleTimeout = setTimeout(idle, timeout);
    };

    const clearTimeouts = () => {
      if (idleTimeout) {
        clearTimeout(idleTimeout);
      }
    };

    const events = ['load', 'mousemove', 'mousedown', 'click', 'scroll', 'keypress'];

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

export const UserActivity = (): JSX.Element => {
  // 10 year timeout, so that we can temporarily disable the screensaver for v0.7
  const [idleTimeout] = useState(10 * 52 * 7 * 24 * 60 * 60 * 1000);
  // const [idleTimeout] = useState(10 * 60 * 1000);
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);

  const onTimeOut = () => {
    if (ventilating) {
      setMultiPopupOpen(false);
      history.push('/screensaver');
    }
  };
  return <IdleTimer timeout={idleTimeout} onTimeOut={onTimeOut} />;
};

export default UserActivity;
