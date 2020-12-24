import React, { PropsWithChildren, useEffect } from "react";
import ModalPopup from "../controllers/ModalPopup";
import { Subscription } from 'rxjs';
import { getcurrentStateKey, getMultiPopupOpenState, setMultiPopupOpen } from "../app/Service";
import { useDispatch, useSelector } from "react-redux";
import { getCycleMeasurementsRR, getParametersFiO2, getParametersFlow, getSensorMeasurementsFiO2Value, getSensorMeasurementsFlow, getSensorMeasurementsSpO2 } from "../../store/controller/selectors";
import { SetValueContent } from "../controllers/ValueModal";
import { makeStyles, Theme, Grid, Tabs, Tab } from "@material-ui/core";
import { a11yProps, TabPanel } from "../controllers/TabPanel";
import ValueInfo from "../dashboard/containers/ValueInfo";
import { BMIN, LMIN, PERCENT } from "../info/units";
import { AlarmModal } from "../controllers";
import { SelectorType } from "./ValueSelectorDisplay";
import { dispatch } from "rxjs/internal/observable/pairs";
import { updateCommittedParameter, updateCommittedState } from "../../store/controller/actions";
import { ALARM_LIMITS, ALARM_LIMITS_STANDBY, PARAMETER_STANDBY } from "../../store/controller/types";

interface Data {
    stateKey: string,
    label: string,
    units: string,
    isAlarmEnabled: boolean,
    isSetvalEnabled: boolean,
    committedSetting?: SelectorType,
    alarmValues: number[],
    setValue: number,
    minValue?: number | null,
    maxValue?: number | null,
}

