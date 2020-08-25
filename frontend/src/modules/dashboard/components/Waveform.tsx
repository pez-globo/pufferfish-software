import React from 'react'
import { curveLinear } from '@vx/curve'
import { LinePath } from '@vx/shape'
import { scaleTime, scaleLinear } from '@vx/scale'

interface DateValue {
  date: Date,
  value: number
}

// accessors
const x = ({ date }: DateValue) => date
const y = ({ value }: DateValue) => value

export interface Props {
  width: number,
  height: number,
  data: DateValue[],
  margin: {
    left: number,
    top: number,
    bottom: number,
    right: number
  },
  strokeWidth: number,
  xRangeMin: number,
  xRangeMax: number,
  yRangeMin: number,
  yRangeMax: number,
}

export const Waveform = ({
  width, height, margin, data, strokeWidth,
  xRangeMin, xRangeMax, yRangeMin, yRangeMax
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
    <LinePath
      data={data}
      x={d => xScale(x(d))}
      y={d => yScale(y(d))}
      stroke={'#ffffff'}
      strokeWidth={strokeWidth}
      curve={curveLinear}
    />
  )
}

Waveform.defaultProps = {
  margin: {
    top: 0,
    bottom: 0,
    left: 0,
    right: 0
  },
  xRangeMin: 0
}
