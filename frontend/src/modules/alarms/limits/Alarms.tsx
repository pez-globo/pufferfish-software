import React from 'react';
import { useSelector } from 'react-redux';
import { getAlarmLimitsRequestDraft } from '../../../store/controller/selectors';
import { AlarmConfiguration } from './AlarmLimitsPage';
import AlarmLimits from './AlarmLimits';

const ParamAlarms = ({
  alarmConfig,
  itemsPerPage,
  page,
}: {
  alarmConfig: AlarmConfiguration[];
  itemsPerPage: number;
  page: number;
}): JSX.Element => {
  const alarmLimitsRequestDraftSelect = useSelector(getAlarmLimitsRequestDraft);

  return (
    <>
      {alarmConfig.slice((page - 1) * itemsPerPage, page * itemsPerPage).map((alarm) => {
        const key = `alarm-config-${alarm.stateKey}`;
        return (
          <AlarmLimits
            key={key}
            label={alarm.label}
            min={alarm.min || 0}
            max={alarm.max || 100}
            stateKey={alarm.stateKey}
            step={alarm.step || 1}
            alarmLimits={alarmLimitsRequestDraftSelect}
            setAlarmLimits={() => null}
          />
        );
      })}
    </>
  );
};
export default ParamAlarms;
