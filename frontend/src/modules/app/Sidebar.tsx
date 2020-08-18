import React from 'react'
import { Link } from 'react-router-dom'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Button, Grid } from '@material-ui/core'
import Navigation from '../navigation/Navigation'
import puffer from '../icons/puffer.svg'
import Shutdown from '../icons/Shutdown.svg'

console.log(puffer);

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        //border: '1px solid yellow',
        background: '',
    },
    sidebar: {
        width: 100,
        height: '100%',
        background:'#0b2e4c'
        //border: '1px solid green',
    },

    puffer:{
        height: 90,
        width: '100%',
        borderRadius: 0,
        background:'#0053b1',
        color:'white',
        alignItems:'baseline',
        textAlign:'center',
        paddingTop:20,
    }

}))

/**
 * Sidebar
 * 
 * The main navigation panel of the application.
 */
export const Sidebar = () => {
    const classes = useStyles()

    return (
        <Grid
            container
            direction='column'
            justify='space-between'
            alignItems='stretch'
            className={classes.sidebar}
            wrap='nowrap'
        >
            <Grid item>
                <Grid
                    className={classes.puffer}
                    //variant='contained'
                    //component={Link}
                    //to='/screensaver'
                    //disableElevation
                >
                    <img src={puffer}/>
                    <br/>
                    Pufferfish
                </Grid>
                
            </Grid>
            <Grid item>
                <Navigation />
            </Grid>
            <Grid item>
                <Grid
                    //variant='contained'
                    className={classes.puffer}
                    style={{ background:'#f73b39'}}
                    //disableElevation
                >
                    <img src={Shutdown}/>
                    <br/>
                    Shutdown
                </Grid>
            </Grid>
        </Grid>
    )
}

export default Sidebar