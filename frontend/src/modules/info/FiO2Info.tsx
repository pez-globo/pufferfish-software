import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { updateCommittedParameter, updateCommittedState } from '../../store/controller/actions';
import { getParametersFiO2, getSensorMeasurementsFiO2 } from '../../store/controller/selectors';
import { PARAMETER_STANDBY } from '../../store/controller/types';
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
 * A `Knob`-based component for handling FiO2 information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 */
const FiO2Info = (): JSX.Element => {
  const dispatch = useDispatch();
  const doSetFiO2 = (setting: number) => {
    dispatch(updateCommittedParameter({ fio2: setting }));
    dispatch(updateCommittedState(PARAMETER_STANDBY, { fio2: setting }));
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
          requestCommitRange={() => null}
        />
      }
    />
  );
};

export default FiO2Info;
