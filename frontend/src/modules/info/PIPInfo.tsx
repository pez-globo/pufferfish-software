import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { getCycleMeasurementsPIP, getParametersPIP } from '../../store/controller/selectors';
import { PARAMETER_STANDBY } from '../../store/controller/types';
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
