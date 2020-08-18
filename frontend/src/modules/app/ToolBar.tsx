import React from 'react'
import { useLocation, Link } from 'react-router-dom'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Button, Grid } from '@material-ui/core'
import { LOGS_ROUTE, DASHBOARD_ROUTE } from '../navigation/constants'
import ModesDropdown from '../navigation/ModesDropdown'
import BackButton from '../navigation/components/BackButton'
import ViewDropdown from '../dashboard/containers/ViewDropdown'
import alarms from '../icons/alarms.svg'

console.log(alarms);

const useStyles = makeStyles((theme: Theme) => ({
    appbar: {
        minHeight: 80,
        maxHeight: 80,
        height: 80,
        //border: '1px solid teal',
        background: 'black'
    },
    button:{
        borderRadius: 8,
        background: '#0053b1',
        color:'#FFFFFF',
        marginRight:19,
    },

    button2:{
        borderRadius: 8,
        paddingLeft:19,
        paddingRight:19,
        background: '#53d769',
        color:'#000'
    },

    alert:{
        borderRadius: 8,
        background:'#0053b1',
        marginRight:12,
        height:'100%',
        //color:'#FFF',
    },

    alertLog:{
        color:'#FFF'
    }
}))

/** 
 * ToolBar
 */
export const ToolBar = () => {
    const classes = useStyles()
    // Store the route location so we can change button/breadcrumb displays
    // depending on the current route.
    let location = useLocation()
    let tools;
    if (location.pathname === DASHBOARD_ROUTE.path) {
        tools =
            <Grid container>
                <Grid item className={classes.button}>
                    <ViewDropdown />
                </Grid>
                <Grid item>
                    <Button
                    className={classes.alert}
                    >
                    <img src={alarms}/>
                    </Button>
                </Grid>
                <Grid item>
                    <Button
                        component={Link}
                        to={LOGS_ROUTE.path}
                        ///color='primary'
                        disableElevation
                        className={classes.alertLog}
                    >
                        Alert Log
                    </Button>
                </Grid>
            </Grid>
    } else {
        // TODO: Make this Dashboard button available only if the ventilator is on. 
        tools =
            <Grid container>
                <Grid item>
                    <BackButton
                        label={DASHBOARD_ROUTE.label}
                        path={DASHBOARD_ROUTE.path}
                    />
                </Grid>
            </Grid>
    }

    return (
        <Grid
            container
            direction='row'
            justify='space-between'
            alignItems='center'
            className={classes.appbar}
            wrap='nowrap'
        >
            <Grid
                container
                item
                xs={8}
                direction='row'
                justify='flex-start'
                alignItems='center'
                //style={{ border: '10px solid green' }}
                wrap='nowrap'
                zeroMinWidth
            >
                <Grid item className={classes.button}>
                    <ModesDropdown />
                </Grid>
                {tools}
            </Grid>
            <Grid
                container
                item
                xs
                alignItems='center'
                direction='row'
                justify='flex-end'
                //style={{ border: '10px solid green' }}
                zeroMinWidth
            >
                <Grid item>
                    <Button
                        color='primary'
                        disableElevation
                        className={classes.button2}
                        //style={{ borderRadius: 0, border: '1px solid blue' }}
                    >
                        Start Ventilation
                    </Button>
                </Grid>
            </Grid>
        </Grid>
    )
}

export default ToolBar