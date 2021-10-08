/**
 * @summary Component handling Multi step Modal popup
 *
 * @file Modal popup has steps to update Set Value & Alarm Range values
 */
import React, { useCallback, useEffect } from 'react';
import { Subscription } from 'rxjs';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { makeStyles, Theme, Grid, Tabs, Tab, Button, Typography } from '@material-ui/core';
import ReplyIcon from '@material-ui/icons/Reply';
// import ArrowForwardIosIcon from '@material-ui/icons/ArrowForwardIos';
import { createSelector } from 'reselect';
import ModalPopup from '../shared/ModalPopup';
import { getcurrentStateKey, getMultiPopupOpenState, setMultiPopupOpen } from '../app/Service';
import {
  getSpO2AlarmLimitsRequest,
  getHRAlarmLimitsRequest,
} from '../../store/controller/selectors';
import {
  getParametersRequestDraftFiO2,
  getParametersRequestDraftFlow,
  getParametersFlow,
  getParametersFiO2,
} from '../../store/controller/selectors/measurements';
import {
  getSmoothedSpO2,
  getSmoothedHR,
} from '../../store/controller/selectors/derived_measurements';
import { a11yProps, TabPanel } from '../shared/TabPanel';
import ValueInfo from '../dashboard/components/ValueInfo';
import { BPM, LMIN, PERCENT } from '../shared/units';
import { ParametersRequest, AlarmLimitsRequest, Range } from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/proto/types';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import ValueModalContent from '../shared/value/ValueModal';
import { AlarmModalContent } from '../alarms/modal/AlarmModal';
import { SelectorType } from '../shared/value/ValueSelectorDisplay';

/**
 * @typedef InternalState
 *
 * Interface for InternalState
 *
 * @prop {string} stateKey Unique identifier for the value
 * @prop {string} label Label for the value
 * @prop {string} units Unit measurement of the value
 * @prop {boolean} isAlarmLimitsEnabled Config to set if value is Alarm type
 * @prop {boolean} isParamEnabled Config to set if value is Set type
 * @prop {number | null} committedParam Current value
 * @prop {Range | null} alarmLimitsDraft Alarm range values
 * @prop {number} paramDraft Set Value
 * @prop {number | null} minValue Minimum under which value cannot decrement
 * @prop {number | null} maxValue Maximum above which value cannot increment
 * @prop {number | null} alarmLimitMin Alarm limit Minimum Range value
 * @prop {number | null} alarmLimitMax Alarm limit Maximum Range value
 * @prop {Range | null} alarmLimitsCurrent Actual Alarm Value when Component was initalized
 * @prop {number} paramCurrent Actual Set Value when Component was initalized
 */
interface InternalState {
  stateKey: string;
  label: string;
  units: string;
  isAlarmLimitsEnabled: boolean;
  isParamEnabled: boolean;
  committedParam?: number | null;
  alarmLimitsDraft: Range | null;
  paramDraft: number;
  minValue?: number | null;
  maxValue?: number | null;
  alarmLimitMin?: number | null;
  alarmLimitMax?: number | null;
  alarmLimitsCurrent: Range | null;
  paramCurrent: number;
}

/**
 * @typedef HFNCControlProps
 *
 * Interface for HFNCProps
 *
 * @prop {alarmLimitsDraftSpO2} Draft alarm Limits for SpO2
 * @prop {alarmLimitsDraftHR}   Draft alarm Limits for HR
 * @prop {alarmLimitsDraftFiO2} Draft alarm Limits for FiO2
 * @prop {alarmLimitsDraftFlow} Draft alarm Limits for Flow
 */
