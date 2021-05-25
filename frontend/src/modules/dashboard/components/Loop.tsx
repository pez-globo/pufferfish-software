/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import { useTheme } from '@material-ui/core';
import { curveLinear } from '@vx/curve';
import { Group } from '@vx/group';
import { scaleLinear, scaleTime } from '@vx/scale';
import { LinePath } from '@vx/shape';
import React from 'react';
import { Axes } from './Axes';

/**
 * @typedef Point
 *
 * Interface for defining point.
 *
 * @prop {number} x  x axes point value
 * @prop {number} y y axes point value
 *
 */
export interface Point {
  x: number;
  y: number;
}

// accessors
const x = ({ x }: Point) => x;
const y = ({ y }: Point) => y;

/**
 * @typedef Props
 *
 * Props interface for the data.
 *
 * @prop {number} width desc for width
 * @prop {number} height desc for height
 * @prop {Point[]} data desc for data
 * @prop {number} margin.left desc for margin.left
 * @prop {number} margin.top desc for margin.top
 * @prop {number} margin.bottom desc for margin.bottom
 * @prop {number} margin.right desc for margin.right
 * @prop {number} strokeWidth desc for strokeWidth
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
  data: Point[];
  margin: {
    left: number;
    top: number;
    bottom: number;
    right: number;
  };
  strokeWidth: number;
  xRangeMin: number;
  xRangeMax: number;
  yRangeMin: number;
  yRangeMax: number;
  type: string;
}

/**
 * Loop
 *
 * @component Component for showing graph data.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 *
 */
export const Loop = ({
  width,
  height,
  margin,
  data,
  strokeWidth,
  xRangeMin,
  xRangeMax,
  yRangeMin,
  yRangeMax,
}: Props): JSX.Element => {
  const theme = useTheme();

  // bounds
  const xMax = width - margin.left - margin.right;
  const yMax = height - margin.top - margin.bottom;

  /**
   * some description
   */
  const xScale = scaleTime({
    domain: [xRangeMin, xRangeMax],
    range: [0, xMax],
  });

  /**
   * some description
   */
  const yScale = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [yMax, 0],
    nice: true,
  });

  // TODO: we need to remove the exclamation points, which are claiming that the
  // values are never undefined
  return (
    <svg width={width} height={height}>
      <Group>
        <LinePath
          data={data}
          x={(d) => (xScale(x(d)) as number) + margin.left}
          y={(d) => (yScale(y(d)) as number) + margin.top}
          stroke={theme.palette.info.main}
          strokeWidth={strokeWidth}
          curve={curveLinear}
        />
      </Group>
    </svg>
  );
};

Loop.defaultProps = Axes.defaultProps;
