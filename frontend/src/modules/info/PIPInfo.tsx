/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for PIP parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ParametersRequest } from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/proto/types';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import { getCycleMeasurementsPIP, getParametersPIP } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { SettingAdjustProps, ValueModal } from '../controllers/ValueModal';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { CMH20 } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsPIP,
});
const PIPDisplay = connect(displaySelector)(ValueDisplay);

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersPIP,
});
const PIPValueModal = connect(settingSelector)(ValueModal);

const label = 'PIP';
const stateKey = 'pip';
const units = CMH20;

/**
 * PIPInfo
 *
 * @component A `Knob`-based component for handling PIP information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const PIPInfo = (): JSX.Element => {
  const dispatch = useDispatch();

  /**
   * some description
   *
   * @param {number} setting - desc for setting
   */
  const doSetPIP = (setting: number) => {
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { pip: setting }),
    );
    dispatch(
      commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, { pip: setting }),
    );
  };
  return (
    <Knob
      valueDisplay={<PIPDisplay label={label} units={units} isLive={true} />}
      valueModal={<PIPValueModal label={label} units={units} requestCommitSetting={doSetPIP} />}
      alarmModal={
        <AlarmModal
          label={label}
          units={units}
          stateKey={stateKey}
          requestCommitRange={() => null}
        />
      }
    />
  );
};

export default PIPInfo;
