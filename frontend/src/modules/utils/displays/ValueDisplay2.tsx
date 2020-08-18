import React from 'react'
import Grid from '@material-ui/core/Grid'
import Typography from '@material-ui/core/Typography'
import { makeStyles } from '@material-ui/core/styles'

export interface ValueProps {
    value: number
}

export interface Props {
    value: number,
    label: string,
    units?: string,
}

const useStyles = makeStyles(() => ({
  current: {
    borderRadius:6,
    border: '2px solid #0053b1',
    paddingLeft: '6px',
    paddingRight:'6px',
    fontSize:'14px',
  },
  units:{
  color:'#d1d4d9'
  }
}))

export const ValueDisplay = ({ value, label, units }: Props) => {
    const classes = useStyles()
    return (
        <Grid container direction='column' style={{ height: '90%', border: '0px solid green' }}>
            <Grid container direction='row' justify='space-between' alignItems='center'>
                <Grid item>
                    <Typography variant='h6' style={{ fontSize: 25 }}>
                        {label}
                    </Typography>
                </Grid>
                <Grid item>
                    <Typography variant='subtitle1'
                    className={classes.current}>
                        {/* TODO: Make `current` a `prop` so that it is only shown when needed. */}
                        current
                    </Typography>
                </Grid>
            </Grid>
            <Grid container item xs direction='row'>
                <Grid
                    container
                    item
                    xs={6}
                    justify='center'
                    alignItems='flex-end'
                    style={{ flexGrow: 1, border: '0px solid orange', }}
                >
                    <Typography align='left' variant='h2'>
                        {value !== undefined ? value.toFixed(2) : '--'}
                    </Typography>
                </Grid>
                <Grid
                    container
                    item
                    xs={6}
                    justify='center'
                    alignItems='flex-end'
                    className={classes.units}
                    style={{ border: '0px solid purple'}}
                >
                    <Typography variant='subtitle1'>
                        {units}
                    </Typography>
                </Grid >
            </Grid>
        </Grid >
    )
}

export default ValueDisplay