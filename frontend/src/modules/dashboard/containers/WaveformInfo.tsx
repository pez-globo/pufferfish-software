/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import React from 'react';
import { connect } from 'react-redux';
import { createStructuredSelector } from 'reselect';
import { Grid, Typography } from '@material-ui/core';
import { StoreState } from '../../../store/types';
import {
  getSensorMeasurementsTime,
  getSensorMeasurementsPaw,
  getSensorMeasurementsFlow,
  getSensorMeasurementsVolume,
} from '../../../store/controller/selectors';

/**
 * @typedef DataProps
 *
 * some description.
 *
 * @prop {number} time desc for time
 * @prop {number} paw desc for paw
 * @prop {number} flow desc for flow
 * @prop {number} volume desc for volume
 *
 */
interface Props {
  time: number;
  paw: number;
  flow: number;
  volume: number;
}

/**
 * WaveformInfo
 *
 * @component Component for showing waveform information.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 *
 */
export const WaveformInfo = ({ time, paw, flow, volume }: Props): JSX.Element => (
  <Grid container item direction="column" alignItems="center" justify="space-evenly">
    <>
      <Grid item xs={3}>
        <Typography>{time}</Typography>
      </Grid>
      <Grid item xs={3}>
        <Typography>{paw}</Typography>
      </Grid>
      <Grid item xs={3}>
        <Typography>{flow}</Typography>
      </Grid>
      <Grid item xs={3}>
        <Typography>{volume}</Typography>
      </Grid>
    </>
  </Grid>
);

/**
 * some description
 */
const selector = createStructuredSelector<StoreState, Props>({
  time: getSensorMeasurementsTime,
  paw: getSensorMeasurementsPaw,
  flow: getSensorMeasurementsFlow,
  volume: getSensorMeasurementsVolume,
});

export default connect(selector)(WaveformInfo);
