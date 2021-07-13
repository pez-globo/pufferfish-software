/**
 * @summary Component handling Multi step Modal popup
 *
 * @file Modal popup has steps to update Set Value & Alarm Range values
 */
import React, { useEffect } from 'react';
import { Subscription } from 'rxjs';
import { useDispatch } from 'react-redux';
import { makeStyles, Theme, Grid, Tabs, Tab, Button, Typography } from '@material-ui/core';
import ReplyIcon from '@material-ui/icons/Reply';
// import ArrowForwardIosIcon from '@material-ui/icons/ArrowForwardIos';
import ModalPopup from '../controllers/ModalPopup';
import { getcurrentStateKey, getMultiPopupOpenState, setMultiPopupOpen } from '../app/Service';
import {
  getSmoothedSpO2,
  getSmoothedHR,
  roundValue,
  getParametersRequestDraftFiO2,
  getParametersRequestDraftFlow,
} from '../../store/controller/selectors';
import { a11yProps, TabPanel } from '../controllers/TabPanel';
import ValueInfo from '../dashboard/components/ValueInfo';
import { BPM, LMIN, PERCENT } from '../info/units';
import { AlarmModal, ValueModal } from '../controllers';
import { ParametersRequest, AlarmLimitsRequest } from '../../store/controller/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import store from '../../store';

/**
 * @typedef Data
 *
 * Interface for Data
 *
 * @prop {string} stateKey Unique identifier for the value
 * @prop {string} label Label for the value
 * @prop {string} units Unit measurement of the value
 * @prop {boolean} isAlarmEnabled Config to set if value is Alarm type
 * @prop {boolean} isSetvalEnabled Config to set if value is Set type
 * @prop {number | null} committedSetting Current value
 * @prop {number[]} alarmValues Alarm range values
 * @prop {number} setValue Set Value
 * @prop {number | null} minValue Minimum under which value cannot decrement
 * @prop {number | null} maxValue Maximum above which value cannot increment
 * @prop {number | null} alarmLimitMin Alarm limit Minimum Range value
 * @prop {number | null} alarmLimitMax Alarm limit Maximum Range value
 * @prop {number[]} alarmValuesActual Actual Alarm Value when Component was initalized
 * @prop {number} setValueActual Actual Set Value when Component was initalized
 */
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
  alarmLimitMin?: number | null;
  alarmLimitMax?: number | null;
  alarmValuesActual: number[];
  setValueActual: number;
}

interface HFNCProps {
  alarmValuesSpO2: number[];
  alarmValuesHR: number[];
  alarmValuesFiO2: number[];
  alarmValuesFlow: number[];
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
    position: 'absolute',
    bottom: '15px',
    right: '15px',
  },
  aButtons: {
    color: '#fff',
  },
  closeBtn: {
    minWidth: '38px',
    marginRight: theme.spacing(1),
    borderRadius: '8px',
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
  tabAligning: {
    maxWidth: '60%',
    margin: '0 auto',
    marginTop: '10px',
    '& .MuiTabs-root': {
      '& .MuiTabs-scroller': {
        '& .MuiTabs-flexContainer': {
          alignItems: 'center',
        },
      },
    },
  },
}));

/**
 * HFNCControls
 *
 * @component A container for displaying the HFNC controls.
 *
 * @returns {JSX.Element}
 */
