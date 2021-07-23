/**
 * @summary Page where range of all alarms in system can be configured
 *
 * @file Alarms page has capablity to paginate if lot more Alarms
 * Alarms listed are based on current Ventialtion Mode selected
 * TODO: move this into modules/alarms/limits and rename this file, to reflect
 * that this page is only for alarm limits, not for alarms!
 */
import { Button, Grid, Typography } from '@material-ui/core';
import { makeStyles, Theme, useTheme } from '@material-ui/core/styles';
import Pagination from '@material-ui/lab/Pagination';
import React, { RefObject, useEffect, useRef, useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import { AlarmLimitsRequest, VentilationMode, Range } from '../../store/proto/mcu_pb';
import {
  getAlarmLimitsRequest,
  getAlarmLimitsRequestDraft,
  getAlarmLimitsRequestUnsaved,
  getAlarmLimitsUnsavedKeys,
  getParametersIsVentilating,
  getParametersRequestMode,
} from '../../store/controller/selectors';
import { MessageType } from '../../store/proto/types';
import { setActiveRotaryReference } from '../app/Service';
import { ValueClicker } from '../controllers';
import ModalPopup from '../modals/ModalPopup';
import ValueSlider from '../controllers/ValueSlider';
import ModeBanner, { BannerType } from '../displays/ModeBanner';
import useRotaryReference from '../utils/useRotaryReference';
import { DiscardAlarmLimitsContent } from '../modals';
import { BPM, PERCENT } from '../info/units';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    justifyContent: 'space-between',
    alignItems: 'stretch',
    flexWrap: 'nowrap',
  },
  panel: {
    borderRadius: theme.panel.borderRadius,
    flexWrap: 'nowrap',
    marginBottom: theme.spacing(2),
  },
  leftContainer: {
    justifyContent: 'space-between',
    marginRight: theme.spacing(2),
  },
  paginationContainer: {
    alignItems: 'center',
    width: 'auto',
    marginLeft: 'auto',
    // border: '1px solid blue'
  },
  applyButton: {
    border: '1px solid black',
  },
  panelBg: {
    justifyContent: 'space-between',
    backgroundColor: theme.palette.background.paper,
    borderRadius: theme.panel.borderRadius,
  },
  infoPanel: {
    flexDirection: 'column',
    justifyContent: 'center',
    borderRadius: theme.panel.borderRadius,
    padding: theme.spacing(2),
    backgroundColor: theme.palette.background.paper,
    marginBottom: theme.spacing(2),
  },
  marginContent: {
    textAlign: 'center',
    marginTop: theme.spacing(2),
    marginBottom: theme.spacing(3),
  },
  marginHeader: {
    textAlign: 'center',
    marginTop: theme.spacing(3),
    marginBottom: theme.spacing(1),
  },
  panelTitle: {
    padding: '5px 16px',
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  panelSlider: {
    padding: '5px 16px',
    borderTop: `2px dashed ${theme.palette.background.default}`,
  },
  numberButton: {
    padding: '5px 0px',
    backgroundColor: 'transparent',
    border: `1px solid ${theme.palette.common.white}`,
    marginRight: theme.spacing(1),
    borderRadius: 8,
    height: '100%',
    minWidth: 55,

    '&:hover': {
      backgroundColor: 'transparent',
    },
  },
  alarmValue: {
    marginRight: theme.spacing(3),
    border: `1px solid ${theme.palette.text.primary}`,
    minWidth: 75,
    paddingLeft: theme.spacing(1),
    paddingRight: theme.spacing(1),
    borderRadius: 8,
  },
}));

