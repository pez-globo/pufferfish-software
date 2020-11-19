import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { getParametersFlow, getSensorMeasurementsFlow } from '../../store/controller/selectors';
import { PARAMETER_STANDBY } from '../../store/controller/types';
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
 * A `Knob`-based component for handling Flow information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 */
const FlowInfo = (): JSX.Element => {
  const dispatch = useDispatch();
  const doSetFlow = (setting: number) => {
    dispatch(updateCommittedParameter({ flow: setting }));
    dispatch(updateCommittedState(PARAMETER_STANDBY, { flow: setting }));
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
