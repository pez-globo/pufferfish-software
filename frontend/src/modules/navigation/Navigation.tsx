import React from 'react'
import { Link } from 'react-router-dom'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Tabs, Tab, Grid } from '@material-ui/core'
import {
    QUICKSTART_ROUTE,
    VALUES_ROUTE,
    MODES_ROUTE,
    ALARMS_ROUTE,
    SETTINGS_ROUTE
} from './constants'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        width: '100%',
    },
    tabs: {
        alignText: 'center',
        zIndex:1,
    },
    tabRoot: {
        minWidth: 0,
        minHeight: 80,
        fontSize: '11px',
        //border: '1px solid orange',
        background: '#0b2e4c',
        color:'#FFFFFF'
    },
    myIndicator:{
        background:'#fff',
        opacity: '.2',
        borderLeft:'4px solid #0053b1',
        width:'100%',
        zIndex:0
    }
}))

/**
 * Navigation
 * 
 * TODO: Remove the `Dashboard` Tab after getting `Back` button navigation working
 *       in the `ToolBar`.
 */
export const Navigation = () => {
    const classes = useStyles()
    const [value, setValue] = React.useState(0)

    const routes = [
        QUICKSTART_ROUTE,
        MODES_ROUTE,
        ALARMS_ROUTE,
        VALUES_ROUTE,
        SETTINGS_ROUTE
    ]

    const handleChange = (event: React.ChangeEvent<{}>, newValue: number) => {
        setValue(newValue)
    }

    return (
        <div className={classes.root}>
            <Tabs
                orientation='vertical'
                variant='scrollable'
                value={value}
                onChange={handleChange}
                className={classes.tabs}
                style={{alignItems:'baseline'}}
                classes={{indicator:classes.myIndicator}}
            >
                {routes.map(route => {
                    return (
                        
                        <Tab
                            label={route.label}
                            component={Link}
                            to={route.path}
                            classes={{ root: classes.tabRoot }}
                            disableRipple
                        >
                        <img src={route.icon}/>
                        </Tab>
                    )
                })}
            </Tabs>
        </div>
    )
}

export default Navigation