import React from 'react'
import { Button, Grid, Typography, makeStyles, Theme } from '@material-ui/core'
import CheckCircleIcon from '@material-ui/icons/CheckCircle'
import ErrorIcon from '@material-ui/icons/Error'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        justifyContent: 'center',
        alignItems: 'stretch',
        height: '100%',
        padding: theme.spacing(2)
    },
    startButton: {
        width: '100%',
        border: '1px solid black',
        borderRadius: 8,
        minHeight: 44,
    },
    viewMoreButton: {
        width: '100%',
        border: '1px solid black',
        borderRadius: 8,
        minHeight: 44,
    },
    marginRight: {
        marginRight: theme.spacing(2)
    },
    successIcon: {
        color: theme.palette.success.main
    },
    errorIcon: {
        color: theme.palette.error.main
    }
}))

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
    const classes = useStyles()

    return (
        <Grid container direction='column' className={classes.root}>
            <Grid container item xs direction='row' justify='space-between'>
                <Grid item >
                    <Typography variant='h5'>{label}</Typography>
                </Grid>
                <Grid item>
                    {isSuccess
                        ? <CheckCircleIcon className={classes.successIcon} />
                        : <ErrorIcon className={classes.errorIcon} />
                    }
                </Grid>
            </Grid>
            <Grid container item xs direction='row' justify='space-between'>
                <Grid item>
                    <Typography>{timestamp.toLocaleTimeString()} </Typography>
                </Grid>
                <Grid item>
                    <Typography>{timestamp.toLocaleDateString()}</Typography>
                </Grid>
            </Grid>
            <Grid container item direction='row' justify='space-evenly' alignItems='center'>
                <Grid item xs className={classes.marginRight}>
                    <Button variant='contained' color='secondary' className={classes.startButton}>
                        Start
                    </Button>
                </Grid>
                <Grid item xs>
                    <Button variant='contained' color='primary' className={classes.viewMoreButton}>
                        View More
                    </Button>
                </Grid>
            </Grid>
        </Grid>
    )
}

export default TestTool