import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Typography, Grid } from '@material-ui/core'
import ModeBanner from '../utils/displays/ModeBanner'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        border: '1px solid yellow',
    },
}))

/**
 * ScreensaverPage
 */
export const ScreensaverPage = () => {
    const classes = useStyles()

    return (
        <Grid container direction='column' justify='space-between'>
            <Grid
                container
                xs
                direction='row'
                justify='space-evenly'
                alignItems='stretch'
                className={classes.root}
                wrap="nowrap"
            >
                {/* TODO: Replace the `Typography` components with value displays. */}
                <Grid item xs={3} style={{ border: '1px solid black' }}>
                    <Typography>PiP</Typography>
                </Grid>
                <Grid item xs={3} style={{ border: '1px solid black' }}>
                    <Typography>PEEP</Typography>
                </Grid>
                <Grid item xs={3} style={{ border: '1px solid black' }}>
                    <Typography>RR</Typography>
                </Grid>
                <Grid item xs={3} style={{ border: '1px solid black' }}>
                    <Typography>FiO2</Typography>
                </Grid>
            </Grid>
            <Grid item>
                <ModeBanner />
            </Grid>
        </Grid>
    )
}

export default ScreensaverPage