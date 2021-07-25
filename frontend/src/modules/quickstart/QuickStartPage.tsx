/**
 * @summary Component for Quick Start page
 *
 *
 */
import { Grid, Typography } from '@material-ui/core';
import { makeStyles, Theme, useTheme } from '@material-ui/core/styles';
import React, { RefObject, useEffect, useRef } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitDraftRequest } from '../../store/controller/actions';
import { VentilationMode, ParametersRequest } from '../../store/proto/mcu_pb';
import {
  getParametersRequestMode,
  getParametersRequestDraftFiO2,
  getParametersRequestDraftFlow,
  getParametersRequestDraftPEEP,
  getParametersRequestDraftRR,
  getParametersRequestDraftVT,
} from '../../store/controller/selectors';
import { MessageType } from '../../store/proto/types';
import { setActiveRotaryReference } from '../app/Service';
import ValueSpinner from '../controllers/ValueSpinner';
import ModeBanner, { BannerType } from '../displays/ModeBanner';
import { BPM, LMIN, PERCENT } from '../info/units';
import {
  FIO2_REFERENCE_KEY,
  PEEP_REFERENCE_KEY,
  RR_REFERENCE_KEY,
  TV_REFERENCE_KEY,
  FLOW_REFERENCE_KEY,
} from '../settings/containers/constants';
import { useRotaryReference } from '../utils/useRotaryReference';
import { SelectorType } from '../displays/ValueSelectorDisplay';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    justifyContent: 'space-between',
    height: '100%',
    flexWrap: 'nowrap',
    // border: '1px solid red',
  },
  topPanel: {
    flexWrap: 'nowrap',
    borderRadius: theme.panel.borderRadius,
    marginBottom: '15px',
    justifyContent: 'space-between',
    alignItems: 'stretch',
    backgroundColor: theme.palette.background.paper,
  },
  standby: {
    padding: theme.spacing(2),
    borderRight: `2px dashed ${theme.palette.background.default}`,
  },
  padding: {
    padding: theme.spacing(2),
    // border: '1px solid red'
  },
  paddingTop: {
    paddingTop: theme.spacing(3),
  },
  middleContainer: {
    flexWrap: 'nowrap',
    width: '100%',
    marginBottom: '15px',
    // border: '1px solid teal',
  },
  middleLeftPanel: {
    borderRadius: theme.panel.borderRadius,
    marginRight: theme.spacing(2),
    backgroundColor: theme.palette.background.paper,
  },
  middleRightPanel: {
    borderRadius: theme.panel.borderRadius,
    backgroundColor: theme.palette.background.paper,
  },
  bottomBorder: {
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  rightBorder: {
    borderRight: `2px dashed ${theme.palette.background.default}`,
  },
}));

// enum PatientSex {
//   MALE,
//   FEMALE,
// }
// enum PatientAge {
//   ADULT,
//   PEDIATRIC,
// }

interface ValueProps {
  selector: SelectorType;
  reference: string;
  label: string;
  stateKey: string;
  units: string;
}

