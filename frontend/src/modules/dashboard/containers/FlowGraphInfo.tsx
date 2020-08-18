import React from 'react'
import { connect } from 'react-redux'
import { createStructuredSelector } from 'reselect'
import { AutoSizer } from 'react-virtualized'
import { StoreState } from '../../../store/types'
import { WaveformPoint } from '../../../store/controller/types'
import {
  getWaveformFlowOld,
  getWaveformFlowNew
} from '../../../store/controller/selectors'
import { Waveform } from '../components/Waveform'
import { Axes } from '../components/Axes'


interface DataProps {
  data: WaveformPoint[]
}


const oldSelector = createStructuredSelector<StoreState, DataProps>({
  data: getWaveformFlowOld,
})
const newSelector = createStructuredSelector<StoreState, DataProps>({
  data: getWaveformFlowNew,
})


interface AutoSizerProps {
  width: number,
  height: number
}


interface WaveformProps extends DataProps, AutoSizerProps {
  strokeWidth: number,
  fill: string
}


const FlowWaveform = ({ data, width, height, strokeWidth,fill }: WaveformProps) => (
  <Waveform
    width={width} height={height} data={data} strokeWidth={strokeWidth}
    fill={fill}
    xRangeMax={10000} yRangeMin={-150} yRangeMax={150}
  />
)


const WaveformOld = connect(oldSelector)(FlowWaveform)
const WaveformNew = connect(newSelector)(FlowWaveform)

const FlowGraphInfo = () => (
  <AutoSizer>
    {({ width, height }: AutoSizerProps) => (
      <Axes
        width={width} height={height}
        
        waveformOld={<WaveformOld width={width} height={height} strokeWidth={1} fill={'rgba(178,192,252,0)'}/>}
        waveformNew={<WaveformNew width={width} height={height} strokeWidth={4} fill={'rgba(178,192,252,0.3)'}/>}
        xRangeMax={10000} yRangeMin={-150} yRangeMax={150}
        title={"Flow"} units={"L/min"}
      />
    )}
  </AutoSizer>
)

export default FlowGraphInfo
