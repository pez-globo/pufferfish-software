import React, { useCallback, useEffect, useState } from "react";
import { Redirect, useHistory } from "react-router-dom";

const IdleTimer = ({ timeout, onTimeOut }: { timeout: number, onTimeOut(): void}): void => {
  
  const idle = useCallback(()=> {
    onTimeOut && onTimeOut()
  },[onTimeOut]) 

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

    const events = [
      'load',
      'mousemove',
      'mousedown',
      'click',
      'scroll',
      'keypress'
    ];

    const resetTimeout = () => {
      clearTimeouts();
      setTimeouts();
    };

    for (let i in events) {
      window.addEventListener(events[i], resetTimeout);
    }

    setTimeouts();
    return () => {
      for (let i in events) {
        window.removeEventListener(events[i], resetTimeout);
        clearTimeouts();
      }
    }
  }, [idle, timeout]);

}

export const UserActivity = () => {
    const [idleTimeout] = useState(10 * 1000) // 10 seconds just for testing
    let history = useHistory()
    const onTimeOut = () => {
      console.log('Timeout triggered')
      history.push('/screensaver')
    }
    return <IdleTimer timeout={idleTimeout} onTimeOut={onTimeOut} />
}

export default UserActivity