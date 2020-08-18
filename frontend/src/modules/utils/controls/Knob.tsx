import React from 'react'
import { Grid, Tabs, Tab, Typography } from '@material-ui/core'
import alarmModal from './AlarmModal'
import ValueModal from './ValueModal'
import { makeStyles } from '@material-ui/core/styles'

const useStyles = makeStyles(() => ({
  alarm: {
    background: '#0053b1',
    borderRadius: 8,
    width: '47%',
    height:42,
    alignItems: 'center'
  },
  setNew:{
    background:'#0053b1',
    borderRadius: 8,
    width: '47%',
    height:42,
    alignItems: 'center'

  }
}))

interface Props {
  valueDisplay: React.ReactNode,
  //valueAdjust: React.ReactNode,
  ValueModal?: React.ReactNode,
  alarmModal?: React.ReactNode
}

export const Knob = ({ valueDisplay, ValueModal, alarmModal }: Props) => {
  const classes = useStyles()
  const [value, setValue] = React.useState(0)

  return (
    <Grid container direction='column'
    style={{ height: '100%', padding: '18px',
    borderBottom: '0px dashed black' }}
    >
      <Grid item xs style={{ width: '100%'}}>
        {valueDisplay}
      </Grid>
      <Grid
        item
        container
        direction='row'
        justify='space-between'
        alignItems='center'
      >
        <Grid item className={classes.setNew} >
          {ValueModal}
        </Grid>
        <Grid item className={classes.alarm}>
          {alarmModal}
        </Grid>
      </Grid>
    </Grid>
  )
}

export const Knob2 = ({ valueDisplay, alarmModal}: Props) => {
  const classes = useStyles()
  const [value, setValue] = React.useState(0)

  return (
    <Grid container direction='column'
    style={{ height: '100%', padding: '18px',
    borderBottom: '0px dashed black' }}
    >
      <Grid item xs style={{ width: '100%'}}>
        {valueDisplay}
      </Grid>
      <Grid
        item
        container
        direction='row'
        justify='space-between'
        alignItems='center'
      >
        <Grid item className={classes.alarm} xs={7}>
          {alarmModal}
        </Grid>
      </Grid>
    </Grid>
  )
}

export const Knob3 = ({ valueDisplay, ValueModal, alarmModal }: Props) => {
  const classes = useStyles()
  const [value, setValue] = React.useState(0)

  return (
    <Grid container direction='column'
    style={{ height: '100%', padding: '18px',
    borderBottom: '0px dashed black' }}
    >
      <Grid item xs style={{ width: '100%'}}>
        {valueDisplay}
      </Grid>
      <Grid
        item
        container
        direction='row'
        justify='space-between'
        alignItems='center'
      >
      </Grid>
    </Grid>
  )
}

export default Knob
