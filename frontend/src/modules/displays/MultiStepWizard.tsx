import React, { useEffect } from 'react';
import { Subscription } from 'rxjs';
import { useDispatch } from 'react-redux';
import { makeStyles, Theme, Grid, Tabs, Tab, Button } from '@material-ui/core';
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
      alarmValues: [],
      setValue: 0,
    };
  };

  const getStoreData = (stateKey: string): number | null => {
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

  switch (stateKey) {
    case 'spo2':
      return createData('SpO2', stateKey, PERCENT, false, true);
    case 'rr':
      return createData('RR', stateKey, BMIN, false, true);
    case 'fio2':
      return createData('FiO2', stateKey, PERCENT, true, false, getStoreData(stateKey), 21);
    case 'flow':
      return createData('Flow Rate', stateKey, LMIN, true, false, getStoreData(stateKey), null, 80);
    default:
  }

  return null;
};

const MultiStepWizard = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  const [open, setOpen] = React.useState(false);
  const [label, setLabel] = React.useState('Ventilation Controls');
  const [stateKey, setStateKey] = React.useState('');
  const [tabIndex, setTabIndex] = React.useState(0);
  const [parameter, setParameter] = React.useState<Data | null>();

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
    if (tabIndex > 0) {
      setLabel(parameter?.isSetvalEnabled ? 'Set New' : 'Alarms');
    } else {
      setLabel('Ventilation Controls');
    }
  }, [tabIndex, parameter]);

  useEffect(() => {
    setTabIndex(1);
    if (parameter) {
      parameter.setValue = 0;
      parameter.alarmValues = [];
    }
  }, [parameter]);

  const doSetValue = (setting: number) => {
    if (parameter) {
      parameter.setValue = setting;
    }
  };

  const doSetAlarmValues = (min: number, max: number) => {
    if (parameter) {
      parameter.alarmValues = [min, max];
    }
  };

  const onConfirm = () => {
    if (parameter?.isSetvalEnabled) {
      dispatch(updateCommittedParameter({ [stateKey]: parameter.setValue }));
      dispatch(updateCommittedState(PARAMETER_STANDBY, { [stateKey]: parameter.setValue }));
    }
    if (parameter?.isAlarmEnabled && parameter.alarmValues.length) {
      dispatch(
        updateCommittedState(ALARM_LIMITS, {
          [stateKey]: { lower: parameter.alarmValues[0], upper: parameter.alarmValues[1] },
        }),
      );
      dispatch(
        updateCommittedState(ALARM_LIMITS_STANDBY, {
          [stateKey]: { lower: parameter.alarmValues[0], upper: parameter.alarmValues[1] },
        }),
      );
    }
  };

  return (
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
              committedSetting={parameter.committedSetting as number}
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
            />
          )}
        </TabPanel>
      </Grid>

      <Grid container item alignItems="center" justify="flex-end" className={classes.actionButtons}>
        <Grid item>
          <Button
            variant="contained"
            className={classes.aButtons}
            style={{ marginRight: '15px' }}
            onClick={() => setMultiPopupOpen(false)}
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
  );
};

export default MultiStepWizard;
