/**
 * @summary A re-usable container displaying value and valueclicker to control various alarms
 *
 */
import { Grid, makeStyles, Theme, Typography, useTheme } from '@material-ui/core';
import React, { useMemo, RefObject, useRef, useEffect } from 'react';
import { useDispatch } from 'react-redux';
import { commitDraftRequest } from '../../../store/controller/actions';
import { AlarmLimitsRequest, Range } from '../../../store/proto/mcu_pb';
import { MessageType } from '../../../store/proto/types';
import ValueClicker from '../../shared/value/ValueClicker';
import ValueSlider from '../../shared/value/ValueSlider';
import { setActiveRotaryReference } from '../../../store/app/actions';
import useRotaryReference from '../../shared/rotary/useRotaryReference';

const useStyles = makeStyles((theme: Theme) => ({
  alarmValue: {
    marginRight: theme.spacing(3),
    border: `1px solid ${theme.palette.text.primary}`,
    minWidth: 75,
    paddingLeft: theme.spacing(1),
    paddingRight: theme.spacing(1),
    borderRadius: 8,
  },
  panelBg: {
    justifyContent: 'space-between',
    backgroundColor: theme.palette.background.paper,
    borderRadius: theme.panel.borderRadius,
  },
  panelTitle: {
    padding: '5px 16px',
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  panelSlider: {
    padding: '5px 16px',
    borderTop: `2px dashed ${theme.palette.background.default}`,
  },
}));

/**
 * @typedef AlarmProps
 *
 * Configurable properties passed to initialize an Alarm container
 *
 * @prop {string} label Alarm label to display in UI
 * @prop {number} min Minimum Range of an Alarm (Default to 0)
 * @prop {number} max Minimum Range of an Alarm (Default to 100)
 * @prop {string} stateKey Unique identifier of alarm (eg spo2, fio2...)
 * @prop {number} step Alarm step difference between Range (Defaults to 1)
 * @prop {Record<string, Range>} alarmLimits Object to store Alarm Lower/Higher limits
 * @prop {function} setAlarmLimits Callback triggers to save updated Alarm limits
 */
interface AlarmProps {
  label: string;
  min: number;
  max: number;
  stateKey: string;
  step?: number;
  alarmLimits: AlarmLimitsRequest | null;
  setAlarmLimits(alarmLimits: Partial<AlarmLimitsRequest>): void;
}

enum SliderType {
  LOWER,
  UPPER,
}

/**
 * UI visual container displaying Alarm Slider & Increment/Decrement value clicker
 *
 * @component Component for displaying individual Alarm
 *
 * Uses the [[AlarmProps]] interface
 *
 * @returns {JSX.Element}
 */
const AlarmLimits = ({
  label,
  min,
  max,
  stateKey,
  step,
  alarmLimits,
  setAlarmLimits,
}: AlarmProps): JSX.Element => {
  const classes = useStyles();
  const theme = useTheme();
  const dispatch = useDispatch();
  const { initRefListener } = useRotaryReference(theme);
  const range =
    alarmLimits === null
      ? undefined
      : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
  const rangeValues: number[] = useMemo(
    () => [range === undefined ? NaN : range.lower, range === undefined ? NaN : range.upper],
    [range],
  );
  /**
   * State to manage Wrapper HTML reference of Alarm's lower & higher Controls(ValueSlider & ValueClicker)
   * This wrapper's HTML border is added or removed based on user's interaction with Alarm Controls
   * It is used for UI identification of which alarm value is changing via rotary encoder
   */
  const [refs] = React.useState<Record<string, RefObject<HTMLDivElement>>>({
    [`${stateKey}_LOWER`]: useRef(null),
    [`${stateKey}_HIGHER`]: useRef(null),
  });

  /**
   * Updates Alarm limit value on Slider change event
   *
   * @param {number[]} range - Alarm range values
   *
   */
  const setRangevalue = (range: number[]) => {
    setAlarmLimits({ [stateKey]: { lower: range[0], upper: range[1] } });
  };

  /**
   * Updates Alarm limit value on value clicker click event
   *
   * @param {number} value - Updated value of Alarm range
   * @param {SliderType} type - Upper or Lower Range
   *
   */
  const onClick = (value: number, type: SliderType) => {
    dispatch(
      setActiveRotaryReference(
        type === SliderType.LOWER ? `${stateKey}_LOWER` : `${stateKey}_HIGHER`,
      ),
    );
    dispatch(
      commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, {
        [stateKey]: {
          lower: type === SliderType.LOWER ? value : rangeValues[0],
          upper: type === SliderType.UPPER ? value : rangeValues[1],
        },
      }),
    );
  };

  /**
   * Calls on initalization of the component
   * This is an event listener which listens to user input on Alarm Controls(ValueSlider & ValueClicker)
   * Based on this event Border around Alarm's HTML wrapper is added/removed
   */
  useEffect(() => {
    initRefListener(refs);
  }, [initRefListener, refs]);

  return (
    <Grid container item xs={6} style={{ padding: '10px 12px' }}>
      <Grid container item xs className={classes.panelBg}>
        <Grid item xs={12} className={classes.panelTitle}>
          <Typography variant="h5">{label}</Typography>
        </Grid>
        <Grid
          container
          alignItems="center"
          justify="center"
          item
          xs={6}
          style={{ borderRight: `2px dashed ${theme.palette.background.default}` }}
        >
          <Grid
            ref={refs[`${stateKey}_LOWER`]}
            container
            item
            xs
            justify="center"
            alignItems="center"
            style={{
              padding: theme.spacing(2),
              height: '100%',
            }}
          >
            <Grid item className={classes.alarmValue}>
              <Typography align="center" variant="h4">
                {rangeValues[0] === undefined || Number.isNaN(rangeValues[0])
                  ? '--'
                  : Number(rangeValues[0])}
              </Typography>
            </Grid>
            <Grid item>
              <ValueClicker
                referenceKey={`${stateKey}_LOWER`}
                value={rangeValues[0]}
                onClick={(value: number) => onClick(value, SliderType.LOWER)}
                min={min}
                max={rangeValues[1]}
                direction="column"
              />
            </Grid>
          </Grid>
        </Grid>
        <Grid container alignItems="center" justify="center" item xs={6}>
          <Grid
            ref={refs[`${stateKey}_HIGHER`]}
            container
            item
            xs
            justify="center"
            alignItems="center"
            style={{
              padding: theme.spacing(2),
              height: '100%',
            }}
          >
            <Grid item className={classes.alarmValue}>
              <Typography align="center" variant="h4">
                {rangeValues[1] === undefined || Number.isNaN(rangeValues[1])
                  ? '--'
                  : Number(rangeValues[1])}
              </Typography>
            </Grid>
            <Grid item>
              <ValueClicker
                referenceKey={`${stateKey}_HIGHER`}
                value={rangeValues[1]}
                onClick={(value: number) => onClick(value, SliderType.UPPER)}
                min={rangeValues[0]}
                max={max}
                direction="column"
              />
            </Grid>
          </Grid>
        </Grid>
        <Grid item xs={12} className={classes.panelSlider}>
          <ValueSlider
            disabled={true}
            min={min}
            max={max}
            step={step}
            onChange={setRangevalue}
            rangeValues={rangeValues}
          />
        </Grid>
      </Grid>
    </Grid>
  );
};

export default AlarmLimits;
