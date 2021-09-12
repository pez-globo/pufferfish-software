/**
 * @summary Component for Quick Start page
 *
 *
 */
import { Grid, Typography } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React from 'react';
import { useSelector } from 'react-redux';
import { VentilationMode } from '../../../store/proto/mcu_pb';
import {
  getParametersRequestMode,
  getParametersRequestDraftFiO2,
  getParametersRequestDraftFlow,
  getParametersRequestDraftPEEP,
  getParametersRequestDraftRR,
  getParametersRequestDraftVT,
} from '../../../store/controller/selectors';
import { setActiveRotaryReference } from '../Service';
import ModeBanner, { BannerType } from '../../modes/ModeBanner';
import { BPM, LMIN, PERCENT } from '../../shared/units';
import {
  FIO2_REFERENCE_KEY,
  PEEP_REFERENCE_KEY,
  RR_REFERENCE_KEY,
  TV_REFERENCE_KEY,
  FLOW_REFERENCE_KEY,
} from '../../settings/tabs/constants';
import ParamValueSpinner from '../../controllers/ParamValueSpinner';

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
          <ParamValueSpinner
            label="FiO2"
            reference={FIO2_REFERENCE_KEY}
            selector={getParametersRequestDraftFiO2}
            stateKey="fio2"
            units={PERCENT}
          />
        </Grid>
        <Grid item xs>
          <ParamValueSpinner
            label="Flow"
            reference={FLOW_REFERENCE_KEY}
            selector={getParametersRequestDraftFlow}
            stateKey="flow"
            units={LMIN}
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
          <ParamValueSpinner
            label="PEEP"
            reference={PEEP_REFERENCE_KEY}
            selector={getParametersRequestDraftPEEP}
            stateKey="peep"
            units="cm H2O"
          />
        </Grid>
        <Grid item xs>
          <ParamValueSpinner
            label="RR"
            reference={RR_REFERENCE_KEY}
            selector={getParametersRequestDraftRR}
            stateKey="rr"
            units={BPM}
          />
        </Grid>
      </Grid>
      <Grid container item xs direction="row">
        <Grid item xs className={classes.rightBorder}>
          <ParamValueSpinner
            label="FiO2"
            reference={FIO2_REFERENCE_KEY}
            selector={getParametersRequestDraftFiO2}
            stateKey="fio2"
            units={PERCENT}
          />
        </Grid>
        <Grid item xs>
          <ParamValueSpinner
            label="TV"
            reference={TV_REFERENCE_KEY}
            selector={getParametersRequestDraftVT}
            stateKey="tv"
            units="mL"
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
