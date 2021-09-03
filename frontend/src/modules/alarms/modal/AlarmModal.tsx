/**
 * @summary Alarm Modal controller to set alarm range
 *
 */
import { Grid, makeStyles, Theme, Typography, useTheme } from '@material-ui/core';
import React, { RefObject, useCallback, useEffect, useRef } from 'react';
import { shallowEqual, useDispatch, useSelector } from 'react-redux';
import { commitRequest, commitDraftRequest } from '../../../store/controller/actions';
import { getAlarmLimitsRequest } from '../../../store/controller/selectors';
import { Range, AlarmLimitsRequest } from '../../../store/proto/mcu_pb';
import { MessageType } from '../../../store/proto/types';
import ModalPopup from '../../modals/ModalPopup';
import ValueClicker from '../../controllers/ValueClicker';
import ValueSlider from '../../controllers/ValueSlider';
import useRotaryReference from '../../utils/useRotaryReference';
import { setActiveRotaryReference } from '../../app/Service';

const useStyles = makeStyles((theme: Theme) => ({
  contentContainer: {
    width: '100%',
    height: '100%',
    minHeight: '290px',
    border: `2px dashed ${theme.palette.background.default}`,
    borderRadius: theme.panel.borderRadius,
    margin: '10px 0px',
  },
  borderBottom: {
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  openButton: {
    width: '100%',
    border: `1px solid ${theme.palette.common.black}`,
    padding: '5px 16px',
  },
  alarmValue: {
    marginRight: theme.spacing(3),
    border: `1px solid ${theme.palette.text.primary}`,
    minWidth: 75,
    paddingLeft: theme.spacing(1),
    paddingRight: theme.spacing(1),
    borderRadius: 8,
  },
  head: {
    '& h3': {
      lineHeight: '1.5',
      padding: '0px 15px',
    },
  },
}));

/**
 * @typedef AlarmAdjustProps
 *
 * Interface for the alarm adjustments
 *
 * @prop {number} committedMin Lower Set Alarm Range Value
 * @prop {number} committedMax Upper Set Alarm Range Value
 */
export interface AlarmAdjustProps {
  committedMin: number;
  committedMax: number;
}

/**
 * @typedef Props
 *
 * Props interface for the AlarmModal/AlarmModalContent component
 *
 * @prop {string} label Alarm label to display in UI
 * @prop {string} units Alarm paramater unit measurement to display
 * @prop {number} committedMin Lower Set Alarm Range Value
 * @prop {number} committedMax Upper Set Alarm Range Value
 * @prop {function} updateModalStatus Callback to send current modal open/close status
 * @prop {function} onModalClose Callback after Modal close
 * @prop {function} handleCommittedRange Callback on updating the Alarm range values
 * @prop {string} stateKey Unique identifier of alarm (eg spo2, fio2...)
 * @prop {number} step Alarm step difference between Range (Defaults to 1)
 * @prop {boolean} openModal Default value to toggle Open/Close Alarm Modal
 * @prop {boolean} labelHeading Switch to show/hide Alarm Label inside Alarm Modal content
 * @prop {number[]} alarmRangeValues Alarm Range Values, if not initialized, committedMin and committedMax are displayed.
 */
interface Props {
  label: string;
  units?: string;
  committedMin?: number;
  committedMax?: number;
  updateModalStatus?(status: boolean): void;
  onModalClose?(status: boolean): void;
  handleCommittedRange(min: number, max: number): void;
  stateKey: string;
  step?: number;
  openModal?: boolean;
  labelHeading?: boolean;
  alarmRangeValues?: Range | null;
}

/**
 * AlarmModalContent
 *
 * @component A container for displaying alarm modal.
 * This Component returns JSX which displays Value for lower/upper limits and ValueClickers to control them,
 * AlarmLimits for only a single Parameter can be controlled (eg: Alarm Limits for SpO2), thus needs to be selected
 * with stateKey.
 * It's currently re-used in AlarmModal as content inside the ModalPopup, and used exclusively
 * in MultiStepWizard for Alarm Modal content as that's already an ModalPopup and content is populated using this component.
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const AlarmModalContent = ({
  label,
  committedMin = 0,
  committedMax = 100,
  handleCommittedRange,
  stateKey,
  step,
  labelHeading = false,
  alarmRangeValues = null,
}: Props): JSX.Element => {
  const classes = useStyles();
  const theme = useTheme();
  const { initRefListener } = useRotaryReference(theme);
  /**
   * State to initalize Lower Set value
   */
  const [min] = React.useState(committedMin);
  /**
   * State to initalize Upper Set value
   */
  const [max] = React.useState(committedMax);
  const alarmLimits = useSelector(getAlarmLimitsRequest, shallowEqual);
  const range =
    alarmLimits === null
      ? undefined
      : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
  const { lower, upper } = range === undefined ? Range.fromJSON({ lower: NaN, upper: NaN }) : range;
  const [rangeValue, setRangeValue] = React.useState<number[]>([
    alarmRangeValues && alarmRangeValues.lower !== 0 ? alarmRangeValues.lower : lower,
    alarmRangeValues && alarmRangeValues.upper !== 0 ? alarmRangeValues.upper : upper,
  ]);
  /**
   * State to provide reference HTML element for Lower/Upper Wrapper
   * Its used in highlighting the HTML Element while using rotary encoder
   */
  const [refs] = React.useState<Record<string, RefObject<HTMLDivElement>>>({
    [`${stateKey}_LOWER`]: useRef(null),
    [`${stateKey}_HIGHER`]: useRef(null),
  });
  // const rangeValueString = JSON.stringify(rangeValue);

  const setUpperLimit = (value: number) => {
    setRangeValue(Object.assign([], rangeValue, { 1: value }));
  };

  const setLowerLimit = (value: number) => {
    setRangeValue(Object.assign([], rangeValue, { 0: value }));
  };

  useEffect(() => {
    initRefListener(refs);
  }, [initRefListener, refs]);

  /**
   * Triggers whenever rangeValue is updated in redux
   */
  useEffect(() => {
    handleCommittedRange(rangeValue[0], rangeValue[1]);
  }, [handleCommittedRange, rangeValue]);

  /**
   * Resets highlighting border around alarm container when clicked across the page
   * Border is usually added on `ValueClicker` button click
   */
  const OnClickPage = () => {
    setActiveRotaryReference(null);
  };

  return (
    <Grid
      container
      direction="column"
      alignItems="stretch"
      className={classes.contentContainer}
      onClick={OnClickPage}
    >
      {labelHeading && (
        <Grid container item xs className={`${classes.borderBottom} ${classes.head}`}>
          <Typography align="left" variant="h3">
            {label}
          </Typography>
        </Grid>
      )}
      <Grid container item xs>
        <Grid
          container
          item
          xs
          justify="center"
          alignItems="center"
          style={{ borderRight: `2px dashed ${theme.palette.background.default}` }}
          className={classes.borderBottom}
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
                {rangeValue[0] !== undefined ? Number(rangeValue[0]) : '--'}
              </Typography>
            </Grid>
            <Grid item>
              <ValueClicker
                referenceKey={`${stateKey}_LOWER`}
                value={rangeValue[0]}
                step={step}
                min={committedMin}
                max={rangeValue[1]}
                onClick={setLowerLimit}
                direction="column"
              />
            </Grid>
          </Grid>
        </Grid>
        <Grid
          container
          item
          xs
          justify="center"
          alignItems="center"
          className={classes.borderBottom}
        >
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
                {rangeValue[1] !== undefined ? Number(rangeValue[1]) : '--'}
              </Typography>
            </Grid>
            <Grid item>
              <ValueClicker
                referenceKey={`${stateKey}_HIGHER`}
                value={rangeValue[1]}
                step={step}
                min={rangeValue[0]}
                max={committedMax}
                onClick={setUpperLimit}
                direction="column"
              />
            </Grid>
          </Grid>
        </Grid>
      </Grid>
      <Grid container item xs alignItems="center">
        <ValueSlider
          disabled={true}
          rangeValues={rangeValue}
          onChange={setRangeValue}
          min={min}
          max={max}
          step={step}
        />
      </Grid>
    </Grid>
  );
};

