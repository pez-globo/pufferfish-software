import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Grid } from '@material-ui/core'
import Routes from '../navigation/Routes'
import Sidebar from './Sidebar'
import ToolBar from './ToolBar'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        height: '100vh',
        background:'black',
        //backgroundColor: theme.palette.background.default,
    },
    main: {
        padding: '0 16px 0 16px',
        height: '100%',
        width: '100%',
        //border: '1px solid red'
    },
    content: {
        flexGrow: 1,
        //border: '1px solid orange',
    }
}))

export const Layout = () => {
    const classes = useStyles()

    return (
        <Grid container direction='row' justify='center' alignItems='stretch' className={classes.root} wrap='nowrap'>
            <Grid item>
                <Sidebar />
            </Grid>
            <Grid container item direction='column' justify='space-evenly' wrap='nowrap' className={classes.main}>
                <Grid item>
                    <ToolBar />
                </Grid>
                <Grid container item className={classes.content} zeroMinWidth>
                    <Routes />
                </Grid>
            </Grid>
        </Grid>
    )
}

export default Layout