/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import React from 'react';
import { useTheme } from '@material-ui/core';
import { fade } from '@material-ui/core/styles/colorManipulator';
import { curveLinear } from '@vx/curve';
import { LinePath, AreaClosed } from '@vx/shape';
import { Group } from '@vx/group';
import { LinearGradient } from '@vx/gradient';
import { scaleLinear } from '@vx/scale';
import { Axes } from './Axes';

/**
 * @typedef DateValue
 *
 * Interface for defining date value.
 *
 * @prop {number} date  display date in x axes
 * @prop {number} value display value in y axes
 *
 */
interface DateValue {
  date: number;
  value: number;
}

/**
 * @typedef Props
 *
 * Props interface for Waveform component.
 *
 * @prop {number} width desc for width
 * @prop {number} height desc for height
 * @prop {DateValue[]} data desc for data
 * @prop {number} margin.left desc for margin.left
 * @prop {number} margin.top desc for margin.top
 * @prop {number} margin.bottom desc for margin.bottom
 * @prop {number} margin.right desc for margin.right
 * @prop {number} strokeWidth desc for strokeWidth
 * @prop {boolean} fill desc for fill
 * @prop {number} xRangeMin desc for xRangeMin
 * @prop {number} xRangeMax desc for xRangeMax
 * @prop {number} yRangeMin desc for yRangeMin
 * @prop {number} yRangeMax desc for yRangeMax
 * @prop {string} type desc for type
 *
 */
export interface Props {
  width: number;
  height: number;
  data: DateValue[];
  margin: {
    left: number;
    top: number;
    bottom: number;
    right: number;
  };
  strokeWidth: number;
  fill: boolean;
  xRangeMin: number;
  xRangeMax: number;
  yRangeMin: number;
  yRangeMax: number;
  type: string;
}

/**
 * Waveform
 *
 * @component Component for showing wave form data.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 *
 */
export const Waveform = ({
  width,
  height,
  margin,
  data,
  strokeWidth,
  fill,
  xRangeMin,
  xRangeMax,
  yRangeMin,
  yRangeMax,
  type,
}: Props): JSX.Element => {
  const theme = useTheme();

  // bounds
  const xMin = margin.left;
  const xMax = width - margin.right;
  const yMin = margin.top;
  const yMax = height - margin.bottom;

  /**
   * some description
   */
  const xScale = scaleLinear({
    domain: [xRangeMin, xRangeMax],
    range: [xMin, xMax],
  });

  /**
   * some description
   */
  const yScale = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [yMax, yMin],
    nice: true,
  });

  /**
   * some description
   */
  const yScale1 = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [height - margin.bottom, 0],
  });

  /**
   * some description
   */
  const yScale2 = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [(height - margin.bottom + margin.top) / 2, 0],
  });

  /**
   * function for handling responsive utils for axis ticks
   *
   * @param {string} type type of wave data.
   *
   * @returns {} some description
   *
   */
  function findAxis(type: string) {
    if (type === 'flow') return yScale2;
    if (type === 'paw' || type === 'volume') return yScale1;
    return yScale;
  }

  /**
   * some description
   *
   * @param {boolean} fill add color based on wave form type
   *
   * @returns {} some description
   *
   */
  function fillF(fill: boolean) {
    if (fill) return fade(theme.palette.info.main, 0.5);
    return 'rgba(0,0,0,0)';
  }

  /**
   * function for handling x axes value accessor
   *
   * Uses the [[DateValue]] interface
   *
   * @param {DateValue} date desc for date
   *
   * @returns {number}
   *
   */
  const xAccessor = ({ date }: DateValue): number => xScale(date) as number;

  /**
   * function for handling  y axes value accessor
   *
   * Uses the [[DateValue]] interface
   *
   * @param {DateValue} value desc for value
   *
   * @returns {number}
   *
   */
  const yAccessor = ({ value }: DateValue): number => yScale(value) as number;

  return (
    <svg width={width} height={height}>
      <LinearGradient
        from={theme.palette.info.main}
        to={theme.palette.info.main}
        toOpacity={0}
        id="gradient"
      />
      <Group>
        <AreaClosed
          data={data}
          x={xAccessor}
          y={yAccessor}
          yScale={findAxis(type)}
          fill={fillF(fill)}
          strokeWidth={strokeWidth}
          curve={curveLinear}
        />
        <LinePath
          data={data}
          x={xAccessor}
          y={yAccessor}
          stroke={theme.palette.info.main}
          strokeWidth={strokeWidth}
          curve={curveLinear}
        />
      </Group>
    </svg>
  );
};

Waveform.defaultProps = Axes.defaultProps;
