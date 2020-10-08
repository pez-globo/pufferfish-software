import React from 'react';
import { bindActionCreators } from 'redux';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { ValueModal, SettingAdjustProps } from '../controllers/ValueModal';
import { AlarmModal, Knob } from '../controllers';

import store from '../../store';
import { StoreState } from '../../store/types';
import { getCycleMeasurementsPEEP, getParametersPEEP } from '../../store/controller/selectors';
import { CMH20 } from './units';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { PARAMETER_STANDBY } from '../../store/controller/types';

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
 * A `Knob`-based component for handling PEEP information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 */
const PEEPInfo = (): JSX.Element => {
  const dispatch = useDispatch();
  const doSetPEEP = (setting: number) => {
    dispatch(updateCommittedParameter({ peep: setting }));
    dispatch(updateCommittedState(PARAMETER_STANDBY, { peep: setting }));
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
