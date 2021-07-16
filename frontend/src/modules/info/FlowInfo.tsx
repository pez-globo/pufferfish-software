/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for Flow parameter
 *
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ParametersRequest } from '../../store/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { commitRequest, commitDraftRequest } from '../../store/controller/actions';
import { getParametersFlow, getSensorMeasurementsFlow } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { SettingAdjustProps, ValueModal } from '../controllers/ValueModal';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { LMIN } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getSensorMeasurementsFlow,
});
const FlowDisplay = connect(displaySelector)(ValueDisplay);

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersFlow,
});
const FlowValueModal = connect(settingSelector)(ValueModal);

const label = 'Flow';
const stateKey = 'flow';
const units = LMIN;

/**
 * FlowInfo
 *
 * @component A `Knob`-based component for handling Flow information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const FlowInfo = (): JSX.Element => {
  const dispatch = useDispatch();

  /**
   * Sets the flow
   *
   * @param {number} setting - desc for setting
   */
  const doSetFlow = (setting: number) => {
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { flow: setting }),
    );
    dispatch(
      commitDraftRequest<ParametersRequest>(MessageType.ParametersRequest, { flow: setting }),
    );
  };
  return (
    <Knob
      valueDisplay={<FlowDisplay label={label} units={units} isLive={true} />}
      valueModal={
        <FlowValueModal label={label} units={units} requestCommitSetting={doSetFlow} max={80} />
      }
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

export default FlowInfo;
