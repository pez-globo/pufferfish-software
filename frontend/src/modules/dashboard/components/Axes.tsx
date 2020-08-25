import React from 'react'
import { Grid } from '@vx/grid'
import { Group } from '@vx/group'
import { AxisLeft, AxisRight, AxisBottom } from '@vx/axis'
import { Line } from '@vx/shape'
import { scaleTime, scaleLinear } from '@vx/scale'
import { GridRows, GridColumns } from '@vx/grid'

// responsive utils for axis ticks
function numTicksForHeight(height: number) {
  if (height <= 300) return 3
  if (300 < height && height <= 600) return 5
  return 10
}

function numTicksForWidth(width: number) {
  if (width <= 300) return 2
  if (300 < width && width <= 400) return 5
  return 10
}

export interface Props {
  width: number,
  height: number,
  waveformOld: React.ReactNode,
  waveformNew: React.ReactNode,
  margin: {
    left: number,
    top: number,
    bottom: number,
    right: number
  },
  xRangeMin: number,
  xRangeMax: number,
  yRangeMin: number,
  yRangeMax: number,
  title: string,
  units: string
}

export const Axes = ({
  width, height, margin, waveformOld, waveformNew,
  xRangeMin, xRangeMax, yRangeMin, yRangeMax, title, units
}: Props) => {
  // bounds
  const xMax = width - margin.left - margin.right
  const yMax = height - margin.top - margin.bottom

  // scales
  const xScale = scaleTime({
    domain: [new Date(xRangeMin), new Date(xRangeMax)],
    range: [0, xMax]
  })
  const yScale = scaleLinear({
    domain: [yRangeMin, yRangeMax],
    range: [yMax, 0],
    nice: true
  })

  return (
    <svg style={{ flexGrow: 2 }} width={width} height={height}>
      <rect x={0} y={0} width={width} height={height} fill='#3d373a' rx={14} />
      <Grid
        top={margin.top}
        left={margin.left}
        xScale={xScale}
        yScale={yScale}
        stroke='rgba(61, 55, 58, 0.9)'
        width={xMax}
        height={yMax}
        numTicksRows={numTicksForHeight(height)}
        numTicksColumns={numTicksForWidth(width)}
      />
      <Group top={margin.top} left={margin.left}>
        {waveformOld}
        {waveformNew}
      </Group>
      <Group left={margin.left}>
        <GridRows scale={yScale} width={xMax} height={yMax} stroke='#000000' />
        <GridColumns scale={xScale} width={xMax} height={yMax} stroke='#000000' />
        <AxisLeft
          top={margin.top}
          left={0}
          scale={yScale}
          hideZero
          numTicks={numTicksForHeight(height)}
          label='Axis Left Label'
          labelProps={{
            fill: '#8e205f',
            textAnchor: 'middle',
            fontSize: 12,
            fontFamily: 'Arial'
          }}
          stroke='#1b1a1e'
          tickStroke='#8e205f'
          tickLabelProps={() => ({
            fill: '#8e205f',
            textAnchor: 'end',
            fontSize: 10,
            fontFamily: 'Arial',
            dx: '-0.25em',
            dy: '0.25em'
          })}
          tickComponent={({ formattedValue, ...tickProps }) => (
            <text {...tickProps}>{formattedValue}</text>
          )}
        />
        <text x='30' y='20' color='#FFFFFF' fontSize={15} >
          {title}
        </text>
        <text x='30' y='32' color='#FFFFFF' fontSize={10} >
          {units}
        </text>
        <AxisRight scale={yScale}
          stroke='#FFFFFF'
          tickLabelProps={() => ({
            fill: '#FFFFFF',
            fontSize: 11,
            textAnchor: 'middle',
          })}
        />
        <AxisBottom
          top={height - margin.bottom}
          left={0}
          scale={xScale}
          numTicks={numTicksForWidth(width)}
          label='Time'
        >
          {axis => {
            const tickLabelSize = 10
            const tickRotate = 45
            const tickColor = '#8e205f'
            const axisCenter = (axis.axisToPoint.x - axis.axisFromPoint.x) / 2
            return (
              <g className='my-custom-bottom-axis'>
                {axis.ticks.map((tick, i) => {
                  const tickX = tick.to.x
                  const tickY = tick.to.y + tickLabelSize + axis.tickLength
                  return (
                    <Group key={`vx-tick-${tick.value}-${i}`} className={'vx-axis-tick'}>
                      <Line from={tick.from} to={tick.to} stroke={tickColor} />
                      <text
                        transform={`translate(${tickX}, ${tickY}) rotate(${tickRotate})`}
                        fontSize={tickLabelSize}
                        textAnchor='middle'
                        fill={tickColor}
                      >
                        {tick.formattedValue}
                      </text>
                    </Group>
                  )
                })}
                <text textAnchor='middle' transform={`translate(${axisCenter}, 50)`} fontSize='8'>
                  {axis.label}
                </text>
              </g>
            )
          }}
        </AxisBottom>
      </Group>
    </svg>
  )
}

Axes.defaultProps = {
  margin: {
    top: 0,
    bottom: 0,
    left: 0,
    right: 0
  },
  xRangeMin: 0
}