const HFNCControls = ({
  alarmValuesSpO2,
  alarmValuesHR,
  alarmValuesFiO2,
  alarmValuesFlow,
}: HFNCProps): JSX.Element => {
  return (
    <React.Fragment>
      <Grid
        container
        item
        justify="center"
        alignItems="stretch"
        direction="column"
        style={{ borderRight: '2px solid #030e17' }}
      >
        <ValueInfo
          selector={getSmoothedSpO2}
          label="SpO2"
          stateKey="spo2"
          units={PERCENT}
          alarmLimits={alarmValuesSpO2}
          showLimits
        />
        <ValueInfo
          selector={getSmoothedHR}
          label="HR"
          stateKey="hr"
          units={BPM}
          alarmLimits={alarmValuesHR}
          showLimits
        />
      </Grid>
      <Grid container item justify="center" alignItems="stretch" direction="column">
        <ValueInfo
          selector={getParametersRequestDraftFiO2}
          label="FiO2"
          stateKey="fio2"
          units={PERCENT}
          alarmLimits={alarmValuesFiO2}
          showLimits
        />
        <ValueInfo
          selector={getParametersRequestDraftFlow}
          label="Flow"
          stateKey="flow"
          units={LMIN}
          alarmLimits={alarmValuesFlow}
          showLimits
        />
      </Grid>
    </React.Fragment>
  );
};

/**
 * Function to return object with configured values
 *
 * @param {string} stateKey Unique identifier for the value
 * @param {string} label Label for the value
 * @param {string} units Unit measurement of the value
 * @param {boolean} isAlarmEnabled Config to set if value is Alarm type
 * @param {boolean} isSetvalEnabled Config to set if value is Set type
 * @param {number[]} alarmValuesActual - Current Alarm values from redux store
 * @param {number | null} committedSetting - Actual Alarm Value when Component was initalized
 * @param {number | null} minValue Minimum under which value cannot decrement
 * @param {number | null} maxValue Maximum above which value cannot increment
 * @param {number | null} alarmLimitMin Alarm limit Minimum Range value
 * @param {number | null} alarmLimitMax Alarm limit Maximum Range value
 *
 * @returns {Data} - returns the `Data` value
 */
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
  alarmLimitMin?: number | null,
  alarmLimitMax?: number | null,
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
    alarmLimitMin,
    alarmLimitMax,
    alarmValues: alarmValuesActual,
    alarmValuesActual,
    setValue: committedSetting as number,
    setValueActual: committedSetting as number,
  };
};

/**
 * Function to get value from redux store for parameters
 *
 * @param {string} stateKey - Unique identifier for the value
 *
 * @returns {number | null} - Value from redux store
 */
const getStoreValueData = (stateKey: string): number | null => {
  // TODO: is there a reason this function directly accesses the store, rather than
  // using the getParametersFiO2 and getParametersFlow2 selectors?
  // Yes, since its non React function, it does not have access to Hooks
  // So it directly accesses from the store
  const storeData = store.getState();
  if (storeData.controller.parameters.current === null) {
    return null;
  }

  switch (stateKey) {
    case 'fio2':
      return roundValue(storeData.controller.parameters.current.fio2);
    case 'flow':
      return roundValue(storeData.controller.parameters.current.flow);
    default:
      return null;
  }
};

/**
 * Function to get value from redux store for Alarms
 *
 * @param {string} stateKey - Unique identifier for the value
 *
 * @returns {number | null} - Value from redux store
 */
const getStoreAlarmData = (stateKey: string): number[] | null => {
  // TODO: is there a reason this function directly access the store, rather than
  // using the getAlarmLimitsRequest selector?
  // Yes, since its non React function, it does not have access to Hooks
  // So it directly accesses from the store
  const storeData = store.getState();
  const alarmLimits = storeData.controller.alarmLimits.request;
  if (alarmLimits === null) {
    return null;
  }

  switch (stateKey) {
    case 'spo2':
      return [alarmLimits.spo2?.lower as number, alarmLimits.spo2?.upper as number];
    case 'hr':
      return [alarmLimits.hr?.lower as number, alarmLimits.hr?.upper as number];
    case 'fio2':
      return [alarmLimits.fio2?.lower as number, alarmLimits.fio2?.upper as number];
    case 'flow':
      return [alarmLimits.flow?.lower as number, alarmLimits.flow?.upper as number];
    default:
      return null;
  }
};

/**
 * Function to frame `Data` object based on `stateKey` provided
 *
 * TODO: Make a constant file for stateKey Constants
 *
 * @param {string} stateKey - Unique identifier for the value
 *
 * @returns {Data | undefined} - `Data` object containing configurations
 */
