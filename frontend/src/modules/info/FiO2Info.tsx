/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for FiO2 parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ParametersRequest } from '../../store/controller/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import { getParametersFiO2, getSensorMeasurementsFiO2 } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { SettingAdjustProps, ValueModal } from '../controllers/ValueModal';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { PERCENT } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getSensorMeasurementsFiO2,
});
const FiO2Display = connect(displaySelector)(ValueDisplay);

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersFiO2,
});
const FiO2ValueModal = connect(settingSelector)(ValueModal);

const label = 'FiO2';
const stateKey = 'fio2';
const units = PERCENT;

/**
 * FiO2Info
 *
 * @component A `Knob`-based component for handling FiO2 information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const FiO2Info = (): JSX.Element => {
  const dispatch = useDispatch();

  /**
   * sets FiO2
   *
   * @param {number} setting - some desc for setting
   *
   */
  const doSetFiO2 = (setting: number) => {
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { fio2: setting }),
    );
    dispatch(
      commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, { fio2: setting }),
    );
  };
  return (
    <Knob
      valueDisplay={<FiO2Display label={label} units={units} isLive={true} />}
      valueModal={
        <FiO2ValueModal label={label} units={units} requestCommitSetting={doSetFiO2} min={21} />
      }
      alarmModal={
        <AlarmModal
          label={label}
          units={units}
          stateKey={stateKey}
          getCommittedRange={() => null}
        />
      }
    />
  );
};

export default FiO2Info;
