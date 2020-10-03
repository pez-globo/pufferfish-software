import React from 'react';
import { bindActionCreators } from 'redux';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { ValueModal, SettingAdjustProps } from '../controllers/ValueModal';
import { AlarmModal, Knob } from '../controllers';
import store from '../../store';
import { StoreState } from '../../store/types';
import { getCycleMeasurementsPIP, getParametersPIP } from '../../store/controller/selectors';
import { CMH20 } from './units';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { PARAMETER_STANDBY } from '../../store/controller/types';

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
 * A `Knob`-based component for handling PIP information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 */
const PIPInfo = (): JSX.Element => {
  const dispatch = useDispatch();
  const doSetPIP = (setting: number) => {
    dispatch(updateCommittedParameter({ pip: setting }));
    dispatch(updateCommittedState(PARAMETER_STANDBY, { pip: setting }));
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
