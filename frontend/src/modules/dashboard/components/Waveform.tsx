import React from 'react';
import { useTheme } from '@material-ui/core';
import { fade } from '@material-ui/core/styles/colorManipulator';
import { curveLinear } from '@vx/curve';
import { LinePath, AreaClosed } from '@vx/shape';
import { Group } from '@vx/group';
import { LinearGradient } from '@vx/gradient';
import { scaleLinear } from '@vx/scale';
import { Axes } from './Axes';

interface DateValue {
  date: number;
  value: number;
}

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

  // scales
  const xScale = scaleLinear({
    domain: [xRangeMin, xRangeMax],
    range: [xMin, xMax],
  });
  const yScale = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [yMax, yMin],
    nice: true,
  });
  const yScale1 = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [height - margin.bottom, 0],
  });
  const yScale2 = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [(height - margin.bottom + margin.top) / 2, 0],
  });
  function findAxis(type: string) {
    if (type === 'flow') return yScale2;
    if (type === 'paw' || type === 'volume') return yScale1;
    return yScale;
  }
  function fillF(fill: boolean) {
    if (fill) return fade(theme.palette.info.main, 0.5);
    return 'rgba(0,0,0,0)';
  }

  const xAccessor = ({ date }: DateValue): number => xScale(date) as number;
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
