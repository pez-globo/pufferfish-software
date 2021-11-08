/**
 * @summary Display Setting Tab of Settings page
 *
 * @file All the Display Settings are configured here
 */
import { Box, Button, FormControl, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState, useRef } from 'react';
import { shallowEqual, useSelector } from 'react-redux';
import { ThemeVariant, Unit } from '../../../store/proto/frontend_pb';
import {
  getFrontendDisplaySetting,
  getSystemSettingsRequest,
} from '../../../store/controller/selectors/frontend_pb';
import { ToggleValue } from '../../shared/value/ToggleValue';
import {
  BRIGHTNESS_REFERENCE_KEY,
  HOUR_REFERENCE_KEY,
  MINUTE_REFERENCE_KEY,
  MONTH_REFERENCE_KEY,
  DAY_REFERENCE_KEY,
  YEAR_REFERENCE_KEY,
} from './constants';
import { setActiveRotaryReference } from '../../app/Service';
import ParamValueSpinner from '../../shared/value/ParamValueSpinner';
import { createSelector } from 'reselect';
import { getController } from '../../../store/controller/selectors';
import { ControllerStates } from '../../../store/controller/types';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    marginBottom: theme.spacing(2),
  },
  periodButton: {
    width: '100%',
    border: `1px solid ${theme.palette.common.black}`,
    borderRadius: 8,
    height: '100%',
    minHeight: 55,
    minWidth: 78,
  },
  themeButton: {
    marginBottom: theme.spacing(2),
    width: 100,
  },
  leftPanel: {
    backgroundColor: theme.palette.background.paper,
    borderRadius: theme.panel.borderRadius,
  },
  rightPanel: {
    height: '100%',
    marginLeft: theme.spacing(2),
    backgroundColor: theme.palette.background.paper,
    borderRadius: theme.panel.borderRadius,
  },
  borderBottom: {
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  leftContainer: {
    borderTop: `2px dashed ${theme.palette.background.default}`,
    paddingLeft: theme.spacing(2),
  },
  date: {
    paddingRight: theme.spacing(5),
  },
  dateTime: {
    paddingTop: 10,
    fontFamily: 'sans-serif',
    fontWeight: 100,
  },
  rightBorder: {
    borderRight: `2px dashed ${theme.palette.background.default}`,
  },
  periodFormControl: {
    width: '100%',
    padding: theme.spacing(2),
  },
  selected: {
    background: theme.palette.primary.main,
    '&:hover': {
      background: theme.palette.primary.main,
    },
  },
}));

// Periods for 12-Hour Based Time Format
enum Period {
  AM,
  PM,
}

/**
 * Returns the number of days in a month for a given year.
 *
 * @param {number} month input argument numeric value from 1-12
 * @param{number} year input argument numeric value
 *
 * @returns {Date} some description
 *
 */
const getDaysInMonth = (month: number, year: number) => {
  const days: number = new Date(year, month, 0).getDate();
  return days;
};
//

/**
 * Converts a 24-hour formatted hour to 12-hour period based format.
 *
 * @param {number} hour input argument numeric value from 1-24
 *
 * @returns {number}
 *
 */
const to12HourClock = (hour: number) => {
  return hour % 12 || 12;
};

/**
 * Converts a 12-hour formatted hour to 24-hour based format.
 *
 * @param {number} hour input argument numeric value from 1-24
 * @param {Period} period either AM or PM
 *
 * @returns {number}
 *
 */
const to24HourClock = (hour: number, period: Period) => {
  return period === Period.PM ? hour + 12 : hour;
};

/**
 * @typedef Props
 *
 * Interface for change settings.
 *
 * @prop {function} onSettingChange Callback to be called once Settings changes
 *
 */
interface Props {
  onSettingChange(settings: Record<string, unknown>): void;
}

/**
 * DateTimeDisplay
 *
 * @component to display date and time.
 *
 * @returns {JSX.Element}
 *
 */
