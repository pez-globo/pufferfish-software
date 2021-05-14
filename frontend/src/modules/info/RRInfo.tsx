/**
 * @deprecated
 * @summary Component to display Value, Set Value Modal & Alarm Modal for RRInfo parameter
 * 
 * Set Value & Alarm Modal is optional
 *
 */
import React from 'react';
import { connect, useDispatch } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { ParametersRequest } from '../../store/controller/proto/mcu_pb';
import { MessageType } from '../../store/controller/types';
import { commitRequest, commitStandbyRequest } from '../../store/controller/actions';
import { getCycleMeasurementsRR, getParametersRR } from '../../store/controller/selectors';
import { StoreState } from '../../store/types';
import { AlarmModal, Knob } from '../controllers';
import { SettingAdjustProps, ValueModal } from '../controllers/ValueModal';
import { ValueDisplay, ValueProps } from '../displays/ValueDisplay';
import { BMIN } from './units';

const displaySelector = createStructuredSelector<StoreState, ValueProps>({
  value: getCycleMeasurementsRR,
});
const RRDisplay = connect(displaySelector)(ValueDisplay);

const settingSelector = createStructuredSelector<StoreState, SettingAdjustProps>({
  committedSetting: getParametersRR,
});
const RRValueModal = connect(settingSelector)(ValueModal);

const label = 'RR';
const stateKey = 'rr';
const units = BMIN;

/**
 * RRInfo
 *
 * @component A `Knob`-based component for handling RR information.
 *
 * TODO: Hook this component into the redux store with correct selectors.
 *
 * @returns {JSX.Element}
 */
const RRInfo = ({ disableSetValue = false }: { disableSetValue?: boolean }): JSX.Element => {
  const dispatch = useDispatch();

  /**
   * some description
   *
   * @param {number} setting - desc for setting
   */
  const doSetRR = (setting: number) => {
    dispatch(
      commitRequest<ParametersRequest>(MessageType.ParametersRequest, { rr: setting }),
    );
    dispatch(
      commitStandbyRequest<ParametersRequest>(MessageType.ParametersRequest, { rr: setting }),
    );
  };
  return (
    <Knob
      valueDisplay={<RRDisplay label={label} units={units} isLive={true} />}
      valueModal={
        <RRValueModal
          disableSetNewButton={disableSetValue}
          label={label}
          units={units}
          requestCommitSetting={doSetRR}
        />
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

export default RRInfo;
