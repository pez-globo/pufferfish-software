import React from 'react'
import { Grid, Typography } from '@material-ui/core'

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
export const ModeBanner = ({ mode = 'Mode (Type)' }: Props) => {
    return (
        <Grid
            container
            direction='row'
            justify='center'
            alignItems='center'
            style={{ border: '1px solid red', maxHeight: 80 }}
        >
            <Grid item xs>
                <Typography align='center' variant='h2'>
                    {mode}
                </Typography>
            </Grid>
        </Grid>
    )
}

export default ModeBanner