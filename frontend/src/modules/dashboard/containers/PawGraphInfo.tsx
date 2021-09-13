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
import { getWaveformPawOld, getWaveformPawNewSegment } from '../../../store/plots/selectors';
import { Waveform } from '../components/Waveform';
import { Axes } from '../components/Axes';

/**
 * @typedef DataProps
 *
 * some description.
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
  data: getWaveformPawOld,
});

/**
 * Function for handling segement selection
 *
 * @param {number} segmentIndex Index of the segment
 *
 */
const newSegmentSelector = (segmentIndex: number) =>
  createStructuredSelector<StoreState, DataProps>({
    data: getWaveformPawNewSegment(segmentIndex),
  });

/**
 * @typedef AutoSizerProps
 *
 * Props interface for setting graph height and width.
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
 * PawWaveform
 *
 * @component Component for showing waveform information.
 *
 * Uses the [[WaveformProps]] interface
 *
 * @returns {JSX.Element}
 *
 */
const PawWaveform = ({ data, width, height, strokeWidth, fill }: WaveformProps) => (
  <Waveform
    type="paw"
    fill={fill}
    width={width}
    height={height}
    data={data}
    strokeWidth={strokeWidth}
    xRangeMax={10000}
    yRangeMin={0}
    yRangeMax={60}
  />
);

const WaveformOld = connect(oldSelector)(PawWaveform);
const WaveformNew0 = connect(newSegmentSelector(0))(PawWaveform);
const WaveformNew1 = connect(newSegmentSelector(1))(PawWaveform);
const WaveformNew2 = connect(newSegmentSelector(2))(PawWaveform);
const WaveformNew3 = connect(newSegmentSelector(3))(PawWaveform);

/**
 *
 * function for handling wave form data.
 *
 * Uses the [[AutoSizerProps]] interface
 *
 * @return {JSX.Element}
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
 * PawGraphInfo
 *
 * @component Component for showing graph information.
 *
 * @returns {JSX.Element}
 *
 */
const PawGraphInfo = (): JSX.Element => (
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
          yRangeMax={60}
          title="Paw"
          units="cm H2O"
        />
      );
    }}
  </AutoSizer>
);

export default PawGraphInfo;
