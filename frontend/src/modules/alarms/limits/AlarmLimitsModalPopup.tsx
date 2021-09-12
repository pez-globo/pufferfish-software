/**
 * @summary A re-usable component for submit and cancel buttons and their modal popups
 */
import { Button, Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useState } from 'react';
import { useDispatch, useSelector } from 'react-redux';
import { commitDraftRequest, commitRequest } from '../../../store/controller/actions';
import {
  getParametersIsVentilating,
  getAlarmLimitsRequestDraft,
  getAlarmLimitsRequest,
  getAlarmLimitsRequestUnsaved,
  getAlarmLimitsUnsavedKeys,
  getParametersRequestMode,
} from '../../../store/controller/selectors';
import { AlarmLimitsRequest, Range } from '../../../store/proto/mcu_pb';
import { MessageType } from '../../../store/proto/types';
import { ModalPopup } from '../../shared';
import { DiscardAlarmLimitsContent } from '../modal';
import { alarmConfiguration } from './AlarmLimitsPage';

const useStyles = makeStyles((theme: Theme) => ({
  applyButton: {
    border: '1px solid black',
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

/**
 * AlarmLimitsModalPopup
 *
 * @component A container that houses the `Submit` and `Cancel` buttons and their respective Modal Popups
 *
 * @returns {JSX.Element}
 */
const AlarmLimitsModalPopup = (): JSX.Element => {
  const classes = useStyles();
  const dispatch = useDispatch();
  const ventilating = useSelector(getParametersIsVentilating);
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsRequestSelect = useSelector(getAlarmLimitsRequest);
  const alarmLimitsRequestUnsaved = useSelector(getAlarmLimitsRequestUnsaved);
  const alarmLimitsUnsavedKeys = useSelector(getAlarmLimitsUnsavedKeys);
  const alarmLimitsRequest = (alarmLimitsRequestSelect as unknown) as Record<string, Range>;
  const alarmLimitsRequestDraft = (alarmLimitsRequestDraftSelect as unknown) as Record<
    string,
    Range
  >;
  const [confirmOpen, setConfirmOpen] = useState(false);
  const [discardOpen, setDiscardOpen] = useState(false);
  const currentMode = useSelector(getParametersRequestMode);
  const alarmConfig = alarmConfiguration(currentMode);

  const handleConfirmOpen = () => {
    setConfirmOpen(true);
  };

  const handleDiscardOpen = () => {
    setDiscardOpen(true);
  };

  /**
   * Closes modal popup
   */
  const handleClose = () => {
    setConfirmOpen(false);
  };

  const handleDiscardClose = () => {
    setDiscardOpen(false);
  };

  /**
   * Function for updating alarm limits
   *
   * @param {Partial<AlarmLimitsRequest>} data - Alarm ranges which to be updated
   */
  const setAlarmLimitsRequestDraft = (data: Partial<AlarmLimitsRequest>) => {
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, data));
  };

  /**
   * Updating the alarm limit request to redux
   */
  const applyChanges = () => {
    if (alarmLimitsRequestDraft === null) {
      return;
    }

    dispatch(
      commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, alarmLimitsRequestDraft),
    );
  };

  /**
   * Closes modal popup & updates the changes
   */
  const handleConfirm = () => {
    setConfirmOpen(false);
    applyChanges();
  };

  const handleDiscardConfirm = () => {
    setDiscardOpen(false);
    if (alarmLimitsRequest !== null) setAlarmLimitsRequestDraft(alarmLimitsRequest);
  };
  return (
    <>
      {ventilating ? (
        <Button
          onClick={handleDiscardOpen}
          color="primary"
          variant="contained"
          className={classes.applyButton}
          disabled={!alarmLimitsRequestUnsaved}
        >
          Cancel
        </Button>
      ) : null}
      {ventilating ? (
        <Button
          onClick={handleConfirmOpen}
          color="secondary"
          variant="contained"
          className={classes.applyButton}
          disabled={!alarmLimitsRequestUnsaved}
        >
          Submit
        </Button>
      ) : null}
      <ModalPopup
        withAction={true}
        label="Set Alarms"
        open={confirmOpen}
        onClose={handleClose}
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
              {alarmConfig.map((param) => {
                if (alarmLimitsRequestDraft !== null) {
                  if (alarmLimitsUnsavedKeys.includes(param.stateKey)) {
                    return (
                      <Typography variant="subtitle1">{`Change ${param.label} alarm limits from [${
                        alarmLimitsRequest[param.stateKey].lower
                      } ${param.units} -
                             ${alarmLimitsRequest[param.stateKey].upper} ${param.units}] to [${
                        alarmLimitsRequestDraft[param.stateKey].lower
                      } ${param.units} -
                                 ${alarmLimitsRequestDraft[param.stateKey].upper} ${
                        param.units
                      }] ?`}</Typography>
                    );
                  }
                }
                return <React.Fragment />;
              })}
            </Grid>
            <Grid item className={classes.marginContent} />
          </Grid>
        </Grid>
      </ModalPopup>
      <ModalPopup
        withAction={true}
        label="Set Alarms"
        open={discardOpen}
        onClose={handleDiscardClose}
        onConfirm={handleDiscardConfirm}
      >
        <DiscardAlarmLimitsContent />
      </ModalPopup>
    </>
  );
};

export default AlarmLimitsModalPopup;