const DateTimeDisplay = () => {
  const classes = useStyles();
  // TODO: split into a DateDisplay and a TimeDisplay.
  // TODO: can we consolidate the timestring useEffect or the decomposed
  // TimeDisplay component with the HeaderClock from ToolBar.tsx?
  const [clockDate, setClockDate] = useState(
    new Date().toLocaleDateString([], {
      month: 'long',
      day: 'numeric',
      year: 'numeric',
    }),
  );
  const [clockTime, setClockTime] = useState(
    new Date().toLocaleTimeString([], {
      hour: '2-digit',
      minute: '2-digit',
      second: '2-digit',
      hour12: true,
    }),
  );

  /**
   * UseEffect to update the clock every second
   */
  useEffect(() => {
    const clockTimer = setInterval(() => {
      setClockDate(
        new Date().toLocaleDateString([], {
          month: 'long',
          day: 'numeric',
          year: 'numeric',
        }),
      );
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

  return (
    <React.Fragment>
      <Box className={classes.date}>
        <Typography variant="h5">Date:</Typography>
        <Typography className={classes.dateTime}>{clockDate}</Typography>
      </Box>
      <Box>
        <Typography variant="h5">Time:</Typography>
        <Typography className={classes.dateTime}>{clockTime}</Typography>
      </Box>
    </React.Fragment>
  );
};

/**
 * DisplayTab
 *
 * @component DisplayTab
 *
 * uses [[Props]] interface
 *
 * TODO: Hook this up to the redux state to persist changes across the system's state.
 * We need to make sure that updates to dat
 *
 * @returns {JSX.Element}
 *
 */
export const DisplayTab = ({ onSettingChange }: Props): JSX.Element => {
  const classes = useStyles();
  /**
   * Calls on initalization of the component
   * This is an event listener which listens to user input on `ValueClicker` buttons click
   * Based on this event Border around Alarm's HTML wrapper is added/removed
   */
  const elRefs = {
    [BRIGHTNESS_REFERENCE_KEY]: useRef(null),
    [HOUR_REFERENCE_KEY]: useRef(null),
    [MINUTE_REFERENCE_KEY]: useRef(null),
    [MONTH_REFERENCE_KEY]: useRef(null),
    [DAY_REFERENCE_KEY]: useRef(null),
    [YEAR_REFERENCE_KEY]: useRef(null),
  };
  const systemSettings = useSelector(getSystemSettingsRequest, shallowEqual);
  const displaySettings = useSelector(getFrontendDisplaySetting, shallowEqual);
  const [displayBrightness, setDisplayBrightness] = React.useState(
    systemSettings === null ? 100 : systemSettings.displayBrightness,
  );
  const [theme, setTheme] = React.useState(
    displaySettings === null ? ThemeVariant.dark : displaySettings.theme,
  );
  const [unit, setUnit] = React.useState(
    displaySettings === null ? Unit.imperial : displaySettings.unit,
  );
  const [date] = React.useState<Date>(new Date());
  const [period, setPeriod] = React.useState(date.getHours() >= 12 ? Period.PM : Period.AM);
  const [minute, setMinute] = React.useState(date.getMinutes());
  // TODO: maybe we should use 24-hour formatted time, since that's the international standard?
  // The UI design team may have forgotten that 12-hour format is not internationally standard,
  // and the 24-hour format may be more standard in medical settings.
  // TODO: maybe we should add a way to change the seconds?
  // TODO: the ParamValueSpinners should roll over from max to min (and vice versa):
  // e.g. it should be possible to increase the minute from 59 to 00
  // TODO: currently if we just want to change brightness, we still have to update the clock,
  // because pressing "Apply Changes" will cause dispatch of a new SystemSettings request.
  // We should move adjustment of system time to a separate tab with a separate "Apply Changes" button.
  const [hour, setHour] = React.useState(to12HourClock(date.getHours())); // Note: `date.hours()` is 24-hour formatted.
  const [day, setDay] = React.useState(date.getDate());
  const [month, setMonth] = React.useState(
    date.getMonth() + 1,
  ); /* Note: `getMonth()` returns a value in [0, 11] */
  const [year, setYear] = React.useState(date.getFullYear());

  /**
   * Listens to state change of date, period, minute, hour, day, month, year, unit, theme, brightness
   * And triggers callback to parent component `SettingsPage`
   */
  useEffect(() => {
    const dateChange = new Date(year, month - 1, day, to24HourClock(hour, period), minute);
    onSettingChange({
      displayBrightness,
      theme,
      unit,
      date: dateChange.getTime() / 1000.0,
    });
  }, [
    date,
    period,
    minute,
    hour,
    day,
    month,
    year,
    unit,
    theme,
    displayBrightness,
    onSettingChange,
  ]);

  /**
   * function for handling month change.
   * callback from ParamValueSpinner whenever month is changed
   *
   * @param {number} change - min 1 and max 12
   *
   */
  const handleMonthChange = (change: number) => {
    const maxDaysInMonth = getDaysInMonth(change, year);
    // Update `day` component if its value > max days in the new month.
    if (day > maxDaysInMonth) {
      setDay(maxDaysInMonth);
    }
    setMonth(change);
  };

  /**
   * function for handling button css based on AM and PM period.
   *
   * @param {Period} updatedPeriod - input value AM and PM
   *
   * @returns {string} some description
   *
   */
  const buttonClass = (updatedPeriod: Period) =>
    updatedPeriod === period
      ? `${classes.periodButton} ${classes.selected}`
      : `${classes.periodButton}`;

  /**
   * function for handling page click event
   */
  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid container className={classes.root} onClick={OnClickPage}>
      <Grid container item xs={4}>
        <Grid container item direction="column" className={classes.leftPanel}>
          {/* Brightness */}
          <Grid container item xs direction="row">
            <ParamValueSpinner
              elRefsArray={elRefs}
              reference={BRIGHTNESS_REFERENCE_KEY}
              selector={getDummyValue}
              value={displayBrightness}
              label="Brightness"
              units="%"
              onClick={setDisplayBrightness}
              min={0}
              max={100}
            />
          </Grid>
          {/* Color TODO: This should be a toggle switch. */}
          <Grid
            container
            item
            xs
            direction="column"
            justify="center"
            className={classes.leftContainer}
          >
            <Typography variant="h5" className={classes.root}>
              Color
            </Typography>
            <ToggleValue
              toggleBetween={[
                { label: 'Dark UI', value: ThemeVariant.dark },
                { label: 'Light UI', value: ThemeVariant.light },
              ]}
              onToggle={(selected: number) => setTheme(selected as ThemeVariant)}
              selected={theme}
            />
          </Grid>
          {/* Unit TODO: This should be a toggle switch. */}
          <Grid
            container
            item
            xs
            direction="column"
            justify="center"
            className={classes.leftContainer}
          >
            <Typography variant="h5" className={classes.root}>
              Unit
            </Typography>
            <ToggleValue
              toggleBetween={[
                { label: 'Imperial', value: Unit.imperial },
                { label: 'Metric', value: Unit.metric },
              ]}
              onToggle={(selected: number) => setUnit(selected as Unit)}
              selected={unit}
            />
          </Grid>
          {/* Date & Time */}
          <Grid
            container
            item
            xs
            direction="row"
            justify="flex-start"
            alignItems="center"
            className={classes.leftContainer}
          >
            <DateTimeDisplay />
          </Grid>
        </Grid>
      </Grid>
      {/* Right Panel: Date & Time */}
      <Grid container item xs={8}>
        <Grid container item xs direction="column" className={classes.rightPanel}>
          <Grid container item xs alignItems="stretch" className={classes.borderBottom}>
            <Grid item xs className={classes.rightBorder}>
              <ParamValueSpinner
                elRefsArray={elRefs}
                reference={HOUR_REFERENCE_KEY}
                selector={getDummyValue}
                value={hour}
                label="Hour"
                onClick={setHour}
                min={1}
                max={12}
              />
            </Grid>
            <Grid item xs>
              <ParamValueSpinner
                elRefsArray={elRefs}
                reference={MINUTE_REFERENCE_KEY}
                selector={getDummyValue}
                value={minute}
                label="Minute"
                onClick={setMinute}
                min={0}
                max={59}
              />
            </Grid>
            <Grid container item justify="center" alignItems="center" xs={3}>
              <FormControl component="fieldset" className={classes.periodFormControl}>
                <Grid container item className={classes.root}>
                  <Button
                    onClick={() => setPeriod(Period.AM)}
                    variant="outlined"
                    className={buttonClass(Period.AM)}
                  >
                    <Typography variant="h6">AM</Typography>
                  </Button>
                </Grid>
                <Grid container item>
                  <Button
                    onClick={() => setPeriod(Period.PM)}
                    variant="outlined"
                    className={buttonClass(Period.PM)}
                  >
                    <Typography variant="h6">PM</Typography>
                  </Button>
                </Grid>
              </FormControl>
            </Grid>
          </Grid>
          <Grid container item xs direction="row" className={classes.borderBottom}>
            <Grid item xs className={classes.rightBorder}>
              <ParamValueSpinner
                elRefsArray={elRefs}
                selector={getDummyValue}
                reference={MONTH_REFERENCE_KEY}
                value={month}
                label="Month"
                onClick={handleMonthChange}
                min={1}
                max={12}
              />
            </Grid>
            <Grid item xs>
              <ParamValueSpinner
                elRefsArray={elRefs}
                selector={getDummyValue}
                reference={DAY_REFERENCE_KEY}
                value={day}
                label="Day"
                onClick={setDay}
                min={1}
                max={getDaysInMonth(month, year)}
              />
            </Grid>
            <Grid item xs={3} />
          </Grid>
          <Grid container item xs direction="row" className={classes.borderBottom}>
            <Grid item xs className={classes.rightBorder}>
              <ParamValueSpinner
                elRefsArray={elRefs}
                selector={getDummyValue}
                reference={YEAR_REFERENCE_KEY}
                value={year}
                label="Year"
                onClick={setYear}
                min={1970}
                max={3000 /* TODO: we should make it possible to have no max */}
              />
            </Grid>
            {/* Moved Apply button out of the tab */}
            {/* <Grid container item xs justify='center' alignItems='center' >
                        <Button variant='contained' color='secondary'>
                            Apply Changes
                        </Button>
                    </Grid> */}
          </Grid>
        </Grid>
      </Grid>
    </Grid>
  );
};

const getDummyValue = createSelector(getController, (states: ControllerStates): null => null);

export default DisplayTab;
