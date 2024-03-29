import { makeStyles, Theme } from '@material-ui/core';
import React, { useState, useEffect } from 'react';
import { useSelector } from 'react-redux';
import { createSelector } from 'reselect';
import { getController } from '../../../store/controller/selectors';
import { ControllerStates } from '../../../store/controller/types';
import { MCUPowerStatus } from '../../../store/proto/mcu_pb';
import Power25Icon from './icons/Power25Icon';
import Power50Icon from './icons/Power50Icon';
import Power75Icon from './icons/Power75Icon';
import PowerChargingIcon from './icons/PowerChargingIcon';
import PowerFullIcon from './icons/PowerFullIcon';
import { PERCENT } from '../../shared/units';

const useStyles = makeStyles((theme: Theme) => ({
  paddingRight: {
    paddingRight: theme.spacing(1),
  },
}));

// selectors to get Battery power
const getMcuPowerStatus = createSelector(
  getController,
  (states: ControllerStates): MCUPowerStatus | null => states.mcuPowerStatus,
);
const getBatteryPowerLeft = createSelector(
  getMcuPowerStatus,
  (mcuPowerStatus: MCUPowerStatus | null): number =>
    mcuPowerStatus === null ? 0 : mcuPowerStatus.powerLeft,
);
const getChargingStatus = createSelector(
  getMcuPowerStatus,
  (mcuPowerStatus: MCUPowerStatus | null): boolean =>
    mcuPowerStatus === null ? false : mcuPowerStatus.charging,
);

/**
 * PowerIndicator
 *
 * @component  A container for displaying the battery and power management status of the ventilator.
 *
 * @returns {JSX.Element} battery percentage remain
 */
const PowerIndicator = (): JSX.Element => {
  const classes = useStyles();
  const batteryPower = useSelector(getBatteryPowerLeft);
  const chargingStatus = useSelector(getChargingStatus);
  const [icon, setIcon] = useState(<PowerFullIcon style={{ fontSize: '2.5rem' }} />);
  /**
   * Updates Icons based on batteryPower/chargingStatus
   */
  useEffect(() => {
    if (batteryPower >= 0 && batteryPower <= 25) {
      setIcon(<Power25Icon style={{ fontSize: '2.5rem' }} />);
    } else if (batteryPower > 25 && batteryPower <= 50) {
      setIcon(<Power50Icon style={{ fontSize: '2.5rem' }} />);
    } else if (batteryPower > 50 && batteryPower <= 75) {
      setIcon(<Power75Icon style={{ fontSize: '2.5rem' }} />);
    } else {
      setIcon(<PowerFullIcon style={{ fontSize: '2.5rem' }} />);
    }

    if (chargingStatus) {
      setIcon(<PowerChargingIcon style={{ fontSize: '2.5rem' }} />);
    }
  }, [batteryPower, chargingStatus]);

  return (
    <React.Fragment>
      <span className={classes.paddingRight}>{`${
        batteryPower !== undefined ? batteryPower.toFixed(0) : '--'
      }${PERCENT}`}</span>
      {icon}
    </React.Fragment>
  );
};
export default PowerIndicator;