interface HFNCControlProps {
  alarmLimitsDraftSpO2: Range;
  alarmLimitsDraftHR: Range;
  alarmLimitsDraftFiO2: Range;
  alarmLimitsDraftFlow: Range;
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
  alarmLimitsDraftSpO2,
  alarmLimitsDraftHR,
  alarmLimitsDraftFiO2,
  alarmLimitsDraftFlow,
}: HFNCControlProps): JSX.Element => {
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
          alarmLimits={alarmLimitsDraftSpO2}
          showLimits
        />
        <ValueInfo
          selector={getSmoothedHR}
          label="HR"
          stateKey="hr"
          units={BPM}
          alarmLimits={alarmLimitsDraftHR}
          showLimits
        />
      </Grid>
      <Grid container item justify="center" alignItems="stretch" direction="column">
        <ValueInfo
          selector={getParametersRequestDraftFiO2}
          label="FiO2"
          stateKey="fio2"
          units={PERCENT}
          alarmLimits={alarmLimitsDraftFiO2}
          showLimits
        />
        <ValueInfo
          selector={getParametersRequestDraftFlow}
          label="Flow"
          stateKey="flow"
          units={LMIN}
          alarmLimits={alarmLimitsDraftFlow}
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
 * @param {boolean} isAlarmLimitsEnabled Config to set if value is Alarm type
 * @param {boolean} isParamEnabled Config to set if value is Set type
 * @param {Range} alarmLimitsCurrent - Current Alarm values from redux store
 * @param {number | null} committedParam - Actual Alarm Value when Component was initalized
 * @param {number | null} minValue Minimum under which value cannot decrement
 * @param {number | null} maxValue Maximum above which value cannot increment
 * @param {number | null} alarmLimitMin Alarm limit Minimum Range value
 * @param {number | null} alarmLimitMax Alarm limit Maximum Range value
 *
 * @returns {InternalState} - returns the `InternalState` value
 */
const createInternalState = (
  label: string,
  stateKey: string,
  units: string,
  isParamEnabled: boolean,
  isAlarmLimitsEnabled: boolean,
  alarmLimitsCurrent: Range | null,
  committedParam?: number | null,
  minValue?: number | null,
  maxValue?: number | null,
  alarmLimitMin?: number | null,
  alarmLimitMax?: number | null,
): InternalState => {
  return {
    label,
    stateKey,
    units,
    isParamEnabled,
    isAlarmLimitsEnabled,
    committedParam,
    minValue,
    maxValue,
    alarmLimitMin,
    alarmLimitMax,
    alarmLimitsDraft: alarmLimitsCurrent,
    alarmLimitsCurrent,
    paramDraft: committedParam as number,
    paramCurrent: committedParam as number,
  };
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
  const [parameter, setParameter] = React.useState<InternalState>();
  /**
   * State to manage all Parameter `Data`
   */
  const [internalState, setInternalState] = React.useState<InternalState[]>([]);
  /**
   * State to manage if button submitted is disabled
   */
  const [isSubmitDisabled, setIsSubmitDisabled] = React.useState(false);

  const alarmLimitsRequestSpO2 = useSelector(getSpO2AlarmLimitsRequest, shallowEqual);
  const alarmLimitsRequestHR = useSelector(getHRAlarmLimitsRequest, shallowEqual);
  const alarmLimitsCurrentFiO2 = useSelector(getFiO2AlarmLimitsDraft, shallowEqual);
  const alarmLimitsCurrentFlow = useSelector(getFlowAlarmLimitsDraft, shallowEqual);
  const parametersFiO2 = useSelector(getParametersFiO2);
  const parametersFlow = useSelector(getParametersFlow);

  /**
   * Function to frame `InternalState` object based on `stateKey` provided
   *
   * TODO: Make a constant file for stateKey Constants
   *
   * @param {string} stateKey - Unique identifier for the value
   *
   * @returns {InternalState | undefined} - `InternalState` object containing configurations
   */
  const determineInput = useCallback(
    (state: string) => {
      const stateMap = new Map<string, InternalState>([
        [
          'spo2',
          createInternalState('SpO2', 'spo2', PERCENT, false, true, alarmLimitsRequestSpO2, -1),
        ],
        [
          'hr',
          createInternalState(
            'HR',
            'hr',
            BPM,
            false,
            true,
            alarmLimitsRequestHR,
            -1,
            null,
            null,
            0,
            200,
          ),
        ],
        [
          'fio2',
          createInternalState('FiO2', 'fio2', PERCENT, true, false, null, parametersFiO2, 21, null),
        ],
        [
          'flow',
          createInternalState(
            'Flow Rate',
            'flow',
            LMIN,
            true,
            false,
            null,
            parametersFlow,
            null,
            80,
          ),
        ],
      ]);
      const stateInput = stateMap.get(state);
      if (stateInput === undefined) {
        return undefined;
      }
      return stateInput;
    },
    [alarmLimitsRequestHR, alarmLimitsRequestSpO2, parametersFiO2, parametersFlow],
  );

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
  }, [determineInput]);

  /**
   * Triggers on TabIndex or Parameter change
   * Updates the label based on Parameter type configured
   */
  useEffect(() => {
    if (parameter) {
      if (tabIndex > 0) {
        setLabel(
          parameter.isParamEnabled ? `${parameter.label} Settings` : `${parameter.label} Alarms`,
        );
      } else {
        setLabel('Ventilation Controls');
      }
    }
  }, [tabIndex, parameter]);

  /**
   * Triggers on parameter or internalState change
   * Initally internalState has only 1 parameter which is set in Component Initalization
   * As user moves around the tabs and clicks on various Set/Alarm container
   * Then those are added into internalState array
   */
  useEffect(() => {
    if (parameter) {
      setTabIndex(1);
      if (!internalState.length) {
        setInternalState([parameter as InternalState]);
      } else {
        const identifier = internalState.find(
          (param: InternalState) => param.stateKey === parameter.stateKey,
        );
        if (!identifier) {
          internalState.push(parameter);
          setInternalState(internalState);
        }
      }
    }
  }, [parameter, internalState]);

  /**
   * Callback on whenever Draft Value of parameter changes
   *
   * @param {number} setting - Updated Draft value of parameter
   *
   * Updates the new Draft value into internalState `InternalState` object
   */
  const handleParamDraftRequest = (setting: number) => {
    if (parameter) {
      const param = internalState.find(
        (param: InternalState) => param.stateKey === parameter.stateKey,
      );
      if (param) param.paramDraft = setting;
      parameter.paramDraft = setting;
      if (isAnyChanges()) {
        setIsSubmitDisabled(false);
      } else {
        setIsSubmitDisabled(true);
      }
    }
  };

  /**
   * Callback on whenever Alarm Limits of parameter changes
   *
   * @param {Range} range- Updated alarm limits range
   *
   * Updates the new alarm range values into internalState `Data` object
   */
  const handleAlarmLimitsRequest = (min: number, max: number) => {
    if (parameter) {
      const param = internalState.find(
        (param: InternalState) => param.stateKey === parameter.stateKey,
      );
      if (param) param.alarmLimitsDraft = { lower: min, upper: max };
      parameter.alarmLimitsDraft = { lower: min, upper: max };
      if (isAnyChanges()) {
        setIsSubmitDisabled(false);
      } else {
        setIsSubmitDisabled(true);
      }
    }
  };

  /**
   * Function to get Alarm Limits Draft from Parameter `InternalState` object
   *
   * @param {string} stateKey - stateKey identifier for which parameter `InternalState` corresponds to
   *
   * @return {Range | null} - Current alarm Limits
   *
   */
  const getAlarmLimitsDraft = (stateKey: string): Range | null => {
    const param = internalState.find((param: InternalState) => param.stateKey === stateKey);
    if (param) {
      if (param.alarmLimitsDraft) return param.alarmLimitsDraft;
    }
    return null;
  };

  /**
   * Function to get Draft Parameter Values from `InternalState` object
   *
   * @param {string} stateKey - stateKey identifier for which parameter `InternalState` corresponds to
   *
   * @return {number} - Draft Parameter value
   *
   */
  const getParamDraft = (stateKey: string): number => {
    const param = internalState.find((param: InternalState) => param.stateKey === stateKey);
    if (param && param.paramDraft) {
      return param.paramDraft as number;
    }
    if (parameter) return parameter.committedParam as number;
    return 0;
  };

  /**
   * Checks if there are any changes user has made
   *
   * @return {boolean} - true if change is there; false if there are no changes
   *
   */
  const isAnyChanges = (): boolean => {
    let anyChange = false;
    internalState.forEach((param: InternalState) => {
      if (param.isParamEnabled) {
        if (param.paramDraft !== param.paramCurrent) {
          anyChange = true;
        }
      } else if (
        param.alarmLimitsDraft &&
        param.alarmLimitsCurrent &&
        (param.alarmLimitsDraft.lower !== param.alarmLimitsCurrent.lower ||
          param.alarmLimitsDraft.upper !== param.alarmLimitsCurrent.upper)
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
   * Updates internalState data into redux store
   * Closes all te popup
   */
  const handleConfirm = () => {
    internalState.forEach((parameter: InternalState) => {
      if (parameter.isParamEnabled) {
        const update = { [parameter.stateKey]: parameter.paramDraft };
        dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
        dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
      }
      if (parameter.isAlarmLimitsEnabled && parameter.alarmLimitsDraft) {
        const update = {
          [parameter.stateKey]: parameter.alarmLimitsDraft,
        };
        dispatch(commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
        dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
      }
    });
    setInternalState([]);
    setConfirmOpen(false);
    setMultiPopupOpen(false);
  };

  /**
   * Callback on clicking Confirm button on Cancel popup
   * Undo all the user made changes
   */
  const handleCancelConfirm = () => {
    if (parameter) {
      parameter.paramDraft = parameter.paramCurrent;
      parameter.alarmLimitsDraft = parameter.alarmLimitsCurrent;
    }
    internalState.map((parameter: InternalState) => {
      const param = parameter;
      param.paramDraft = param.paramCurrent;
      param.alarmLimitsDraft = param.alarmLimitsCurrent;
      return param;
    });
    setInternalState([]);
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

  const updateDraftParams = () => {
    let update = {};
    internalState.map((param: InternalState) => {
      update = { [param.stateKey]: param.paramDraft };
      return update;
    });
    dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  const handleTabChange = () => {
    setTabIndex(tabIndex - 1);
    updateDraftParams();
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
                onClick={handleTabChange}
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
                onClick={() => updateDraftParams()}
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
                  parameter?.isParamEnabled
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
              <HFNCControls
                alarmLimitsDraftSpO2={getAlarmLimitsDraft('spo2') || alarmLimitsRequestSpO2}
                alarmLimitsDraftHR={getAlarmLimitsDraft('hr') || alarmLimitsRequestHR}
                alarmLimitsDraftFiO2={alarmLimitsCurrentFiO2}
                alarmLimitsDraftFlow={alarmLimitsCurrentFlow}
              />
            </TabPanel>
            <TabPanel value={tabIndex} index={1}>
              {parameter && parameter.isParamEnabled ? (
                <ValueModalContent
                  openModal={open && parameter.stateKey === stateKey}
                  key={parameter.stateKey}
                  committedSetting={getParamDraft(parameter.stateKey)}
                  label={parameter.label}
                  units={parameter.units}
                  requestCommitSetting={handleParamDraftRequest}
                  {...(parameter.minValue && { min: parameter.minValue })}
                  {...(parameter.maxValue && { max: parameter.maxValue })}
                />
              ) : (
                parameter && (
                  <AlarmModalContent
                    label={parameter.label}
                    units={parameter.units}
                    stateKey={parameter.stateKey}
                    handleCommittedRange={handleAlarmLimitsRequest}
                    labelHeading={true}
                    alarmRangeValues={getAlarmLimitsDraft(parameter.stateKey)}
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
              {internalState.map((param: InternalState) => {
                if (param.isParamEnabled) {
                  if (param.paramDraft !== param.paramCurrent) {
                    return (
                      <Typography variant="subtitle1">{`Change ${param.label} from ${param.paramCurrent} ${param.units} to ${param.paramDraft} ${param.units}?`}</Typography>
                    );
                  }
                } else if (
                  param.alarmLimitsDraft &&
                  param.alarmLimitsCurrent &&
                  (param.alarmLimitsDraft.lower !== param.alarmLimitsCurrent.lower ||
                    param.alarmLimitsDraft.upper !== param.alarmLimitsCurrent.upper)
                ) {
                  return (
                    <Typography variant="subtitle1">{`Change ${param.label} alarm limits from [${param.alarmLimitsCurrent.lower} ${param.units} - ${param.alarmLimitsCurrent.upper} ${param.units}] to [${param.alarmLimitsDraft.lower} ${param.units} - ${param.alarmLimitsDraft.upper} ${param.units}]?`}</Typography>
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
              {internalState.map((param: InternalState) => {
                if (param.isParamEnabled) {
                  if (param.paramDraft !== param.paramCurrent) {
                    return (
                      <Typography variant="subtitle1">{`Keep ${param.label} at ${param.paramCurrent} ${param.units} instead of changing to ${param.paramDraft} ${param.units}?`}</Typography>
                    );
                  }
                } else if (
                  param.alarmLimitsDraft &&
                  param.alarmLimitsCurrent &&
                  (param.alarmLimitsDraft.lower !== param.alarmLimitsCurrent.lower ||
                    param.alarmLimitsDraft.upper !== param.alarmLimitsCurrent.upper)
                ) {
                  return (
                    <Typography variant="subtitle1">{`Keep ${param.label} alarm limits at [${param.alarmLimitsCurrent.lower} ${param.units} - ${param.alarmLimitsCurrent.upper} ${param.units}] instead of changing to [${param.alarmLimitsDraft.lower} ${param.units} - ${param.alarmLimitsDraft.upper} ${param.units}]?`}</Typography>
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

const automaticAlarmLimitsRangeSelector = (selector: SelectorType, value: number) =>
  createSelector(selector, (param: number) => {
    return { lower: param - value, upper: param + value };
  });
const getFiO2AlarmLimitsDraft = automaticAlarmLimitsRangeSelector(getParametersRequestDraftFiO2, 2);
const getFlowAlarmLimitsDraft = automaticAlarmLimitsRangeSelector(getParametersRequestDraftFlow, 2);

export default MultiStepWizard;
