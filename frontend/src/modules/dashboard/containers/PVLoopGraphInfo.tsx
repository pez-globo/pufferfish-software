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
import { PVPoint } from '../../../store/controller/types';
import { getPVLoop } from '../../../store/plots/selectors';
import { Loop, Point } from '../components/Loop';
import { Axes } from '../components/Axes';

/**
 * @typedef DataProps
 *
 * some description
 *
 * @prop {PVPoint[]} data desc for data
 *
 */
interface DataProps {
  data: PVPoint[];
}

/**
 * some description.
 */
const selector = createStructuredSelector<StoreState, DataProps>({
  data: getPVLoop,
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
 *
 */
interface WaveformProps extends DataProps, AutoSizerProps {
  strokeWidth: number;
}

/**
 *
 * function for setting PV point.
 *
 * @param {PVPoint} data desc of data
 *
 * @returns {Point}
 *
 */
const pointMapper = (data: PVPoint): Point => ({
  x: data.pressure,
  y: data.volume,
});

/**
 *
 * function for setting wave form graph with strokeWidth and fill values.
 *
 * Uses the [[WaveformProps]] interface
 *
 * @returns {JSX.Element}
 */
const PVLoop = ({ data, width, height, strokeWidth }: WaveformProps) => (
  <Loop
    type="pv"
    width={width}
    height={height}
    data={data.map(pointMapper)}
    strokeWidth={strokeWidth}
    xRangeMin={0}
    xRangeMax={60}
    yRangeMin={0}
    yRangeMax={1000}
  />
);

const CurrentLoop = connect(selector)(PVLoop);

/**
 *
 * some description.
 *
 * Uses the [[AutoSizerProps]] interface
 *
 * @returns {JSX.Element}
 *
 */
const loops = ({ width, height }: AutoSizerProps) => (
  <React.Fragment>
    <CurrentLoop width={width} height={height} strokeWidth={4} />,
  </React.Fragment>
);

/**
 * PVLoopGraphInfo
 *
 * @component Component for showing Pressure volume graph information.
 *
 * @returns {JSX.Element}
 *
 */
const PVLoopGraphInfo = (): JSX.Element => (
  <AutoSizer>
    {(props: AutoSizerProps) => {
      const { width, height } = props;
      return (
        <Axes
          width={width}
          height={height}
          waveforms={loops({ width, height })}
          xRangeMin={0}
          xRangeMax={60}
          xRangeRescale={1}
          yRangeMin={0}
          yRangeMax={1000}
          title="PV"
          units="cm H2O"
        />
      );
    }}
  </AutoSizer>
);

export default PVLoopGraphInfo;
