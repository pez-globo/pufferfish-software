/**
 * @summary A Re-usable component for Setting Parameter Value in a Modal popup
 */
import React, { useCallback, useEffect, useRef } from 'react';
import { makeStyles, Theme, Grid, Typography } from '@material-ui/core';
import { shallowEqual, useSelector } from 'react-redux';
import { getRotaryEncoder } from '../../store/controller/selectors';
import RotaryEncodeController from '../shared/RotaryEncodeController';
import ValueClicker from './ValueClicker';

const useStyles = makeStyles((theme: Theme) => ({
  contentContainer: {
    minHeight: '290px',
    border: `2px dashed ${theme.palette.background.default}`,
    borderRadius: theme.panel.borderRadius,
    margin: '0px 0px 10px',
    paddingTop: theme.spacing(2),
    paddingLeft: theme.spacing(2),
    paddingRight: theme.spacing(4),
  },
  borderBottom: {
    borderBottom: `2px dashed ${theme.palette.background.default}`,
  },
  openButton: {
    width: '100%',
    border: `1px solid ${theme.palette.common.black}`,
    padding: '5px 16px',
  },
  unitsLabel: {
    paddingTop: theme.spacing(4),
    paddingLeft: theme.spacing(2),
    opacity: 0.8,
  },
}));

/**
 * @typedef ContentProps
 *
 * Props interface for the setting value
 *
 * @prop {string} label Label for the value
 * @prop {string} units Unit measurement of the value
 * @prop {number} committedSetting current value
 * @prop {boolean} disableSetNewButton Configuration to show/hide `Set New` outside modal
 * @prop {function} requestCommitSetting Callback when value changes
 * @prop {function} updateModalStatus Callback when modal status changes
 * @prop {boolean} openModal Configure Modal open/close status
 * @prop {number} min Minimum under which value cannot decrement
 * @prop {number} max Maximum above which value cannot increment
 *
 */
interface ContentProps {
  label: string;
  units?: string;
  committedSetting: number;
  requestCommitSetting(setting: number): void;
  updateModalStatus?(status: boolean): void;
  openModal?: boolean;
  min?: number;
  max?: number;
}

/**
 * ValueModal
 *
 * @component A container that displays the values of Parameters like FiO2/Flow
 * and provides ValueClickers to adjust them.
 * This component is not wrapped in a Modal Popup and is currently used inside
 * the multistep modal popup(MultiStepWizard).
 * It does not provide capability to adjust the alarm limits for these Parameters, which is provided by AlarmModal.
 *
 * Uses the [[ContentProps]] interface
 * @returns JSX.Element
 */
const ValueModalContent = ({
  label,
  units,
  committedSetting,
  openModal = false,
  updateModalStatus,
  requestCommitSetting,
  min = 0,
  max = 100,
}: ContentProps): JSX.Element => {
  const classes = useStyles();
  const isInitialMount = useRef(true);
  const rotaryEncoder = useSelector(getRotaryEncoder, shallowEqual);
  /**
   * State to manage modal status
   */
  const [open, setOpen] = React.useState(openModal);
  /**
   * Initalization to set default value & modal status
   */
  const [value, setValue] = React.useState(committedSetting);
  /**
   * State to manage rotary encoder acitvity
   */
  const [isRotaryActive, setIsRotaryActive] = React.useState(false);

  /**
   * Triggers whenever openModal status updates
   */
  useEffect(() => {
    setOpen(openModal);
    return () => {
      setOpen(false);
    };
  }, [openModal]);
  /**
   * Initalization to set default value & modal status
   */
  const initSetValue = useCallback(() => {
    setValue(committedSetting >= min ? committedSetting : min);
  }, [committedSetting, min]);

  useEffect(() => {
    initSetValue();
  }, [initSetValue]);
  /**
   * Triggers callback when modal status changes
   */
  useEffect(() => {
    if (updateModalStatus) {
      updateModalStatus(open);
    }
  });

  useEffect(() => {
    requestCommitSetting(value);
  }, [requestCommitSetting, value]);

  // const handleConfirm = () => {
  //   requestCommitSetting(value);
  // };

  /**
   * Triggers whenever rotaryEncoder value in redux store is updated.
   */
  useEffect(() => {
    // Disable on initial mount since multi step dialog will run everytime when dialog opens
    if (isInitialMount.current) {
      isInitialMount.current = false;
    } else {
      setIsRotaryActive(true);
    }
  }, [rotaryEncoder]);

  /**
   * Function for handling label changed for PIP.
   *
   * @param {string} label - desc for label
   *
   * @returns empty string
   */
  function pipClarify(label: string) {
    if (label === 'PIP') return '*not PEEP compensated';
    return '';
  }

  const modalContent = (
    <Grid container direction="row">
      <Grid container item xs direction="column" className={classes.contentContainer}>
        <Grid item>
          <Typography variant="h4">
            {label}
            <Typography variant="h6" style={{ opacity: 0.8 }}>
              {pipClarify(label)}
            </Typography>
          </Typography>
        </Grid>
        <Grid container item xs wrap="nowrap">
          <Grid container item alignItems="baseline">
            <Typography align="left" style={{ fontSize: '9.5rem' }}>
              {value.toFixed(0)}
            </Typography>
            <Typography align="center" variant="h5" className={classes.unitsLabel}>
              {units}
            </Typography>
          </Grid>
        </Grid>
      </Grid>
      <Grid item>
        <ValueClicker referenceKey="" value={value} min={min} max={max} onClick={setValue} />
      </Grid>
    </Grid>
  );

  return (
    <Grid container direction="column" alignItems="center" justify="center">
      {rotaryEncoder !== null ? (
        <RotaryEncodeController
          isActive={isRotaryActive}
          value={value}
          onClick={(num: number) => {
            setValue(num);
          }}
          min={min}
          max={max}
        />
      ) : null}
      <Grid container direction="column" alignItems="center" justify="center">
        {modalContent}
      </Grid>
    </Grid>
  );
};

export default ValueModalContent;