const useStyles = makeStyles((theme: Theme) => ({
    tabPanelContainer: {
        flexGrow: 1,
        justifyContent: 'space-between',
        alignItems: 'stretch',
        height: '100',
        // border: '1px solid green',
    },
    tabs: {
        // paddingTop: theme.spacing(1)
        // border: '1px solid red',
        width: '100%',
    },
    tab: {
        borderRadius: 8,
        border: `2px solid ${theme.palette.primary.main}`,
        margin: '0px 8px',
        zIndex: 1,
        minHeight: 40,
        minWidth: 220,
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
}));

const HFNCControls = (): JSX.Element => {
    return (
        <React.Fragment>
            <Grid container item justify="center" alignItems="stretch">
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
            <Grid container item justify="center" alignItems="stretch">
                <ValueInfo
                    mainContainer={{
                        selector: getSensorMeasurementsFiO2Value,
                        label: "FiO2",
                        stateKey: "fio2",
                        units: PERCENT
                    }}
                />
                <ValueInfo
                    mainContainer={{
                        selector: getSensorMeasurementsFlow,
                        label: "Flow Rate",
                        stateKey: "flow",
                        units: LMIN
                    }}
                />
            </Grid>
        </React.Fragment>
    )
}

// TODO: Make a constant file for stateKey Constants
const determineInput = (stateKey: string) => {

    const createData = (
        label: string,
        stateKey: string,
        units: string,
        isSetvalEnabled: boolean,
        isAlarmEnabled: boolean,
        committedSetting?: SelectorType,
        minValue?: number | null,
        maxValue?: number | null,
    ): Data => {
        return { label, stateKey, units, isSetvalEnabled, isAlarmEnabled, committedSetting, minValue, maxValue, alarmValues: [], setValue: 0 };
    };

    switch (stateKey) {
        case 'spo2':
            return createData('SpO2',stateKey, PERCENT, false, true);
        case 'rr':
            return createData('RR', stateKey, BMIN, false, true);
        case 'fio2':
            return createData('FiO2', stateKey, PERCENT, true, false, getParametersFlow, 21);
        case 'flow':
            return createData('Flow Rate', stateKey, LMIN, true, false, getParametersFiO2, null, 80);
        default:
            break;
    }
}


const MultiStepWizard = () => {
    const classes = useStyles();
    const dispatch = useDispatch();
    const [open, setOpen] = React.useState(false);
    const [label, setLabel] = React.useState('Ventilation Controls');
    const [stateKey, setStateKey] = React.useState('');
    const [tabIndex, setTabIndex] = React.useState(0);
    const [parameter, setParameter] = React.useState<Data>();

    const handleChange = (event: React.ChangeEvent<Record<string, unknown>>, newValue: number) => {
        setTabIndex(newValue);
    };

    useEffect(() => {
        const popupEventSubscription: Subscription = getMultiPopupOpenState().subscribe((state: boolean) => {
            setOpen(state);
        });
        const stateKeyEventSubscription: Subscription = getcurrentStateKey().subscribe((state: string) => {
            setStateKey(state);
        });
        return () => {
            if (popupEventSubscription) {
                popupEventSubscription.unsubscribe();
            }
            if (stateKeyEventSubscription) {
                stateKeyEventSubscription.unsubscribe();
            }
        };
    }, [])

    useEffect(() => {
        setParameter(determineInput(stateKey));
    }, [stateKey])

    useEffect(() => {
        console.log('state key ', stateKey, parameter);
        setTabIndex(1);
        if(parameter) {
            parameter.setValue = 0;
            parameter.alarmValues = [];
        }
    }, [parameter])

    const doSetValue = (setting: number) => {
        console.log('Chhange set valye ', setting);
        if(parameter) {
            parameter.setValue = setting;
        }
    };

    const doSetAlarmValues = (min: number, max: number) => {
        console.log('Chhange alarm valye ', min, max);
        if(parameter) {
            parameter.alarmValues = [min, max];
        }
    };

    const onConfirm = () => {
        if(parameter?.isSetvalEnabled) {
            console.log('Chhange set valye ',parameter.setValue, stateKey);
            dispatch(updateCommittedParameter({ [stateKey]: parameter.setValue }));
            dispatch(updateCommittedState(PARAMETER_STANDBY, { [stateKey]: parameter.setValue }));
        }
        if(parameter?.isAlarmEnabled) {
            console.log('Chhange alarm valye ',parameter.alarmValues, stateKey);
            dispatch(
                updateCommittedState(ALARM_LIMITS, {
                  [`${stateKey}Min`]: parameter.alarmValues[0],
                  [`${stateKey}Max`]: parameter.alarmValues[1],
                }),
              );
              dispatch(
                updateCommittedState(ALARM_LIMITS_STANDBY, {
                  [`${stateKey}Min`]: parameter.alarmValues[0],
                  [`${stateKey}Max`]: parameter.alarmValues[1],
                }),
              );
        }
    }

    return (
        <ModalPopup
            withAction={true}
            label={label}
            open={open}
            onClose={() => setMultiPopupOpen(false)}
            onConfirm={onConfirm}
        >
            <Grid container>
                <TabPanel value={tabIndex} index={0}>
                    <HFNCControls />
                </TabPanel>
                <TabPanel value={tabIndex} index={1}>
                    {parameter?.isSetvalEnabled ?
                        <SetValueContent
                            updateModalStatus={() => { }}
                            openModal={open}
                            disableSetNewButton={true}
                            committedSettingSelector={parameter.committedSetting}
                            label={parameter.label}
                            units={parameter.units}
                            requestCommitSetting={doSetValue}
                            {...(parameter.minValue && { min: parameter.minValue })}
                            {...(parameter.maxValue && { max: parameter.maxValue })}
                        />
                        :
                        <AlarmModal
                            updateModalStatus={() => { }}
                            openModal={open}
                            disableAlarmButton={true}
                            label={parameter?.label || ''}
                            units={parameter?.units || ''}
                            stateKey={stateKey}
                            requestCommitRange={doSetAlarmValues}
                            contentOnly={true}
                        />
                    }
                </TabPanel>
            </Grid>
            <Grid container item justify="center" alignItems="center">
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
                        label={parameter?.isSetvalEnabled ? "Set New" : "Alarms"}
                        {...a11yProps(1)}
                        className={classes.tab}
                        classes={{ selected: classes.selectedTab }}
                    />
                    <Grid container item justify="flex-end" alignItems="center">

                    </Grid>
                </Tabs>
            </Grid>

        </ModalPopup>
    )
};

export default MultiStepWizard;
