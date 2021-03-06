/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 *
 */
import React from 'react';
import { useTheme } from '@material-ui/core';
import { Grid, GridRows, GridColumns } from '@vx/grid';
import { Group } from '@vx/group';
import { AxisLeft, AxisBottom } from '@vx/axis';
import { Line } from '@vx/shape';
import { scaleLinear } from '@vx/scale';

/**
 * function for handling responsive utils for axis ticks
 *
 * @param {number} height height of the axis
 *
 * @returns {number} it will retuns 3,5 or 10.
 */
function numTicksForHeight(height: number) {
  if (height <= 300) return 3;
  if (height > 300 && height <= 600) return 5;
  return 10;
}

/**
 * some description
 *
 * @param {number} width width of the axis
 *
 * @returns {number} it will retuns 2,5 or 10.
 */
function numTicksForWidth(width: number) {
  if (width <= 300) return 2;
  if (width > 300 && width <= 400) return 5;
  return 10;
}

/**
 * @typedef Props
 *
 * Props interface for the Axes component.
 *
 * @prop {number} width desc for width
 * @prop {number} height desc for height
 * @prop {React.ReactNode} waveforms desc for waveforms
 * @prop {number} margin.left desc for margin.left
 * @prop {number} margin.top desc for margin.top
 * @prop {number} margin.bottom desc for margin.bottom
 * @prop {number} margin.right desc for margin.right
 * @prop {number} xRangeMin desc for xRangeMin
 * @prop {number} xRangeMax desc for xRangeMax
 * @prop {number} xRangeRescale desc for xRangeRescale
 * @prop {number} yRangeMin desc for yRangeMin
 * @prop {number} yRangeMax desc for yRangeMax
 * @prop {string} title desc for title
 * @prop {string} units desc for units
 *
 */
export interface Props {
  width: number;
  height: number;
  waveforms: React.ReactNode;
  margin: {
    left: number;
    top: number;
    bottom: number;
    right: number;
  };
  xRangeMin: number;
  xRangeMax: number;
  xRangeRescale: number;
  yRangeMin: number;
  yRangeMax: number;
  title: string;
  units: string;
}

/**
 * Axes
 *
 * @component Component for showing Axes graph data.
 *
 * Uses the [[Props]] interface
 *
 * @returns {JSX.Element}
 *
 */
export const Axes = ({
  width,
  height,
  margin,
  waveforms,
  xRangeMin,
  xRangeMax,
  xRangeRescale,
  yRangeMin,
  yRangeMax,
  title,
  units,
}: Props): JSX.Element => {
  const theme = useTheme();

  // bounds
  const xMax = width - margin.left - margin.right;
  const yMax = height - margin.top - margin.bottom;

  /**
   * some description
   */
  const xScale = scaleLinear({
    domain: [xRangeMin * xRangeRescale, xRangeMax * xRangeRescale],
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

  return (
    <svg width={width} height={height}>
      <text
        x={margin.left / 3}
        y={margin.top / 2}
        fill={theme.typography.body1.color}
        fontSize={28}
      >
        {title}
      </text>
      <text
        x={margin.left / 3 + (title.length * 15 + 20)}
        y={margin.top / 2}
        fill={theme.typography.body1.color}
        opacity={0.8}
        fontSize={18}
      >
        - {units}
      </text>
      <Grid
        top={margin.top}
        left={margin.left}
        xScale={xScale}
        yScale={yScale}
        stroke={theme.typography.body1.color}
        strokeOpacity={0.21}
        width={xMax}
        height={yMax}
        numTicksRows={numTicksForHeight(height)}
        numTicksColumns={numTicksForWidth(width)}
      />
      <Group>{waveforms}</Group>
      <Group left={margin.left}>
        <GridRows scale={yScale} width={xMax} height={yMax} stroke="rgba(255,255,255,0)" />
        <GridColumns scale={xScale} width={xMax} height={yMax} stroke="rgba(255,255,255,0)" />
        <AxisLeft
          top={margin.top}
          left={0}
          scale={yScale}
          numTicks={numTicksForHeight(height)}
          label="Axis Left Label"
          labelProps={{
            fill: 'rgba(255,255,255,0)',
            textAnchor: 'middle',
            fontSize: 12,
          }}
          stroke="rgba(255,255,255,0)"
          tickStroke="rgba(255,255,255,0)"
          tickLabelProps={() => ({
            fill: theme.typography.body1.color,
            opacity: 0.5,
            textAnchor: 'end',
            fontSize: 10,
            dx: '-0.25em',
            dy: '0.25em',
          })}
          tickComponent={({ formattedValue, ...tickProps }) => (
            <text {...tickProps}>{formattedValue}</text>
          )}
        />
        <AxisBottom
          top={height - margin.bottom}
          left={0}
          scale={xScale}
          numTicks={numTicksForWidth(width)}
          tickLabelProps={() => ({
            fill: theme.typography.body1.color,
            opacity: 0.5,
            fontSize: 14,
            textAnchor: 'start',
          })}
          label="Time"
        >
          {(axis) => {
            const tickLabelSize = 12;
            const tickOffset = -tickLabelSize / 2;
            const tickRotate = 0;
            const tickColor = theme.typography.body1.color;
            const axisCenter = (axis.axisToPoint.x - axis.axisFromPoint.x) / 2;
            return (
              <g className="my-custom-bottom-axis">
                {axis.ticks.map((tick, i) => {
                  const tickX = tick.to.x;
                  const tickY = tick.to.y + tickLabelSize + axis.tickLength + tickOffset;
                  const key = `${tick.value}-${i}`;
                  return (
                    <Group key={`vx-tick-${key}`} className="vx-axis-tick">
                      <Line from={tick.from} to={tick.to} />
                      <text
                        transform={`translate(${tickX}, ${tickY}) rotate(${tickRotate})`}
                        fontSize={tickLabelSize}
                        textAnchor="middle"
                        fill={tickColor}
                        opacity={0.5}
                      >
                        {tick.formattedValue}
                      </text>
                    </Group>
                  );
                })}
                <text textAnchor="middle" transform={`translate(${axisCenter}, 50)`} fontSize="8">
                  {axis.label}
                </text>
              </g>
            );
          }}
        </AxisBottom>
      </Group>
    </svg>
  );
};

Axes.defaultProps = {
  margin: {
    top: 45,
    bottom: 40,
    left: 40,
    right: 10,
  },
  xRangeMin: 0,
  xRangeRescale: 1,
};