/**
 * AlarmModal
 *
 * @component A container for displaying AlarmModalContent Component wrapped in a ModalPopup.
 * This component opens a modal popup with ability to set alarm limits for a single parameter (eg: SpO2)
 * It's currently used inside ValueInfo and LogsPage (on pressing the `settings` button)
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const AlarmModal = ({
  label,
  committedMin = 0,
  committedMax = 100,
  updateModalStatus,
  handleCommittedRange,
  onModalClose,
  openModal = false,
  stateKey,
  step,
  labelHeading = false,
  alarmRangeValues = null,
}: Props): JSX.Element => {
  const dispatch = useDispatch();
  /**
   * State to toggle opening Alarm popup
   */
  const [open, setOpen] = React.useState(false);
  /**
   * Function for handling the closing of the modal.
   */
  const handleClose = () => {
    setOpen(false);
    if (onModalClose) {
      onModalClose(false);
    }
  };

  const alarmLimits = useSelector(getAlarmLimitsRequest, shallowEqual);
  const range =
    alarmLimits === null
      ? undefined
      : ((alarmLimits as unknown) as Record<string, Range>)[stateKey];
  const { lower, upper } = range === undefined ? Range.fromJSON({ lower: NaN, upper: NaN }) : range;
  const [rangeValue] = React.useState<number[]>([
    alarmRangeValues && alarmRangeValues.lower !== 0 ? alarmRangeValues.lower : lower,
    alarmRangeValues && alarmRangeValues.upper !== 0 ? alarmRangeValues.upper : upper,
  ]);

  /**
   * This callback updates the value of the current model status(open/close) to parent component
   */
  const initSetValue = useCallback(() => {
    setOpen(openModal);
  }, [openModal]);

  /**
   * Sets openModal props value to update current modal status(open/close) value received from parent component
   */
  useEffect(() => {
    initSetValue();
  }, [initSetValue]);

  useEffect(() => {
    if (updateModalStatus) {
      updateModalStatus(open);
    }
  });

  /**
   * Function for opening a modal to confirm the changes.
   */
  const handleConfirm = () => {
    const update = {
      [stateKey]: {
        lower: rangeValue[0],
        upper: rangeValue[1],
      },
    };
    dispatch(commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, update));
    handleCommittedRange(rangeValue[0], rangeValue[1]);
    handleClose();
  };

  return (
    <Grid container direction="column" alignItems="center" justify="center">
      <ModalPopup
        withAction={true}
        label={`${label} - Alarm`}
        open={open}
        onClose={handleClose}
        onConfirm={handleConfirm}
      >
        <AlarmModalContent
          label={label}
          committedMin={committedMin}
          committedMax={committedMax}
          handleCommittedRange={handleCommittedRange}
          stateKey={stateKey}
          step={step}
          labelHeading={labelHeading}
          alarmRangeValues={alarmRangeValues}
        />
      </ModalPopup>
    </Grid>
  );
};
