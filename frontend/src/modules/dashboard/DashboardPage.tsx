import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Grid, Tabs, Tab, Typography } from '@material-ui/core'
import { PawGraphInfo, FlowGraphInfo } from './containers'
import { ValueDisplay } from '../utils/displays/ValueDisplay'
import { TabPanel, a11yProps } from '../utils/controls/TabPanel'
import {Knob, Knob3} from '../utils/controls/Knob'
import {
  PIPInfo, 
  RRInfo,
  PEEPInfo,
  FiO2Info,
  TVInfo
} from '../utils/info'

const useStyles = makeStyles(() => ({
  root: {
    borderRadius: theme.borderRadius,
    background: theme.background1,
    color: '#FFFFFF',
  },

  topPanel: {
    flexGrow: 1,
    marginBottom: theme.padding,
    borderRadius: theme.borderRadius,
    background: theme.background1,
  },

  leftPanel:{
    borderRadius: theme.borderRadius,
    background: theme.background2,
  },

  graphWrapper:{
    marginLeft: theme.padding,
    borderRadius: theme.borderRadius,
    background: theme.background2,
  },

  graphTop:{
  },

  bigIndicator:{
    borderRadius: 8,
    border: '2px solid #0053b1',
    zIndex: 0,
    marginBottom: 14,
    minHeight:20,
    height:40,
    background:theme.button,
  },

  graph: {
    height:'100%'
  },

  button1:{
    borderRadius: 8,
    border: '2px solid #0053b1',
    margin: 14,
    zIndex: 1,
    minHeight:20,
    height:40
  },

  bottomPanel: {
    
    borderRadius: theme.borderRadius,
    marginBottom: theme.padding,
    height: 220,
    background: theme.background2,
  },
}))

const theme = {
  background1: 'black',
  background2: '#0b2e4c',
  button: '#0053b1',
  color: '#21192e',
  borderRadius: 16,
  padding: 24,
  text1: 14
}

/**
 * DashboardPage
 */
export const DashboardPage = () => {
  const classes = useStyles()
  const [value, setValue] = React.useState(0)

  const handleChange = (event: React.ChangeEvent<{}>, newValue: number) => {
    setValue(newValue)
  }

  return (
    <Grid
      container
      direction="column"
      justify="space-evenly"
      className={classes.root}
      wrap="nowrap"
    >
      {/* Top Panel */}
      <Grid
        container
        item
        direction="row"
        className={classes.topPanel}
      >

        {/* Left Panel */}
        <Grid
          container
          item
          direction="column"
          justify="center"
          alignItems="stretch"
          wrap="nowrap"
          xs= {3}
          className={classes.leftPanel}
          zeroMinWidth
        >
          <Grid item xs={12}>
            <PIPInfo />
          </Grid>
          <Grid item style={{borderTop: '2px dashed black'}}xs={12}>
            <PEEPInfo />
          </Grid>
        </Grid>

        {/* Graph Panel */}
        <Grid
          container item xs direction="column"
          className={classes.graphWrapper}
        >
          <Grid
          container item direction="row" justify="center" 
          className={classes.graphTop}
          >
            <Tabs value={value}
            classes={{indicator: classes.bigIndicator}}
            onChange={handleChange}>
              <Tab label="Waveforms" {...a11yProps(0)} className={classes.button1}/>
              <Tab label="PV Loops" {...a11yProps(1)} className={classes.button1}/>
              <Tab label="Compliance" {...a11yProps(2)} className={classes.button1}/>
            </Tabs>
          </Grid>


          <TabPanel value={value} index={0}>
            <Grid 
            container 
            item 
            xs 
            direction="column" 
            justify="space-between" 
            style={{height:'100%'}}
            >
              
              <Grid item container style={{height:'48%'}}>
                <FlowGraphInfo />
              </Grid>
              <Grid item style={{height:'4%'}}/>
              <Grid item container style={{height:'48%'}}>
                <PawGraphInfo />
              </Grid>
            </Grid>
          </TabPanel>
          <TabPanel value={value} index={1}>
            <Grid container item xs direction="column" justify="space-between" className={classes.graph}>
              <Grid item xs>
                <Typography>PV Loops</Typography>
              </Grid>
            </Grid>
          </TabPanel>
          <TabPanel value={value} index={2}>
            <Grid container item xs direction="column" justify="space-between"
            className={classes.graph}>
              <Grid item xs>
                <Typography>Compliance</Typography>
              </Grid>
            </Grid>
          </TabPanel>
        </Grid>
      </Grid>

      {/* Bottom Panel */}
      <Grid
        container item direction='row'
      >
        <Grid item container xs className={classes.bottomPanel}>
          <Grid item xs={6} style={{borderRight:'2px dashed black'}}>
            <RRInfo />
          </Grid>
          <Grid item xs={6}>
            <FiO2Info />
          </Grid>
        </Grid>
        <Grid item container xs style={{marginLeft: '24px'}} className={classes.bottomPanel}>
          <Grid item xs={6}style={{borderRight:'2px dashed black'}}>
            <TVInfo />
          </Grid>
          <Grid item xs={6}>
            <Knob3
              valueDisplay={<ValueDisplay value={1}
              label="I:E Ratio" />}
              //valueAdjust={null}
              ValueModal={null}
            />
          </Grid>
          
        </Grid>
      </Grid>
    </Grid>
  )
}

export default DashboardPage