// NOTE: Temporary Alarm until the UI team address interface concerns.
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
  alarmLimits: Range | null;
  requestCommitRange(range: Range): void;
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
const Alarm = ({
  label,
  min,
  max,
  stateKey,
  step,
  alarmLimits,
  requestCommitRange,
}: AlarmProps): JSX.Element => {
  const classes = useStyles();
  const theme = useTheme();
  const { initRefListener } = useRotaryReference(theme);
  const range = alarmLimits === null ? undefined : alarmLimits;
  const [rangeValues, setAlarmLimits] = useState(() => [
    range === undefined ? NaN : range.lower,
    range === undefined ? NaN : range.upper,
  ]);
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
    setAlarmLimits(range);
  };

  /**
   * Triggers whenever rangeValue is updated in redux
   */
  useEffect(() => {
    requestCommitRange(Range.fromJSON({ lower: rangeValues[0], upper: rangeValues[1] }));
  }, [requestCommitRange, rangeValues]);

  /**
   * Updates Alarm limit value on value clicker click event
   *
   * @param {number} value - Updated value of Alarm range
   * @param {SliderType} type - Upper or Lower Range
   *
   */
  const onClick = (value: number, type: SliderType) => {
    setActiveRotaryReference(
      type === SliderType.LOWER ? `${stateKey}_LOWER` : `${stateKey}_HIGHER`,
    );
    setAlarmLimits([
      type === SliderType.LOWER ? value : rangeValues[0],
      type === SliderType.UPPER ? value : rangeValues[1],
    ]);
    requestCommitRange({ lower: rangeValues[0], upper: rangeValues[1] });
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

/**
 * @typedef AlarmConfiguration
 *
 * Interface for the alarm configuration
 *
 * @prop {string} label Alarm label to display in UI
 * @prop {number} min Minimum Range of an Alarm (Default to 0)
 * @prop {number} max Minimum Range of an Alarm (Default to 100)
 * @prop {string} stateKey Unique identifier of alarm (eg spo2, fio2...)
 * @prop {number} step Alarm step difference between Range (Defaults to 1)
 */
export interface AlarmConfiguration {
  label: string;
  min?: number;
  max?: number;
  units: string;
  stateKey: string;
  step?: number;
}

/**
 * @param {VentilationMode | null} ventilationMode - List of Alarms displayed in UI based on selected ventilation mode
 *
 * @returns {AlarmConfiguration[]} List of alarms
 *
 */
export const alarmConfiguration = (
  ventilationMode: VentilationMode | null,
): Array<AlarmConfiguration> => {
  switch (ventilationMode) {
    case VentilationMode.hfnc:
      return [
        { label: 'SpO2', stateKey: 'spo2', units: PERCENT },
        { label: 'HR', stateKey: 'hr', max: 200, units: BPM },
      ];
    case VentilationMode.pc_ac:
    case VentilationMode.vc_ac:
    case VentilationMode.niv_pc:
    case VentilationMode.niv_ps:
    case VentilationMode.psv:
    default:
      return [];
  }
};

/**
 * Main container of Alarms page
 *
 * @component A container for housing all alarm configurations.
 *
 * @returns {JSX.Element}
 */
export const AlarmsPage = (): JSX.Element => {
  const classes = useStyles();
  const itemsPerPage = 5;
  const [page, setPage] = React.useState(1);
  const [pageCount, setPageCount] = React.useState(1);
  const [parameter] = React.useState<AlarmProps>();
  const [key] = React.useState('');

  /**
   * Trigger on Pagination click event
   *
   * @param {React.ChangeEvent<unknown>} event - Default Change event object
   * @param {number} value - Page number value to update current page
   */
  const handleChange = (event: React.ChangeEvent<unknown>, value: number) => {
    setPage(value);
  };

  const dispatch = useDispatch();
  const currentMode = useSelector(getParametersRequestMode);
  const ventilating = useSelector(getParametersIsVentilating);
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsRequestSelect = useSelector(getAlarmLimitsRequest);
  const alarmLimitsRequestUnsaved = useSelector(getAlarmLimitsRequestUnsaved);
  const alarmLimitsUnsavedKeys = useSelector(getAlarmLimitsUnsavedKeys);
  const alarmLimitsRequest = (alarmLimitsRequestSelect as unknown) as Record<string, Range>;
  const alarmLimitsRequestDraft = (alarmLimitsRequestDraftSelect as unknown) as Record<
    string,
    Range
  >;
  /**
   * Function for updating alarm limits
   *
   * @param {Partial<AlarmLimitsRequest>} data - Alarm ranges which to be updated
   */
  const setAlarmLimitsRequestDraft = (data: Partial<AlarmLimitsRequest>) => {
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, data));
  };

  /**
   * Updating the alarm limit request to redux
   */
  const applyChanges = () => {
    if (parameter === undefined) {
      return;
    }
    const update = {
      [key]: parameter?.alarmLimits,
    };

    dispatch(commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
  };
  const alarmConfig = alarmConfiguration(currentMode);
  const [confirmOpen, setConfirmOpen] = useState(false);
  const [discardOpen, setDiscardOpen] = useState(false);

  /**
   * Closes modal popup
   */
  const handleClose = () => {
    setConfirmOpen(false);
  };

  const handleDiscardClose = () => {
    setDiscardOpen(false);
  };

  /**
   * Closes modal popup & updates the changes
   */
  const handleConfirm = () => {
    setConfirmOpen(false);
    applyChanges();
  };

  const handleDiscardConfirm = () => {
    setDiscardOpen(false);
    if (alarmLimitsRequest !== null) setAlarmLimitsRequestDraft(alarmLimitsRequest);
  };

  const handleConfirmOpen = () => {
    setConfirmOpen(true);
  };

  const handleDiscardOpen = () => {
    setDiscardOpen(true);
  };

  const handleAlarmLimitsRange = (range: Range) => {
    if (parameter) {
      const state = alarmConfig.find((alarm: AlarmConfiguration) => alarm.stateKey === key);
      if (state) parameter.alarmLimits = range;
    }
  };

  /**
   * Calls everytime when `alarmConfig` variable changes
   * Updates the page count
   */
  useEffect(() => {
    setPageCount(Math.ceil(alarmConfig.length / itemsPerPage));
  }, [alarmConfig]);

  /**
   * Triggers an event to reset active UI wrapper of Alarm Controls
   * Resets highlighting border around alarm container when clicked across the page
   * Border is usually added on `ValueClicker` button click
   * This allows to reset rotary reference border when user clic
   */
  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid container direction="column" className={classes.root} onClick={OnClickPage}>
      <Grid container item xs direction="column" className={classes.panel}>
        <Grid container item xs direction="row">
          <Grid container spacing={3} style={{ margin: '-10px -12px' }}>
            {/* Splits Alarms based on page number & itemsPerPage count to show in the page */}
            {alarmConfig.slice((page - 1) * itemsPerPage, page * itemsPerPage).map((alarm) => {
              const key = `${alarm.stateKey}`;
              // setKey(key);
              return (
                <Alarm
                  key={key}
                  label={alarm.label}
                  min={alarm.min || 0}
                  max={alarm.max || 100}
                  stateKey={alarm.stateKey}
                  step={alarm.step || 1}
                  alarmLimits={Range.fromJSON({
                    lower: alarmLimitsRequestDraft[alarm.stateKey].lower,
                    upper: alarmLimitsRequestDraft[alarm.stateKey].upper,
                  })}
                  requestCommitRange={handleAlarmLimitsRange}
                />
              );
            })}
          </Grid>
        </Grid>
        <Grid container item xs>
          {/* <Grid item>
            <Typography variant="h3">Alarms</Typography>
          </Grid> */}
          <Grid container className={classes.paginationContainer}>
            {pageCount > 1 && (
              <Grid item>
                <Pagination
                  count={pageCount}
                  page={page}
                  onChange={handleChange}
                  defaultPage={1}
                  variant="outlined"
                  shape="rounded"
                  size="large"
                />
              </Grid>
            )}
            <Grid item style={{ textAlign: 'right' }}>
              {ventilating ? (
                <Button
                  onClick={handleDiscardOpen}
                  color="primary"
                  variant="contained"
                  className={classes.applyButton}
                  disabled={!alarmLimitsRequestUnsaved}
                >
                  Cancel
                </Button>
              ) : null}
              <Button
                onClick={handleConfirmOpen}
                color="secondary"
                variant="contained"
                className={classes.applyButton}
              >
                {ventilating ? 'Submit' : 'Apply Changes'}
              </Button>
              <ModalPopup
                withAction={true}
                label="Set Alarms"
                open={confirmOpen}
                onClose={handleClose}
                onConfirm={handleConfirm}
              >
                <Grid container alignItems="center">
                  <Grid container alignItems="center" justify="center">
                    <Grid container alignItems="center" className={classes.marginHeader}>
                      <Grid item xs>
                        <Typography variant="h4">Confirm New Changes?</Typography>
                      </Grid>
                    </Grid>
                    <Grid item className={classes.marginContent}>
                      {alarmConfig.map((param) => {
                        if (alarmLimitsRequestDraft !== null) {
                          if (alarmLimitsUnsavedKeys.includes(param.stateKey)) {
                            return (
                              <Typography variant="subtitle1">{`Change ${
                                param.label
                              } alarm limits from [${alarmLimitsRequest[param.stateKey].lower} ${
                                param.units
                              } -
                              ${alarmLimitsRequest[param.stateKey].upper} ${param.units}] to [${
                                alarmLimitsRequestDraft[param.stateKey].lower
                              } ${param.units} -
                                  ${alarmLimitsRequestDraft[param.stateKey].upper} ${
                                param.units
                              }] ?`}</Typography>
                            );
                          }
                        }
                        return <React.Fragment />;
                      })}
                    </Grid>
                    <Grid item className={classes.marginContent} />
                  </Grid>
                </Grid>
              </ModalPopup>
              <ModalPopup
                withAction={true}
                label="Set Alarms"
                open={discardOpen}
                onClose={handleDiscardClose}
                onConfirm={handleDiscardConfirm}
              >
                <DiscardAlarmLimitsContent />
              </ModalPopup>
            </Grid>
          </Grid>
        </Grid>
        {/* Right Container for Storing Alarm Slides */}
        <Grid item xs>
          <ModeBanner bannerType={BannerType.NORMAL} />
        </Grid>
      </Grid>
      {/* Title */}
    </Grid>
  );
};

export default AlarmsPage;
