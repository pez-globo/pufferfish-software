import React, { useEffect } from 'react';
import { Subscription } from 'rxjs';
import { useDispatch } from 'react-redux';
import { makeStyles, Theme, Grid, Tabs, Tab, Button, Typography } from '@material-ui/core';
import CloseIcon from '@material-ui/icons/Close';
import ReplyIcon from '@material-ui/icons/Reply';
import ModalPopup from '../controllers/ModalPopup';
import { getcurrentStateKey, getMultiPopupOpenState, setMultiPopupOpen } from '../app/Service';
import {
  getCycleMeasurementsRR,
  getSensorMeasurementsSpO2,
  getSmoothedFiO2Value,
  getSmoothedFlow,
  roundValue,
} from '../../store/controller/selectors';
import { SetValueContent } from '../controllers/ValueModal';
import { a11yProps, TabPanel } from '../controllers/TabPanel';
import ValueInfo from '../dashboard/containers/ValueInfo';
import { BMIN, LMIN, PERCENT } from '../info/units';
import { AlarmModal } from '../controllers';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import {
  ALARM_LIMITS,
  ALARM_LIMITS_STANDBY,
  PARAMETER_STANDBY,
} from '../../store/controller/types';
import store from '../../store';

interface Data {
  stateKey: string;
  label: string;
  units: string;
  isAlarmEnabled: boolean;
  isSetvalEnabled: boolean;
  committedSetting?: number | null;
  alarmValues: number[];
  setValue: number;
  minValue?: number | null;
  maxValue?: number | null;
  alarmValuesActual: number[];
  setValueActual: number;
}

