import React from 'react'
import { Grid, Tabs, Tab } from '@material-ui/core/'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { TabPanel, a11yProps } from '../utils/controls/TabPanel'
import { InfoTab, TestCalibrationTab, DisplayTab } from './containers'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
    },
    tabPanelContainer: {
        flexGrow: 1,
        justifyContent: 'space-between',
        alignItems: 'stretch',
        height: '100',
        // border: '1px solid green',
    },
    tabContainer: {
        alignItems: 'center',
        background: '#0b2e4c',
        color:'white',
        //border: '10px solid green'
    }
}))

/** 
 * SettingsPage
 * 
 * A container storing various setting panels.
 */
export const SettingsPage = () => {
    const classes = useStyles()
    const [value, setValue] = React.useState(0)

    const handleChange = (event: React.ChangeEvent<{}>, newValue: number) => {
        setValue(newValue)
    }

    return (
        <Grid container direction='column' className={classes.root}>
            <Grid container direction='row' className={classes.tabPanelContainer}>
                <TabPanel value={value} index={0}>
                    <InfoTab />
                </TabPanel>
                <TabPanel value={value} index={1}>
                    <TestCalibrationTab />
                </TabPanel>
                <TabPanel value={value} index={2}>
                    <DisplayTab />
                </TabPanel>
            </Grid>
            <Grid container item direction='row' className={classes.tabContainer}>
                <Tabs value={value} onChange={handleChange}>
                    <Tab label='Info' {...a11yProps(0)} />
                    <Tab label='Test & Calibration' {...a11yProps(1)} />
                    <Tab label='Display' {...a11yProps(2)} />
                </Tabs>
            </Grid>
        </Grid >
    )
}

export default SettingsPage