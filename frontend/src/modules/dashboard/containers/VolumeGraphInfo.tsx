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
import { AutoSizer } from 'react-virtualized';
import { StoreState } from '../../../store/types';
import { WaveformPoint } from '../../../store/controller/types';
import {
  getWaveformVolumeOld,
  getWaveformVolumeNewSegment,
} from '../../../store/controller/selectors';
import { Waveform } from '../components/Waveform';
import { Axes } from '../components/Axes';

/**
 * @typedef DataProps
 *
 * some description
 *
 * @prop {WaveformPoint[]} data desc for data
 *
 */
interface DataProps {
  data: WaveformPoint[];
}

/**
 * some description.
 */
const oldSelector = createStructuredSelector<StoreState, DataProps>({
  data: getWaveformVolumeOld,
});

/**
 * Function for handling segement selection
 *
 * @param {number} segmentIndex Index of the segment
 *
 */
const newSegmentSelector = (segmentIndex: number) =>
  createStructuredSelector<StoreState, DataProps>({
    data: getWaveformVolumeNewSegment(segmentIndex),
  });

/**
 * @typedef AutoSizerProps
 *
 * Props interface for setting value graph height and width.
 *
 * @prop {number} width width of graph
 * @prop {number} height height of graph
 *
 */
interface AutoSizerProps {
  width: number;
  height: number;
}

/**
 * @typedef WaveformProps
 *
 * Props interface for setting wave form graph with strokeWidth and fill values.
 *
 * @prop {number} strokeWidth strokeWidth of waveform graph
 * @prop {boolean} fill fill of waveform graph
 *
 */
interface WaveformProps extends DataProps, AutoSizerProps {
  strokeWidth: number;
  fill: boolean;
}

/**
 * VolumeWaveform
 *
 * @component Component for showing volume waveform information.
 *
 * Uses the [[WaveformProps]] interface
 *
 * @returns {JSX.Element}
 *
 */
const VolumeWaveform = ({ data, width, height, strokeWidth, fill }: WaveformProps) => (
  <Waveform
    type="volume"
    fill={fill}
    width={width}
    height={height}
    data={data}
    strokeWidth={strokeWidth}
    xRangeMax={10000}
    yRangeMin={0}
    yRangeMax={1000}
  />
);

const WaveformOld = connect(oldSelector)(VolumeWaveform);
const WaveformNew0 = connect(newSegmentSelector(0))(VolumeWaveform);
const WaveformNew1 = connect(newSegmentSelector(1))(VolumeWaveform);
const WaveformNew2 = connect(newSegmentSelector(2))(VolumeWaveform);
const WaveformNew3 = connect(newSegmentSelector(3))(VolumeWaveform);

/**
 *
 * function for handling wave form data.
 *
 * Uses the [[AutoSizerProps]] interface
 *
 * @returns {JSX.Element}
 *
 */
const waveforms = ({ width, height }: AutoSizerProps) => (
  <React.Fragment>
    <WaveformOld width={width} height={height} strokeWidth={1} fill={false} />,
    <WaveformNew0 width={width} height={height} strokeWidth={4} fill={true} />
    <WaveformNew1 width={width} height={height} strokeWidth={4} fill={true} />
    <WaveformNew2 width={width} height={height} strokeWidth={4} fill={true} />
    <WaveformNew3 width={width} height={height} strokeWidth={4} fill={true} />
  </React.Fragment>
);

/**
 * VolumeGraphInfo
 *
 * @component Component for showing volume graph information.
 *
 * @returns {JSX.Element}
 *
 */
const VolumeGraphInfo = (): JSX.Element => (
  <AutoSizer>
    {(props: AutoSizerProps) => {
      const { width, height } = props;
      return (
        <Axes
          width={width}
          height={height}
          waveforms={waveforms({ width, height })}
          xRangeMax={10000}
          xRangeRescale={1 / 1000}
          yRangeMin={0}
          yRangeMax={1000}
          title="Volume"
          units="mL"
        />
      );
    }}
  </AutoSizer>
);

export default VolumeGraphInfo;
