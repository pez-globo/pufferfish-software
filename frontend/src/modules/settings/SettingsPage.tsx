import React from 'react'
import { Grid, Tabs, Tab } from '@material-ui/core/'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { TabPanel, a11yProps } from '../controllers/TabPanel'
import { InfoTab, TestCalibrationTab, DisplayTab } from './containers'

const useStyles = makeStyles((theme: Theme) => ({
    tabPanelContainer: {
        flexGrow: 1,
        justifyContent: 'space-between',
        alignItems: 'stretch',
        height: '100',
        // border: '1px solid green',
    },
    tabContainer: {
        minHeight: 80,
        // border: '1px solid red',
    },
    tabs: {
        // paddingTop: theme.spacing(1) 
        // border: '1px solid red',
        width: '100%',
    },
    tab: {
        borderRadius: 8,
        border: '2px solid ' + theme.palette.primary.main,
        margin: theme.spacing(1),
        zIndex: 1,
        minHeight: 40,
        minWidth: 220,
    },
    selectedTab:{color:theme.palette.primary.contrastText},
    tabIndicator: {
        borderRadius: 8,
        border: '2px solid ' + theme.palette.primary.main,
        zIndex: 0,
        marginBottom: theme.spacing(1),
        minHeight: 40,
        background: theme.palette.primary.main
    },
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
        <Grid container direction='column'>
            <Grid container className={classes.tabPanelContainer}>
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
            <Grid container item justify='center' alignItems='center' className={classes.tabContainer}>
                <Tabs value={value} onChange={handleChange} className={classes.tabs} classes={{ indicator: classes.tabIndicator }}>
                    <Tab label='Info' {...a11yProps(0)} className={classes.tab}
                    classes={{selected:classes.selectedTab}} />
                    <Tab label='Test & Calibration' {...a11yProps(1)} className={classes.tab}
                    classes={{selected:classes.selectedTab}} />
                    <Tab label='Display' {...a11yProps(2)} className={classes.tab}
                    classes={{selected:classes.selectedTab}} />
                </Tabs>
            </Grid>
        </Grid>
    )
}

export default SettingsPage