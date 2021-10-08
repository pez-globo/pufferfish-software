import { Button, Grid } from '@material-ui/core';
import React, { useState, useEffect } from 'react';
import { useDispatch, useSelector, shallowEqual, batch } from 'react-redux';
import { useHistory } from 'react-router-dom';
import { createSelector } from 'reselect';
import { getBackendConnected } from '../../../store/connection/selectors';
import { commitRequest, commitDraftRequest } from '../../../store/controller/actions';
import {
  getParametersIsVentilating,
  getVentilatingStatusChanging,
  getParametersRequestMode,
  getParametersRequestDraft,
  getParametersRequest,
} from '../../../store/controller/selectors/measurements';
import {
  getAlarmLimitsRequestUnsaved,
  getFirmwareConnected,
  getAlarmLimitsRequestDraft,
  getAlarmLimitsRequest,
  getAlarmMuteRequest,
  getAlarmMuteStatus,
} from '../../../store/controller/selectors';
import {
  ParametersRequest,
  VentilationMode,
  AlarmLimitsRequest,
  Range,
  AlarmMute,
  AlarmMuteRequest,
} from '../../../store/proto/mcu_pb';
import { MessageType } from '../../../store/proto/types';
import { ModalPopup } from '../../shared';
import { QUICKSTART_ROUTE, DASHBOARD_ROUTE } from '../navigation/constants';
import { DiscardAlarmLimitsContent } from '../../alarms/modal';
import { DISCARD_LIMITS_POPUP } from '../../../store/app/types';
import { getDiscardAlarmLimitsPopupOpen } from '../../../store/app/selectors';

const StartPauseButton = ({ staticStart }: { staticStart?: boolean }): JSX.Element => {
  const dispatch = useDispatch();
  const storeReady = useSelector(getStoreReady);
  const ventilating = useSelector(getParametersIsVentilating);
  const ventilatingStatusChanging = useSelector(getVentilatingStatusChanging);
  const alarmLimitsRequestUnsaved = useSelector(getAlarmLimitsRequestUnsaved);
  const firmwareConnected = useSelector(getFirmwareConnected);
  const backendConnected = useSelector(getBackendConnected);
  const currentMode = useSelector(getParametersRequestMode);
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);
  const alarmLimitsRequestDraft = (alarmLimitsRequestDraftSelect as unknown) as Record<
    string,
    Range
  >;
  const parameterRequestDraft = useSelector(getParametersRequestDraft, shallowEqual);
  const history = useHistory();

  const dispatchParameterRequest = (update: Partial<ParametersRequest>) => {
    dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  /**
   * State to manage ventilation label
   * Label is Dynamic based on ventilation state
   */
  const [label, setLabel] = useState('Start');
  /**
   * State to toggle if Ventilating isDisabled
   */
  const [isDisabled, setIsDisabled] = useState(false);

  /**
   * Updates Ventilation status on clicking Start/Pause ventilation
   */
  const updateVentilationStatus = () => {
    if (ventilating) {
      // if ventilating and there are unsaved alarm limit changes then open modal popup
      if (alarmLimitsRequestUnsaved) {
        dispatch({ type: DISCARD_LIMITS_POPUP, open: true });
        return;
      }
      // if both firmware and backend are connected, and response.ventilating is true
      // then on pressing 'Pause Ventilation' we go back to QuickStart page
      // instead of the page we are on in ventilating mode (eg: settings, alarms screen)
      if (firmwareConnected && backendConnected) history.push(QUICKSTART_ROUTE.path);
    } else {
      initParameterUpdate();
    }
    dispatchParameterRequest({ ventilating: !ventilating });
  };

  /**
   * Update Paramters to redux store when ventilation starts
   */
  const initParameterUpdate = () => {
    if (parameterRequestDraft === null || alarmLimitsRequestDraft === null) {
      return;
    }

    switch (currentMode) {
      case VentilationMode.hfnc:
        batch(() => {
          dispatch(
            commitRequest<ParametersRequest>(MessageType.ParametersRequest, {
              fio2: parameterRequestDraft.fio2,
              flow: parameterRequestDraft.flow,
            }),
          );
          dispatch(
            commitRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, {
              spo2: alarmLimitsRequestDraft.spo2,
              hr: alarmLimitsRequestDraft.hr,
            }),
          );
        });
        break;
      case VentilationMode.pc_ac:
      case VentilationMode.vc_ac:
      case VentilationMode.niv_pc:
      case VentilationMode.niv_ps:
      case VentilationMode.psv:
        dispatchParameterRequest({
          peep: parameterRequestDraft.peep,
          vt: parameterRequestDraft.vt,
          rr: parameterRequestDraft.rr,
          fio2: parameterRequestDraft.fio2,
        });
        break;
      default:
        break;
    }
  };

  /**
   * Enable/Disable and set label for Start/Pause Ventilation button
   */
  useEffect(() => {
    if (ventilating) {
      setIsDisabled(ventilatingStatusChanging);
      setLabel(ventilatingStatusChanging ? 'Pausing...' : 'Pause');
      return;
    }
    setIsDisabled(ventilatingStatusChanging || !firmwareConnected || !backendConnected);
    if (ventilatingStatusChanging) {
      setLabel('Starting...');
      return;
    }
    setLabel(!firmwareConnected || !backendConnected ? 'Connecting...' : 'Start');
  }, [ventilatingStatusChanging, backendConnected, firmwareConnected, ventilating]);

  /**
   * Switch to dashboard page if ventilating
   */
  useEffect(() => {
    if (storeReady && ventilating) {
      history.push(DASHBOARD_ROUTE.path);
    }
  }, [ventilating, history, storeReady]);

  const StartPauseVentilationButton = (
    <Button
      style={{ marginRight: 10, marginLeft: 10 }}
      onClick={updateVentilationStatus}
      variant="contained"
      color="secondary"
      disabled={isDisabled}
    >
      {label}
    </Button>
  );

  const LandingPageStartButton = (
    <Button
      onClick={() => history.push(QUICKSTART_ROUTE.path)}
      variant="contained"
      color="secondary"
      disabled={!storeReady}
    >
      {storeReady ? 'Start' : 'Loading...'}
    </Button>
  );

  return (
    <>
      <Grid item>{staticStart ? LandingPageStartButton : StartPauseVentilationButton}</Grid>
    </>
  );
};

