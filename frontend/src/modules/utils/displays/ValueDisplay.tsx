import React from 'react'
import { Grid, makeStyles, Theme, Typography } from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        height: '100%',
        width: '100%',
        // border: '1px solid black'
    },
    displayContainer: {
        justifyContent: 'space-between',
        alignItems: 'stretch',
        // border: '1px solid orange'
    },
    liveContainer: {
        justifyContent: 'center',
        alignItems: 'center',
        height: '100%',
        //border: '1px solid blue'
    },
    liveBox: {
        width: '100%',
        fontSize: 14,
        borderRadius: 6,
        border: '2px solid #0053b1',
        textAlign: 'center',
        paddingLeft: '6px',
    paddingRight:'6px',
    },
    valueContainer: {
        justifyContent: 'flex-start',
        alignItems: 'center',
        flexGrow: 1,
    },
    unitsContainer: {
        justifyContent: 'center',
        alignItems: 'center',
        paddingLeft: theme.spacing(1),
        paddingTop: theme.spacing(5),
        // border: '1px solid red',
    },

}))

export interface ValueProps {
    value: number
}

export interface Props {
    value: number,
    label: string,
    units?: string,
    isLive?: boolean
}

/**
 * ValueDisplay
 * 
 * A re-usable component for displaying numeric values.
 */
export const ValueDisplay = ({ value, label, units = '', isLive = false }: Props) => {
    const classes = useStyles()

    return (
        <Grid container direction='column' className={classes.root}>
            <Grid container direction='row' justify='space-between' alignItems='center' wrap='nowrap'>
                <Grid item xs style={{ width: '100%' }}>
                    <Typography variant='h5'>
                        {label}
                    </Typography>
                </Grid>
                {isLive &&
                    <Grid container item xs={4} className={classes.liveContainer}>
                        <Typography className={classes.liveBox}>
                            Current
                        </Typography>
                    </Grid>
                }
            </Grid>
            <Grid container item xs className={classes.displayContainer}>
                <Grid container item xs className={classes.valueContainer}>
                    <Typography align='center' style={{ fontWeight: 400, fontSize: '4.5rem' }}>
                        {value !== undefined ? value.toFixed(0) : '--'}
                    </Typography>
                    {units !== '' &&
                        <Typography align='center' variant='body1' className={classes.unitsContainer}>
                            {units}
                        </Typography>
                    }
                </Grid>
            </Grid>
        </Grid>
    )
}

export default ValueDisplay