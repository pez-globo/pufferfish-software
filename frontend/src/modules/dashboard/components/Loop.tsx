import { useTheme } from '@material-ui/core';
import { curveLinear } from '@vx/curve';
import { Group } from '@vx/group';
import { scaleLinear, scaleTime } from '@vx/scale';
import { LinePath } from '@vx/shape';
import React from 'react';
import { Axes } from './Axes';

export interface Point {
  x: number;
  y: number;
}

// accessors
const x = ({ x }: Point) => x;
const y = ({ y }: Point) => y;

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

  // scales
  const xScale = scaleTime({
    domain: [xRangeMin, xRangeMax],
    range: [0, xMax],
  });
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