export const StartButtonModalPopup = (): JSX.Element => {
  const dispatch = useDispatch();
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);
  const alarmLimitsRequestSelect = useSelector(getAlarmLimitsRequest);
  const alarmLimitsRequest = (alarmLimitsRequestSelect as unknown) as Record<string, Range>;

  const setAlarmLimitsRequestDraft = (data: Partial<AlarmLimitsRequest>) => {
    dispatch(commitDraftRequest<AlarmLimitsRequest>(MessageType.AlarmLimitsRequest, data));
  };
  const dispatchParameterRequest = (update: Partial<ParametersRequest>) => {
    dispatch(commitRequest<ParametersRequest>(MessageType.ParametersRequest, update));
  };

  const discardAlarmLimitsPopup = useSelector(getDiscardAlarmLimitsPopupOpen);

  const [discardOpen, setDiscardOpen] = useState(false);

  useEffect(() => {
    setDiscardOpen(discardAlarmLimitsPopup);
  }, [discardAlarmLimitsPopup]);

  /**
   * onClickHandler for Start ModalPopup 'Confirm' button
   */
  const handleStartDiscardConfirm = () => {
    setAlarmLimitsRequestDraft(alarmLimitsRequest);
    dispatchParameterRequest({ ventilating: !ventilating });
    dispatch({ type: DISCARD_LIMITS_POPUP, open: false });
    history.push(QUICKSTART_ROUTE.path);
  };

  return (
    <ModalPopup
      withAction={true}
      label="Set Alarms"
      open={discardOpen}
      onClose={() => dispatch({ type: DISCARD_LIMITS_POPUP, open: false })}
      onConfirm={handleStartDiscardConfirm}
    >
      <DiscardAlarmLimitsContent />
    </ModalPopup>
  );
};

// Initialization
const getStoreReady = createSelector(
  getParametersRequest,
  getAlarmLimitsRequest,
  getAlarmMuteStatus,
  getAlarmMuteRequest,
  getBackendConnected,
  getFirmwareConnected,
  (
    parametersRequest: ParametersRequest | null,
    alarmLimitsRequest: AlarmLimitsRequest | null,
    alarmMute: AlarmMute | null,
    alarmMuteRequest: AlarmMuteRequest | null,
    backendConnected: boolean,
    firmwareConnected: boolean,
  ): boolean =>
    parametersRequest !== null &&
    alarmLimitsRequest !== null &&
    alarmMute !== null &&
    alarmMuteRequest !== null &&
    backendConnected &&
    firmwareConnected,
);

export default StartPauseButton;