const determineInput = (stateKey: string): Data | undefined => {
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
    case 'hr':
      return createData(
        'HR',
        stateKey,
        BPM,
        false,
        true,
        getStoreAlarmData(stateKey) as number[],
        -1,
        null,
        null,
        0,
        200,
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
  return undefined;
};

/**
 * MultiStepWizard
 *
 * @component A container for displaying the multi-step wizard.
 *
 * @returns {JSX.Element}
 */
const MultiStepWizard = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  /**
   * State to manage Multi step Modal `open` status
   */
  const [open, setOpen] = React.useState(false);
  /**
   * State to manage Confirmation Modal `open` status
   */
  const [confirmOpen, setConfirmOpen] = React.useState(false);
  /**
   * State to manage Cancellation Modal `open` status
   */
  const [cancelOpen, setCancelOpen] = React.useState(false);
  /**
   * State to manage dynamic label displayed on each step
   */
  const [label, setLabel] = React.useState('Ventilation Controls');
  /**
   * State to manage current `stateKey`
   */
  const [stateKey, setStateKey] = React.useState('');
  /**
   * State to manage Tab index
   */
  const [tabIndex, setTabIndex] = React.useState(0);
  /**
   * State to manage Parameter `Data`
   */
  const [parameter, setParameter] = React.useState<Data>();
  /**
   * State to manage all Parameter `Data`
   */
  const [multiParams, setMultiParams] = React.useState<Data[]>([]);
  /**
   * State to manage if button submitted is disabled
   */
  const [isSubmitDisabled, setIsSubmitDisabled] = React.useState(false);

  /**
   * Trigger on Tab index change event
   *
   * @param {React.ChangeEvent<Record<string, unknown>>} event - Default Change event object
   * @param {number} newValue - Tab index value to update current tab
   *
   */
  const handleChange = (event: React.ChangeEvent<Record<string, unknown>>, newValue: number) => {
    setTabIndex(newValue);
  };

  /**
   * Triggers on Component Initalization
   * Listens to 2 events `isMultiPopupOpen` & `currentStateKey`
   * On `isMultiPopupOpen` open event, multistep popup is displayed in UI
   * Current stateKey decides which Tab to be shown once open
   * For example if user click on `FiO2` Set Value container in Dashboard,
   * Then current stateKey would be `FiO2` and multistep popup will show Set Value modal
   */
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
      setConfirmOpen(false);
      setCancelOpen(false);
    };
  }, []);

  /**
   * Triggers on TabIndex or Parameter change
   * Updates the label based on Parameter type configured
   */
  useEffect(() => {
    if (parameter) {
      if (tabIndex > 0) {
        setLabel(
          parameter.isSetvalEnabled ? `${parameter.label} Settings` : `${parameter.label} Alarms`,
        );
      } else {
        setLabel('Ventilation Controls');
      }
    }
  }, [tabIndex, parameter]);

  /**
   * Triggers on parameter or multiParams change
   * Initally multiParams has only 1 parameter which is set in Component Initalization
   * As user moves around the tabs and clicks on various Set/Alarm container
   * Then those are added into multiParams array
   */
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

  /**
   * Callback on whenever Set Value of parameter changes
   *
   * @param {number} setting - Updated value of parameter
   *
   * Updates the new Set value into multiParams `Data` object
   */
  const doSetValue = (setting: number) => {
    if (parameter) {
      const param = multiParams.find((param: Data) => param.stateKey === parameter.stateKey);
      if (param) param.setValue = setting;
      parameter.setValue = setting;
      if (open) {
        const update = { [stateKey]: setting };
        dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
      }
      if (isAnyChanges()) {
        setIsSubmitDisabled(false);
      } else {
        setIsSubmitDisabled(true);
      }
    }
  };

  /**
   * Callback on whenever Alarm range value of parameter changes
   *
   * @param {number} min - Updated lower limit for the alarm value
   * @param {number} max - Updated upper limit for the alarm value
   *
   * Updates the new alarm range values into multiParams `Data` object
   */
  const doSetAlarmValues = (min: number, max: number) => {
    if (parameter) {
      const param = multiParams.find((param: Data) => param.stateKey === parameter.stateKey);
      if (param) param.alarmValues = [min, max];
      parameter.alarmValues = [min, max];
      if (isAnyChanges()) {
        setIsSubmitDisabled(false);
      } else {
        setIsSubmitDisabled(true);
      }
    }
  };

  /**
   * Function to get Alarm values from Parameter `Data` object
   *
   * @param {string} stateKey - stateKey identifier for which parameter `Data` corresponds to
   *
   * @return {Array} - Current alarm range values
   *
   */
  const getAlarmValues = (stateKey: string) => {
    const param = multiParams.find((param: Data) => param.stateKey === stateKey);
    if (param) {
      if (param.alarmValues.length) return param.alarmValues;
    }
    return [];
  };

  /**
   * Function to get Set values from Parameter `Data` object
   *
   * @param {string} stateKey - stateKey identifier for which parameter `Data` corresponds to
   *
   * @return {number} - Current Set Value
   *
   */
  const getSetValues = (stateKey: string) => {
    const param = multiParams.find((param: Data) => param.stateKey === stateKey);
    if (param && param.setValue) {
      return param.setValue as number;
    }
    if (parameter) return parameter.committedSetting as number;
    return 0;
  };

  /**
   * Checks if there are any changes user has made
   *
   * @return {boolean} - true if change is there; false if there are no changes
   *
   */
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

  /**
   * Callback on clicking of cancel button
   * Opens Cancel popup
   */
  const onCancel = () => {
    if (isAnyChanges()) {
      setCancelOpen(true);
    } else {
      setMultiPopupOpen(false, stateKey);
    }
  };

  /**
   * Callback on clicking of confirm button
   * Opens Confirmation popup
   */
  const onConfirm = () => {
    if (isAnyChanges()) {
      setConfirmOpen(true);
    } else {
      setMultiPopupOpen(false, stateKey);
    }
  };

  /**
   * Callback on clicking Confirm button on Confirmation popup
   * Updates multiParams data into redux store
   * Closes all te popup
   */
  const handleConfirm = () => {
    multiParams.forEach((parameter: Data) => {
      if (parameter.isSetvalEnabled) {
        const update = { [parameter.stateKey]: parameter.setValue };
        dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
        dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
      }
      if (parameter.isAlarmEnabled && parameter.alarmValues.length) {
        const update = {
          [parameter.stateKey]: {
            lower: parameter.alarmValues[0],
            upper: parameter.alarmValues[1],
          },
        };
        dispatch(commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
        dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
      }
    });
    setMultiParams([]);
    setConfirmOpen(false);
    setMultiPopupOpen(false);
  };

  /**
   * Callback on clicking Confirm button on Cancel popup
   * Undo all the user made changes
   */
  const handleCancelConfirm = () => {
    if (parameter) {
      parameter.setValue = parameter.setValueActual;
      parameter.alarmValues = parameter.alarmValuesActual;
    }
    multiParams.map((parameter: Data) => {
      const param = parameter;
      param.setValue = param.setValueActual;
      param.alarmValues = param.alarmValuesActual;
      return param;
    });
    setMultiParams([]);
    setCancelOpen(false);
    setMultiPopupOpen(false);
  };

  /**
   * Callback on clicking Cancel button on Confirmation popup
   * Navigates back to MultiStep popup
   */
  const handleCancelOnConfirmPopup = () => {
    setConfirmOpen(false);
    // setMultiPopupOpen(true, stateKey);
  };

  /**
   * Callback on clicking Cancel button on Cancel popup
   * Navigates back to MultiStep popup
   */
  const handleCancelOnCancelPopup = () => {
    setCancelOpen(false);
    // setMultiPopupOpen(true, stateKey);s
  };

  return (
    <React.Fragment>
      <ModalPopup
        withAction={false}
        label={label}
        open={open}
        showCloseIcon={true}
        onClose={onCancel}
      >
        <Grid style={{ flexGrow: 1 }}>
          <Grid container item className={classes.tabAligning}>
            <Grid style={{ minHeight: 38, minWidth: 38 }}>
              <Button
                onClick={() => setTabIndex(tabIndex - 1)}
                variant="contained"
                color="primary"
                disabled={!(tabIndex > 0)}
                className={classes.closeBtn}
              >
                <ReplyIcon />
              </Button>
            </Grid>
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
              {/* <Tab
                label={<ArrowForwardIosIcon />}
                {...a11yProps(98)}
                className={classes.nonTab}
              /> */}
              <Tab
                style={{ visibility: tabIndex === 0 ? 'hidden' : 'visible' }}
                label={
                  parameter?.isSetvalEnabled
                    ? `${parameter.label} Settings`
                    : `${parameter?.label} Alarms`
                }
                {...a11yProps(1)}
                className={classes.tab}
                classes={{ selected: classes.selectedTab }}
              />
            </Tabs>
          </Grid>
          <Grid container className={classes.tabAligning}>
            <TabPanel value={tabIndex} index={0}>
              {/* TODO: use selectors instead of local state */}
              <HFNCControls
                alarmValuesSpO2={getAlarmValues('spo2')}
                alarmValuesHR={getAlarmValues('hr')}
                alarmValuesFiO2={getAlarmValues('fio2')}
                alarmValuesFlow={getAlarmValues('flow')}
              />
            </TabPanel>
            <TabPanel value={tabIndex} index={1}>
              {parameter && parameter.isSetvalEnabled ? (
                <ValueModal
                  openModal={open && parameter.stateKey === stateKey}
                  key={parameter.stateKey}
                  committedSetting={getSetValues(parameter.stateKey)}
                  label={parameter.label}
                  units={parameter.units}
                  requestCommitSetting={doSetValue}
                  {...(parameter.minValue && { min: parameter.minValue })}
                  {...(parameter.maxValue && { max: parameter.maxValue })}
                />
              ) : (
                parameter && (
                  <AlarmModal
                    openModal={open && parameter.stateKey === stateKey}
                    label={parameter.label}
                    units={parameter.units}
                    stateKey={parameter.stateKey}
                    getCommittedRange={doSetAlarmValues}
                    contentOnly={true}
                    labelHeading={true}
                    alarmRangeValues={getAlarmValues(parameter.stateKey)}
                    {...(parameter.alarmLimitMin && { committedMin: parameter.alarmLimitMin })}
                    {...(parameter.alarmLimitMax && { committedMax: parameter.alarmLimitMax })}
                  />
                )
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
                color="primary"
              >
                Cancel
              </Button>
            </Grid>
            <Grid item>
              <Button
                variant="contained"
                disabled={isSubmitDisabled}
                className={classes.aButtons}
                onClick={onConfirm}
                color="secondary"
                style={{ color: '#000' }}
              >
                Submit
              </Button>
            </Grid>
          </Grid>
        </Grid>
      </ModalPopup>
      <ModalPopup
        withAction={true}
        maxWidth="xs"
        label=""
        open={confirmOpen}
        onClose={handleCancelOnConfirmPopup}
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
          </Grid>
        </Grid>
      </ModalPopup>
      <ModalPopup
        withAction={true}
        maxWidth="xs"
        label=""
        open={cancelOpen}
        onClose={handleCancelOnCancelPopup}
        onConfirm={handleCancelConfirm}
      >
        <Grid container alignItems="center">
          <Grid container alignItems="center" justify="center">
            <Grid container alignItems="center" className={classes.marginHeader}>
              <Grid item xs>
                <Typography variant="h4">Keep Previous Values?</Typography>
              </Grid>
            </Grid>
            <Grid item className={classes.marginContent}>
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
          </Grid>
        </Grid>
      </ModalPopup>
    </React.Fragment>
  );
};

export default MultiStepWizard;
