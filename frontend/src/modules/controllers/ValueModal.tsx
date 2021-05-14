/**
 * @summary Re-usable wrapper components to display & control value
 *
 */
import React, { useCallback, useEffect, useRef } from 'react';
import { makeStyles, Theme, Grid, Button, Typography } from '@material-ui/core';
import { shallowEqual, useSelector } from 'react-redux';
import ValueClicker from './ValueClicker';
import ModalPopup from './ModalPopup';
import { getRotaryEncoder } from '../../store/controller/selectors';

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
 * @typedef SettingAdjustProps
 *
 * Interface for the adjusting props settings
 *
 * @prop {number} committedSetting updated value
 */
export interface SettingAdjustProps {
  committedSetting: number;
}

/**
 * @deprecated
 * @typedef Props
 *
 * Props interface for the value model
 *
 * @prop {string} label desc for label
 * @prop {string} units desc for units
 * @prop {number} committedSetting current value
 * @prop {boolean} disableSetNewButton Configuration to show/hide `Set New` outside modal
 * @prop {function} requestCommitSetting Callback when value changes
 * @prop {function} updateModalStatus Callback when modal status changes
 * @prop {boolean} openModal Configure Modal open/close status
 * @prop {number} min Minimum under which value cannot decrement
 * @prop {number} max Maximum above which value cannot increment
 *
 */
interface Props {
  label: string;
  units?: string;
  committedSetting: number;
  disableSetNewButton?: boolean;
  requestCommitSetting(setting: number): void;
  updateModalStatus?(status: boolean): void;
  openModal?: boolean;
  min?: number;
  max?: number;
}

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
 * @component A container for displaying value modal settings.
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
export const ValueModal = ({
  label,
  units,
  committedSetting,
  disableSetNewButton = false,
  openModal = false,
  updateModalStatus,
  requestCommitSetting,
  min = 0,
  max = 100,
}: Props): JSX.Element => {
  const classes = useStyles();
  const rotaryEncoder = useSelector(getRotaryEncoder, shallowEqual);
  /** 
   * State to manage modal status
   */
  const [open, setOpen] = React.useState(false);
  /** 
   * State to manage value
   */
  const [value, setValue] = React.useState(committedSetting);
  /** 
   * Initalization to set default value & modal status
   */
  const initSetValue = useCallback(() => {
    setValue(committedSetting >= min ? committedSetting : min);
    setOpen(openModal);
  }, [committedSetting, openModal, min]);

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
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [open]);

  /**
   * Function for handling opening of the model.
   */
  const handleOpen = () => {
    setOpen(true);
  };
  /**
   * Function for handling closing of the model.
   */
  const handleClose = () => {
    setOpen(false);
  };
  /**
   * Function for handling confirmation of the model.
   */
  const handleConfirm = () => {
    requestCommitSetting(value);
    setOpen(false);
  };

  /**
   * Updates the value using Rotary encoder.
   */
  const updateRotaryData = useCallback(
    () => {
      if (rotaryEncoder === null) {
        return;
      }

      if (!open) {
        return;
      }

      const stepDiff = rotaryEncoder.stepDiff || 0;
      const valueClone = value >= min ? value : min;
      const newValue = valueClone + stepDiff;
      if (newValue < min) {
        setValue(min);
      } else if (newValue > max) {
        setValue(max);
      } else {
        setValue(newValue);
      }
      if (rotaryEncoder.buttonPressed) {
        handleConfirm();
      }
    },
    // eslint-disable-next-line react-hooks/exhaustive-deps
    [rotaryEncoder, min, max],
  );

  useEffect(() => {
    updateRotaryData();
  }, [updateRotaryData]);

  /**
   * Function for handling label changed for PIP.
   *
   * @param {string} label - Value label
   *
   * @returns string
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
      <Grid container item xs>
        {!disableSetNewButton && (
          <Button
            onClick={handleOpen}
            color="primary"
            variant="contained"
            className={classes.openButton}
          >
            Set New
          </Button>
        )}
      </Grid>
      <ModalPopup
        withAction={true}
        label="Set New"
        open={open}
        onClose={handleClose}
        onConfirm={handleConfirm}
      >
        {modalContent}
      </ModalPopup>
    </Grid>
  );
};

/**
 * SetValueContent
 *
 * @component A container for setting the value of modal settings in multistep popup.
 *
 * Uses the [[ContentProps]] interface
 * @returns JSX.Element
 */

export const SetValueContent = ({
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
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [open]);

  useEffect(() => {
    requestCommitSetting(value);
  }, [requestCommitSetting, value]);

  /**
   * Function for handling the request commit setting.
   */
  const handleConfirm = () => {
    requestCommitSetting(value);
  };
  /**
   * Updates the value using Rotary encoder.
   */
  const updateRotaryData = useCallback(
    () => {
      if (rotaryEncoder === null) {
        return;
      }

      if (!open || Number.isNaN(rotaryEncoder.stepDiff)) {
        return;
      }

      const stepDiff = rotaryEncoder.stepDiff || 0;
      const valueClone = value >= min ? value : min;
      const newValue = valueClone + stepDiff;
      if (newValue < min) {
        setValue(min);
      } else if (newValue > max) {
        setValue(max);
      } else {
        setValue(newValue);
      }
      if (rotaryEncoder.buttonPressed) {
        handleConfirm();
      }
    },
    // eslint-disable-next-line react-hooks/exhaustive-deps
    [rotaryEncoder, min, max],
  );
  /**
   * Triggers whenever rotaryEncoder value in redux store is updated.
   */
  useEffect(() => {
    // Disable on initial mount since multi step dialog will run everytime when dialog opens
    if (isInitialMount.current) {
      isInitialMount.current = false;
    } else {
      updateRotaryData();
    }
  }, [updateRotaryData, rotaryEncoder]);

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
      {modalContent}
    </Grid>
  );
};

export default ValueModal;
