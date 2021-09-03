import { makeStyles, Theme } from '@material-ui/core';
import React, { useState, useEffect } from 'react';

const useStyles = makeStyles((theme: Theme) => ({
  clockPadding: {
    paddingRight: theme.spacing(1),
    paddingLeft: theme.spacing(1),
  },
}));

/**
 * HeaderClock
 *
 * @component A container for displaying header clock of the page.
 * shows the current time in toolbar
 *
 * @returns {JSX.Element} current time
 */
const HeaderClock = (): JSX.Element => {
  const classes = useStyles();
  // we can initialize with something more specific like wall clock time
  const [clockTime, setClockTime] = useState(
    new Date().toLocaleTimeString([], {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: true,
    }),
  );

  /**
   * UseEffect to run the clock every second
   */
  useEffect(() => {
    const clockTimer = setInterval(() => {
      setClockTime(
        new Date().toLocaleTimeString([], {
          hour: '2-digit',
          minute: '2-digit',
          second: '2-digit',
          hour12: true,
        }),
      );
    }, 100);
    return () => {
      clearInterval(clockTimer);
    };
  }, []);
  return <span className={classes.clockPadding}>{clockTime}</span>;
};

export default HeaderClock;
