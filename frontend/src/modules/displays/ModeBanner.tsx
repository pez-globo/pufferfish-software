import React from 'react'
import { Grid, Typography, makeStyles, Theme } from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        height: 80,
        maxHeight: 80        
    },
    text: {
        color: theme.palette.background.paper,
    }
}))

interface Props {
    mode?: string
}

/**
 * ModeBanner
 * 
 * A contianer for displaying the current mode in a large font banner. 
 * 
 * TODO: Hook up the `mode` prop to the redux store so we can display
 *       the current mode below.
 */
export const ModeBanner = ({ mode = 'Current Mode' }: Props) => {
    const classes = useStyles()

    return (
        <Grid container justify='center' alignItems='center' className={classes.root}>
            <Grid item>
                <Typography align='center' variant='h2' className={classes.text}>
                    {mode}
                </Typography>
            </Grid>
        </Grid>
    )
}

export default ModeBanner