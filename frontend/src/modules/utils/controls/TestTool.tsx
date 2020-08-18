import React from 'react'
import { Button, Grid, Typography } from '@material-ui/core'
import CheckCircleIcon from '@material-ui/icons/CheckCircle'
import ErrorIcon from '@material-ui/icons/Error';

interface Props {
    label: string,
    isSuccess: boolean
    timestamp: Date,
}

/**
 * TestTool
 * 
 * A component for performing tests and calibrations. 
 */
export const TestTool = ({ label, isSuccess, timestamp }: Props) => {
    return (
        <Grid container direction='column' justify='center' alignItems='stretch' style={{ height: '100%', border: '1px solid pink' }}>
            <Grid container item xs direction='row' justify='space-between' style={{ border: '1px solid green' }}>
                <Grid item >
                    <Typography variant='h5'>{label}</Typography>
                </Grid>
                <Grid item>
                    {isSuccess ? <CheckCircleIcon /> : <ErrorIcon />}
                </Grid>
            </Grid>
            <Grid container item xs direction='row' justify='space-between' style={{ border: '1px solid green' }}>
                <Grid item>
                    <Typography>{timestamp.toLocaleTimeString()} </Typography>
                </Grid>
                <Grid item>
                    <Typography>{timestamp.toLocaleDateString()}</Typography>
                </Grid>
            </Grid>
            <Grid container item xs direction='row' justify='space-evenly' alignItems='center' style={{ border: '1px solid red' }}>
                <Grid item xs>
                    <Button variant='outlined' style={{ width: '100%', borderRadius: 0, border: '1px solid teal' }}>
                        Start
                    </Button>
                </Grid>
                <Grid item xs>
                    <Button variant='outlined' style={{ width: '100%', borderRadius: 0, border: '1px solid teal' }}>
                        View More
                    </Button>
                </Grid>
            </Grid>
        </Grid >
    )
}

export default TestTool