const useStyles = makeStyles((theme: Theme) => ({
  tabPanelContainer: {
    flexGrow: 1,
    justifyContent: 'space-between',
    alignItems: 'stretch',
    height: '100',
  },
  tabs: {
    width: '100%',
  },
  tab: {
    borderRadius: 8,
    border: `2px solid ${theme.palette.primary.main}`,
    marginRight: '8px',
    zIndex: 1,
    minHeight: 40,
  },
  selectedTab: { color: theme.palette.primary.contrastText },
  tabIndicator: {
    borderRadius: 8,
    border: `2px solid ${theme.palette.primary.main}`,
    zIndex: 0,
    marginBottom: theme.spacing(1),
    minHeight: 40,
    background: theme.palette.primary.main,
  },
  actionButtons: {
    marginBottom: '10px',
  },
  aButtons: {
    background: '#234562',
    color: '#fff',

    '&:hover': {
      background: '#124876',
    },
  },
  closeBtn: {
    position: 'absolute',
    top: '25px',
    right: '25px',
    zIndex: 9999,
    cursor: 'pointer',
    border: '1px solid #ccc',
    borderRadius: '4px',
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
}));

const HFNCControls = (): JSX.Element => {
  return (
    <React.Fragment>
      <Grid
        container
        item
        justify="center"
        alignItems="stretch"
        style={{ borderRight: '2px solid #030e17' }}
      >
        <ValueInfo
          mainContainer={{
            selector: getCycleMeasurementsRR,
            label: 'RR',
            stateKey: 'rr',
            units: BMIN,
          }}
        />
        <ValueInfo
          mainContainer={{
            selector: getSensorMeasurementsSpO2,
            label: 'SpO2',
            stateKey: 'spo2',
            units: PERCENT,
          }}
        />
      </Grid>
      <Grid container item justify="center" alignItems="stretch" direction="column">
        <ValueInfo
          mainContainer={{
            selector: getSmoothedFiO2Value,
            label: 'FiO2',
            stateKey: 'fio2',
            units: PERCENT,
          }}
        />
        <ValueInfo
          mainContainer={{
            selector: getSmoothedFlow,
            label: 'Flow Rate',
            stateKey: 'flow',
            units: LMIN,
          }}
        />
      </Grid>
    </React.Fragment>
  );
};

// TODO: Make a constant file for stateKey Constants
const determineInput = (stateKey: string): Data | null => {
  const createData = (
    label: string,
    stateKey: string,
    units: string,
    isSetvalEnabled: boolean,
    isAlarmEnabled: boolean,
    alarmValuesActual: number[],
    committedSetting?: number | null,
    minValue?: number | null,
    maxValue?: number | null,
  ): Data => {
    return {
      label,
      stateKey,
      units,
      isSetvalEnabled,
      isAlarmEnabled,
      committedSetting,
      minValue,
      maxValue,
      alarmValues: alarmValuesActual,
      alarmValuesActual,
      setValue: committedSetting as number,
      setValueActual: committedSetting as number,
    };
  };

  const getStoreValueData = (stateKey: string): number | null => {
    const storeData = store.getState();
    switch (stateKey) {
      case 'fio2':
        return roundValue(storeData.controller.parameters.fio2);
      case 'flow':
        return roundValue(storeData.controller.parameters.flow);
      default:
    }
    return null;
  };

  const getStoreAlarmData = (stateKey: string): number[] | null => {
    const storeData = store.getState();
    const alarmLimits = storeData.controller.alarmLimitsRequest;
    switch (stateKey) {
      case 'spo2':
        return [alarmLimits.spo2?.lower as number, alarmLimits.spo2?.upper as number];
      case 'rr':
        return [alarmLimits.rr?.lower as number, alarmLimits.rr?.upper as number];
      default:
    }
    return null;
  };

  switch (stateKey) {
    case 'spo2':
      return createData(
        'SpO2',
        stateKey,
        PERCENT,
        false,
        true,
        getStoreAlarmData(stateKey) as number[],
        -1,
      );
    case 'rr':
      return createData(
        'RR',
        stateKey,
        BMIN,
        false,
        true,
        getStoreAlarmData(stateKey) as number[],
        -1,
      );
    case 'fio2':
      return createData(
        'FiO2',
        stateKey,
        PERCENT,
        true,
        false,
        [],
        getStoreValueData(stateKey) as number,
        21,
        null,
      );
    case 'flow':
      return createData(
        'Flow Rate',
        stateKey,
        LMIN,
        true,
        false,
        [],
        getStoreValueData(stateKey) as number,
        null,
        80,
      );
    default:
  }

  return null;
};

const MultiStepWizard = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  const [open, setOpen] = React.useState(false);
  const [confirmOpen, setConfirmOpen] = React.useState(false);
  const [cancelOpen, setCancelOpen] = React.useState(false);
  const [label, setLabel] = React.useState('Ventilation Controls');
  const [stateKey, setStateKey] = React.useState('');
  const [tabIndex, setTabIndex] = React.useState(0);
  const [parameter, setParameter] = React.useState<Data | null>();
  const [multiParams, setMultiParams] = React.useState<Data[]>([]);

  const handleChange = (event: React.ChangeEvent<Record<string, unknown>>, newValue: number) => {
    setTabIndex(newValue);
  };

  useEffect(() => {
    const popupEventSubscription: Subscription = getMultiPopupOpenState().subscribe(
      (state: boolean) => {
        setOpen(state);
      },
    );
    const stateKeyEventSubscription: Subscription = getcurrentStateKey().subscribe(
      (state: string) => {
        setStateKey(state);
        setParameter(determineInput(state));
      },
    );
    return () => {
      if (popupEventSubscription) {
        popupEventSubscription.unsubscribe();
      }
      if (stateKeyEventSubscription) {
        stateKeyEventSubscription.unsubscribe();
      }
    };
  }, []);

  useEffect(() => {
    if (parameter) {
      if (tabIndex > 0) {
        setLabel(parameter.isSetvalEnabled ? 'Set New' : 'Alarms');
      } else {
        setLabel('Ventilation Controls');
      }
    }
  }, [tabIndex, parameter]);

  useEffect(() => {
    if (parameter) {
      setTabIndex(1);
      if (!multiParams.length) {
        setMultiParams([parameter as Data]);
      } else {
        const identifier = multiParams.find((param: Data) => param.stateKey === parameter.stateKey);
        if (!identifier) {
          multiParams.push(parameter);
          setMultiParams(multiParams);
        }
      }
    }
  }, [parameter, multiParams]);

  const doSetValue = (setting: number) => {
    if (parameter) {
      const param = multiParams.find((param: Data) => param.stateKey === parameter.stateKey);
      if (param) param.setValue = setting;
      parameter.setValue = setting;
    }
  };

  const doSetAlarmValues = (min: number, max: number) => {
    if (parameter) {
      const param = multiParams.find((param: Data) => param.stateKey === parameter.stateKey);
      if (param) param.alarmValues = [min, max];
      parameter.alarmValues = [min, max];
    }
  };

  const getAlarmValues = (stateKey: string) => {
    const param = multiParams.find((param: Data) => param.stateKey === stateKey);
    if (param) {
      if (param.alarmValues.length) return param.alarmValues;
    }
    return [];
  };

  const getSetValues = (stateKey: string) => {
    const param = multiParams.find((param: Data) => param.stateKey === stateKey);
    if (param) {
      if (param.setValue) return param.setValue as number;
    }
    if (parameter) return parameter.committedSetting as number;
    return 0;
  };

  const isAnyChanges = () => {
    let anyChange = false;
    multiParams.forEach((param: Data) => {
      if (param.isSetvalEnabled) {
        if (param.setValue !== param.setValueActual) {
          anyChange = true;
        }
      } else if (
        param.alarmValues.length &&
        (param.alarmValues[0] !== param.alarmValuesActual[0] ||
          param.alarmValues[1] !== param.alarmValuesActual[1])
      ) {
        anyChange = true;
      }
    });
    return anyChange;
  };

  const onCancel = () => {
    if (isAnyChanges()) {
      setCancelOpen(true);
    }
    setMultiPopupOpen(false);
  };

  const onConfirm = () => {
    if (isAnyChanges()) {
      setConfirmOpen(true);
    }
    setMultiPopupOpen(false);
  };

  const handleConfirm = () => {
    multiParams.forEach((parameter: Data) => {
      if (parameter.isSetvalEnabled) {
        dispatch(updateCommittedParameter({ [parameter.stateKey]: parameter.setValue }));
        dispatch(
          updateCommittedState(PARAMETER_STANDBY, { [parameter.stateKey]: parameter.setValue }),
        );
      }
      if (parameter.isAlarmEnabled && parameter.alarmValues.length) {
        dispatch(
          updateCommittedState(ALARM_LIMITS, {
            [parameter.stateKey]: {
              lower: parameter.alarmValues[0],
              upper: parameter.alarmValues[1],
            },
          }),
        );
        dispatch(
          updateCommittedState(ALARM_LIMITS_STANDBY, {
            [parameter.stateKey]: {
              lower: parameter.alarmValues[0],
              upper: parameter.alarmValues[1],
            },
          }),
        );
      }
    });
    setMultiParams([]);
    setConfirmOpen(false);
  };

  const handleCancelConfirm = () => {
    setMultiParams([]);
    setCancelOpen(false);
  };

  return (
    <React.Fragment>
      <ModalPopup withAction={false} label={label} open={open}>
        {tabIndex === 0 ? (
          <CloseIcon onClick={() => setMultiPopupOpen(false)} className={classes.closeBtn} />
        ) : (
          <ReplyIcon onClick={() => setTabIndex(tabIndex - 1)} className={classes.closeBtn} />
        )}
        <Grid container item>
          <Tabs
            value={tabIndex}
            onChange={handleChange}
            classes={{ indicator: classes.tabIndicator }}
          >
            <Tab
              label="HFNC Control"
              {...a11yProps(0)}
              className={classes.tab}
              classes={{ selected: classes.selectedTab }}
            />
            <Tab
              style={{ visibility: tabIndex === 0 ? 'hidden' : 'visible' }}
              label={parameter?.isSetvalEnabled ? 'Set New' : 'Alarms'}
              {...a11yProps(1)}
              className={classes.tab}
              classes={{ selected: classes.selectedTab }}
            />
          </Tabs>
        </Grid>
        <Grid container>
          <TabPanel value={tabIndex} index={0}>
            <HFNCControls />
          </TabPanel>
          <TabPanel value={tabIndex} index={1}>
            {parameter?.isSetvalEnabled ? (
              <SetValueContent
                openModal={open}
                committedSetting={getSetValues(stateKey)}
                label={parameter.label}
                units={parameter.units}
                requestCommitSetting={doSetValue}
                {...(parameter.minValue && { min: parameter.minValue })}
                {...(parameter.maxValue && { max: parameter.maxValue })}
              />
            ) : (
              <AlarmModal
                openModal={open}
                label={parameter?.label || ''}
                units={parameter?.units || ''}
                stateKey={stateKey}
                requestCommitRange={doSetAlarmValues}
                contentOnly={true}
                labelHeading={true}
                alarmRangeValues={getAlarmValues(stateKey)}
              />
            )}
          </TabPanel>
        </Grid>

        <Grid
          container
          item
          alignItems="center"
          justify="flex-end"
          className={classes.actionButtons}
        >
          <Grid item>
            <Button
              variant="contained"
              className={classes.aButtons}
              style={{ marginRight: '15px' }}
              onClick={onCancel}
            >
              Cancel
            </Button>
          </Grid>
          <Grid item>
            <Button variant="contained" className={classes.aButtons} onClick={onConfirm}>
              Submit
            </Button>
          </Grid>
        </Grid>
      </ModalPopup>
      <ModalPopup
        withAction={true}
        maxWidth="xs"
        label=""
        open={confirmOpen}
        onClose={() => setConfirmOpen(false)}
        onConfirm={handleConfirm}
      >
        <Grid container alignItems="center" className={classes.marginHeader}>
          <Grid item xs>
            <Typography variant="h4">Confirm New Changes?</Typography>
          </Grid>
        </Grid>
        <Grid item alignItems="center" className={classes.marginContent}>
          {multiParams.map((param: Data) => {
            if (param.isSetvalEnabled) {
              if (param.setValue !== param.setValueActual) {
                return (
                  <Typography variant="subtitle1">{`Change ${param.label} to ${param.setValue} ${param.units}?`}</Typography>
                );
              }
            } else if (
              param.alarmValues.length &&
              (param.alarmValues[0] !== param.alarmValuesActual[0] ||
                param.alarmValues[1] !== param.alarmValuesActual[1])
            ) {
              return (
                <Typography variant="subtitle1">{`Change ${param.label} alarm range to ${param.alarmValues[0]} - ${param.alarmValues[1]}?`}</Typography>
              );
            }
            return <React.Fragment />;
          })}
        </Grid>
      </ModalPopup>
      <ModalPopup
        withAction={true}
        maxWidth="xs"
        label=""
        open={cancelOpen}
        onClose={() => setCancelOpen(false)}
        onConfirm={handleCancelConfirm}
      >
        <Grid container alignItems="center" className={classes.marginHeader}>
          <Grid item xs>
            <Typography variant="h4">Keep Previous Values?</Typography>
          </Grid>
        </Grid>
        <Grid item alignItems="center" className={classes.marginContent}>
          {multiParams.map((param: Data) => {
            if (param.isSetvalEnabled) {
              if (param.setValue !== param.setValueActual) {
                return (
                  <Typography variant="subtitle1">{`Keep ${param.label} to ${param.setValueActual} ${param.units}?`}</Typography>
                );
              }
            } else if (
              param.alarmValues.length &&
              (param.alarmValues[0] !== param.alarmValuesActual[0] ||
                param.alarmValues[1] !== param.alarmValuesActual[1])
            ) {
              return (
                <Typography variant="subtitle1">{`Keep ${param.label} alarm range to ${param.alarmValuesActual[0]} - ${param.alarmValuesActual[1]}?`}</Typography>
              );
            }
            return <React.Fragment />;
          })}
        </Grid>
      </ModalPopup>
    </React.Fragment>
  );
};

export default MultiStepWizard;
