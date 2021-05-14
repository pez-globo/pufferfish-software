import { Button, Grid, Typography } from '@material-ui/core';
import { makeStyles, Theme, useTheme } from '@material-ui/core/styles';
import Pagination from '@material-ui/lab/Pagination';
import React, { RefObject, useEffect, useRef, useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import { AlarmLimitsRequest, VentilationMode, Range } from '../../store/controller/proto/mcu_pb';
import {
  getAlarmLimitsCurrent,
  getAlarmLimitsRequestDraft,
  getAlarmLimitsUnsavedChanges,
  getParametersIsVentilating,
  getParametersRequestMode,
} from '../../store/controller/selectors';
import { MessageType } from '../../store/controller/types';
import { setActiveRotaryReference } from '../app/Service';
import { ValueClicker } from '../controllers';
import ModalPopup from '../controllers/ModalPopup';
import ValueSlider from '../controllers/ValueSlider';
import ModeBanner from '../displays/ModeBanner';
import useRotaryReference from '../utils/useRotaryReference';

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

const Alarm = ({
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
  const { initRefListener } = useRotaryReference(theme);
  // TODO: when the software is in ventilating mode, the user must be able to
  // discard changes (which means that any alarm limits being persisted in
  // AlarmLimitsRequestStandby would need to be reset using the values from
  // AlarmLimitsRequest, when the user wants to discard values). This could
  // be done with by dispatching a commitDraftRequest action with the
  // alarmLimitsRequest selector as the update field.
  const range =
    alarmLimits === null
      ? undefined
      : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
  const rangeValues: number[] = [
    range === undefined ? NaN : range.lower,
    range === undefined ? NaN : range.upper,
  ];
  const [refs] = React.useState<Record<string, RefObject<HTMLDivElement>>>({
    [`${stateKey}_LOWER`]: useRef(null),
    [`${stateKey}_HIGHER`]: useRef(null),
  });
  const setRangevalue = (range: number[]) => {
    setAlarmLimits({ [stateKey]: { lower: range[0], upper: range[1] } });
  };
  const onClick = (value: number, type: SliderType) => {
    setActiveRotaryReference(
      type === SliderType.LOWER ? `${stateKey}_LOWER` : `${stateKey}_HIGHER`,
    );
    setAlarmLimits({
      [stateKey]: {
        lower: type === SliderType.LOWER ? value : rangeValues[0],
        upper: type === SliderType.UPPER ? value : rangeValues[1],
      },
    });
  };

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
                max={max}
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
                min={min}
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

interface AlarmConfiguration {
  label: string;
  min?: number;
  max?: number;
  stateKey: string;
  step?: number;
}

const alarmConfiguration = (ventilationMode: VentilationMode | null): Array<AlarmConfiguration> => {
  switch (ventilationMode) {
    case VentilationMode.hfnc:
      return [
        { label: 'SpO2', stateKey: 'spo2' },
        { label: 'HR', stateKey: 'hr', max: 200 },
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
 * AlarmsPage
 *
 * A container for housing all alarm configurations.
 */
export const AlarmsPage = (): JSX.Element => {
  const classes = useStyles();
  const itemsPerPage = 5;
  const [page, setPage] = React.useState(1);
  const [pageCount, setPageCount] = React.useState(1);
  const handleChange = (event: React.ChangeEvent<unknown>, value: number) => {
    setPage(value);
  };

  const alarmLimitsRequestDraft = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsCurrent = useSelector(getAlarmLimitsCurrent);
  const dispatch = useDispatch();
  const currentMode = useSelector(getParametersRequestMode);
  const ventilating = useSelector(getParametersIsVentilating);
  const alarmLimitsUnsaved = useSelector(getAlarmLimitsUnsavedChanges);
  const alarmLimits = (alarmLimitsCurrent as unknown) as Record<string, Range>;
  const alarmLimitsDraft = (alarmLimitsRequestDraft as unknown) as Record<string, Range>;
  const setAlarmLimitsRequestDraft = (data: Partial<AlarmLimitsRequest>) => {
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, data));
  };
  const applyChanges = () => {
    if (alarmLimitsRequestDraft === null) {
      return;
    }

    dispatch(
      commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, alarmLimitsRequestDraft),
    );
  };
  const alarmConfig = alarmConfiguration(currentMode);
  const [confirmOpen, setConfirmOpen] = useState(false);
  const [discardOpen, setDiscardOpen] = useState(false);
  const [isDisabled, setIsDisabled] = useState(!alarmLimitsUnsaved);

  const handleClose = () => {
    setConfirmOpen(false);
  };

  const handleDiscardClose = () => {
    setDiscardOpen(false);
  };

  const handleConfirm = () => {
    setConfirmOpen(false);
    applyChanges();
    setIsDisabled(true);
  };

  const handleDiscardConfirm = () => {
    setDiscardOpen(false);
    setAlarmLimitsRequestDraft(alarmLimits);
    setIsDisabled(true);
  };

  const handleConfirmOpen = () => {
    setConfirmOpen(true);
  };

  const handleDiscardOpen = () => {
    setDiscardOpen(true);
  };

  useEffect(() => {
    setPageCount(Math.ceil(alarmConfig.length / itemsPerPage));
  }, [alarmConfig]);

  useEffect(() => {
    if (alarmLimitsUnsaved) {
      setIsDisabled(false);
    } else {
      setIsDisabled(true);
    }
  }, [alarmLimitsUnsaved]);

  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid container direction="column" className={classes.root} onClick={OnClickPage}>
      <Grid container item xs direction="column" className={classes.panel}>
        <Grid container item xs direction="row">
          <Grid container spacing={3} style={{ margin: '-10px -12px' }}>
            {alarmConfig.slice((page - 1) * itemsPerPage, page * itemsPerPage).map((alarm) => {
              const key = `alarm-config-${alarm.stateKey}`;
              return (
                <Alarm
                  key={key}
                  label={alarm.label}
                  min={alarm.min || 0}
                  max={alarm.max || 100}
                  stateKey={alarm.stateKey}
                  step={alarm.step || 1}
                  alarmLimits={alarmLimitsRequestDraft}
                  setAlarmLimits={setAlarmLimitsRequestDraft}
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
                  disabled={isDisabled}
                >
                  Cancel
                </Button>
              ) : null}
              {ventilating ? (
                <Button
                  onClick={handleConfirmOpen}
                  color="secondary"
                  variant="contained"
                  className={classes.applyButton}
                  disabled={isDisabled}
                >
                  Submit
                </Button>
              ) : null}
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
                        if (
                          alarmLimits[param.stateKey].lower !==
                            alarmLimitsDraft[param.stateKey]?.lower ||
                          alarmLimits[param.stateKey].upper !==
                            alarmLimitsDraft[param.stateKey]?.upper
                        ) {
                          return (
                            <Typography variant="subtitle1">{`Change ${
                              param.label
                            } alarm range to ${alarmLimitsDraft[param.stateKey].lower} -
                                ${alarmLimitsDraft[param.stateKey].upper}?`}</Typography>
                          );
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
                <Grid container alignItems="center">
                  <Grid container alignItems="center" justify="center">
                    <Grid container alignItems="center" className={classes.marginHeader}>
                      <Grid item xs>
                        <Typography variant="h4">Keep Previous Values?</Typography>
                      </Grid>
                    </Grid>
                    <Grid item className={classes.marginContent}>
                      {alarmConfig.map((param) => {
                        if (
                          alarmLimits[param.stateKey].lower !==
                            alarmLimitsDraft[param.stateKey]?.lower ||
                          alarmLimits[param.stateKey].upper !==
                            alarmLimitsDraft[param.stateKey]?.upper
                        ) {
                          return (
                            <Typography variant="subtitle1">{`Keep ${param.label} alarm range to ${
                              alarmLimits[param.stateKey].lower
                            } -
                                ${alarmLimits[param.stateKey].upper}?`}</Typography>
                          );
                        }
                        return <React.Fragment />;
                      })}
                    </Grid>
                    <Grid item className={classes.marginContent} />
                  </Grid>
                </Grid>
              </ModalPopup>
            </Grid>
          </Grid>
        </Grid>
        {/* Right Container for Storing Alarm Slides */}
        <Grid item xs>
          <ModeBanner bannerType="normal" />
        </Grid>
      </Grid>
      {/* Title */}
    </Grid>
  );
};

export default AlarmsPage;