const ParamValueClicker = ({
  selector,
  reference,
  label,
  stateKey,
  units,
}: ValueProps): JSX.Element => {
  const dispatch = useDispatch();
  const theme = useTheme();
  const value = useSelector(selector);
  const { initRefListener } = useRotaryReference(theme);

  /**
   * State to manage Wrapper HTML reference of parameter `ValueSpinner`
   * This wrapper's HTML border is added or removed based on user's interaction with Controls
   * It is used for UI identification of which control value is changing via rotary encoder
   */
  const [elRefs] = React.useState<Record<string, RefObject<HTMLDivElement>>>({
    [PEEP_REFERENCE_KEY]: useRef(null),
    [RR_REFERENCE_KEY]: useRef(null),
    [FIO2_REFERENCE_KEY]: useRef(null),
    [TV_REFERENCE_KEY]: useRef(null),
    [FLOW_REFERENCE_KEY]: useRef(null),
  });

  /**
   * Calls on initalization of the component
   * This is an event listener which listens to user input on `ValueSpinner` buttons click
   * Based on this event Border around Alarm's HTML wrapper is added/removed
   */
  useEffect(() => {
    initRefListener(elRefs);
  }, [initRefListener, elRefs]);

  const updateParam = (key: string, value: number) => {
    const update = { [key]: value } as Partial<ParametersRequest>;
    dispatch(commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  return (
    <ValueSpinner
      reference={elRefs[reference]}
      referenceKey={reference}
      label={label}
      units={units}
      value={value}
      onClick={(value: number) => updateParam(stateKey, value)}
    />
  );
};

/**
 * SetParameters
 *
 * @component for setting quick start parameters

 *
 * @returns {JSX.Element}
 *
 */
const SetParameters = (): JSX.Element => {
  const classes = useStyles();

  const hfncModeContent = (
    <Grid container item xs={8} direction="column" className={classes.middleRightPanel}>
      <Grid container item xs direction="row" className={classes.bottomBorder}>
        <Grid item xs className={classes.rightBorder}>
          <ParamValueClicker
            reference={FIO2_REFERENCE_KEY}
            label="FiO2"
            stateKey="fio2"
            units={PERCENT}
            selector={getParametersRequestDraftFiO2}
          />
        </Grid>
        <Grid item xs>
          <ParamValueClicker
            reference={FLOW_REFERENCE_KEY}
            label="Flow"
            units={LMIN}
            stateKey="flow"
            selector={getParametersRequestDraftFlow}
          />
        </Grid>
      </Grid>
      <Grid container item xs direction="row">
        <Grid item xs className={classes.rightBorder} />
        <Grid item xs />
      </Grid>
    </Grid>
  );

  // Currently un-used (HFNC ONLY MODE)
  // so the OnClick Callback is null
  // TODO: fix defaultContent onClick callback when we have other modes
  const defaultContent = (
    <Grid container item xs={8} direction="column" className={classes.middleRightPanel}>
      <Grid container item xs direction="row" className={classes.bottomBorder}>
        <Grid item xs className={classes.rightBorder}>
          <ParamValueClicker
            reference={PEEP_REFERENCE_KEY}
            label="PEEP"
            stateKey="peep"
            units="cm H2O"
            selector={getParametersRequestDraftPEEP}
          />
        </Grid>
        <Grid item xs>
          <ParamValueClicker
            reference={RR_REFERENCE_KEY}
            label="RR"
            stateKey="rr"
            units={BPM}
            selector={getParametersRequestDraftRR}
          />
        </Grid>
      </Grid>
      <Grid container item xs direction="row">
        <Grid item xs className={classes.rightBorder}>
          <ParamValueClicker
            reference={FIO2_REFERENCE_KEY}
            label="FiO2"
            stateKey="fio2"
            units={PERCENT}
            selector={getParametersRequestDraftFiO2}
          />
        </Grid>
        <Grid item xs>
          <ParamValueClicker
            reference={TV_REFERENCE_KEY}
            label="TV"
            stateKey="tv"
            units="mL"
            selector={getParametersRequestDraftVT}
          />
        </Grid>
      </Grid>
    </Grid>
  );

  const ventilationMode = useSelector(getParametersRequestMode);
  if (ventilationMode === null) {
    return defaultContent;
  }
  const contentMap = new Map<VentilationMode, JSX.Element>([
    [VentilationMode.hfnc, hfncModeContent],
  ]);
  const modeContent = contentMap.get(ventilationMode);
  if (modeContent === undefined) {
    return defaultContent;
  }
  return modeContent;
};

/**
 * QuickStartPage
 *
 * @component for displaying quick start page
 *
 * @returns {JSX.Element}
 *
 */
export const QuickStartPage = (): JSX.Element => {
  const classes = useStyles();
  // const [patientSex, setPatientSex] = React.useState(PatientSex.MALE);
  // const [patientAge, setPatientAge] = React.useState(PatientAge.ADULT);
  // const [patientHeight, setPatientHeight] = React.useState(62);
  // const [patientCircuitTestDate] = React.useState(new Date());
  // const [preUseCheckDate] = React.useState(new Date());

  /**
   * Resets highlighting border around container when clicked across the page
   * Border is usually added on `ValueSpinner` button click
   */
  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid container direction="column" className={classes.root} onClick={OnClickPage}>
      <Grid container item className={classes.topPanel}>
        <Grid item xs className={classes.standby}>
          <Typography variant="h3">Standby</Typography>
          <Typography variant="body1">Patient Not Ventilated</Typography>
        </Grid>
        <Grid item xs className={`${classes.standby} ${classes.padding}`}>
          {/* <Typography variant="h6">Age:</Typography>
          <Grid item xs className={classes.paddingTop}>
            <ToggleValue
              toggleBetween={[
                { label: 'Adult', value: PatientAge.ADULT },
                { label: 'Pediatric', value: PatientAge.PEDIATRIC },
              ]}
              onToggle={(selected: number) => setPatientAge(selected as PatientAge)}
              selected={patientAge}
            />
          </Grid> */}
        </Grid>
        <Grid item xs className={`${classes.standby} ${classes.padding}`}>
          {/* <Typography variant="h6">Sex:</Typography>
          <Grid item xs className={classes.paddingTop}>
            <ToggleValue
              toggleBetween={[
                { label: 'Male', value: PatientSex.MALE },
                { label: 'Female', value: PatientSex.FEMALE },
              ]}
              onToggle={(selected: number) => setPatientSex(selected as PatientSex)}
              selected={patientSex}
            />
          </Grid> */}
        </Grid>
        <Grid item xs={3}>
          {/* <ValueSpinner
            label="Height"
            units="i"
            onClick={setPatientHeight}
            value={patientHeight}
            min={6}
            max={120}
          /> */}
        </Grid>
      </Grid>
      <Grid container item xs className={classes.middleContainer}>
        <Grid container item xs direction="column" className={classes.middleLeftPanel}>
          <Grid item xs className={classes.bottomBorder}>
            {/* <TestTool label="Pre-Use Check" isSuccess={true} timestamp={preUseCheckDate} /> */}
          </Grid>
          <Grid item xs>
            {/* <TestTool
              label="Patient Circuit Test"
              isSuccess={true}
              timestamp={patientCircuitTestDate}
            /> */}
          </Grid>
        </Grid>
        <SetParameters />
      </Grid>
      <Grid>
        <ModeBanner bannerType={BannerType.NORMAL} />
      </Grid>
    </Grid>
  );
};

export default QuickStartPage;
