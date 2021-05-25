/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for PEEP parameter
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
import { getCycleMeasurementsPEEP, getParametersPEEP } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { SettingAdjustProps, ValueModal } from '../controllers/ValueModal';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { CMH20 } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsPEEP,
});
const PEEPDisplay = connect(displaySelector)(ValueDisplay);

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersPEEP,
});
const PEEPValueModal = connect(settingSelector)(ValueModal);

const label = 'PEEP';
const stateKey = 'peep';
const units = CMH20;

/**
 * PEEPInfo
 *
 * @component A `Knob`-based component for handling PEEP information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const PEEPInfo = (): JSX.Element => {
  const dispatch = useDispatch();

  /**
   * some description
   *
   * @param {number} setting - desc for setting
   */
  const doSetPEEP = (setting: number) => {
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { peep: setting }),
    );
    dispatch(
      commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, { peep: setting }),
    );
  };
  return (
    <Knob
      valueDisplay={<PEEPDisplay label={label} units={units} isLive={true} />}
      valueModal={<PEEPValueModal label={label} units={units} requestCommitSetting={doSetPEEP} />}
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

export default PEEPInfo;
