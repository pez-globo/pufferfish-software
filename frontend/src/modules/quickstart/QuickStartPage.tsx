import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import {
    Grid,
    Typography,
    FormControl,
    RadioGroup,
    FormControlLabel,
    Radio
} from '@material-ui/core'
import TestTool from '../utils/controls/TestTool'
import ModeBanner from '../utils/displays/ModeBanner'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        border: '1px dashed brown',
        color:'white'
    },
}))

export const QuickStartPage = () => {
    const classes = useStyles()

    return (
        <Grid
            container
            direction='column'
            justify='space-between'
            alignItems='stretch'
            className={classes.root}
            wrap="nowrap"
        >
            {/* Top Row */}
            <Grid container item xs wrap="nowrap" style={{ maxHeight: 180, border: '1px solid teal' }}>
                <Grid item xs style={{ border: '1px solid green' }}>
                    <Typography variant='h3'>Standby</Typography>
                    <Typography variant='body1'>Patient Not Ventilated</Typography>
                </Grid>
                <Grid item xs style={{ border: '1px solid green' }}>
                    <Typography variant='h6'>Age:</Typography>
                    <FormControl component="fieldset">
                        <RadioGroup defaultValue="Adult" name="age-radios">
                            <FormControlLabel value="Adult" control={<Radio />} label="Adult" />
                            <FormControlLabel value="Pediatric" control={<Radio />} label="Pediatric" />
                        </RadioGroup>
                    </FormControl>
                </Grid>
                <Grid item xs style={{ border: '1px solid green' }}>
                    <Typography variant='h6'>Sex:</Typography>
                    <FormControl component="fieldset">
                        <RadioGroup defaultValue="Male" name="gender-radios">
                            <FormControlLabel value="Male" control={<Radio />} label="Male" />
                            <FormControlLabel value="Female" control={<Radio />} label="Female" />
                        </RadioGroup>
                    </FormControl>
                </Grid>
                <Grid item xs style={{ border: '1px solid green' }}>
                    <Typography variant='h6'>Height:</Typography>
                </Grid>
            </Grid>
            {/* Middle Row */}
            <Grid
                container
                item
                xs
                direction='row'
                wrap="nowrap"
                style={{ width: '100%', border: '1px solid teal' }}
            >
                {/* Left Bottom Panel */}
                <Grid container item xs direction='column' style={{ border: '1px solid blue' }}>
                    <Grid item xs style={{ border: '1px solid red' }}>
                        <TestTool label='Pre-Use Check' isSuccess={true} timestamp={new Date()} />
                    </Grid>
                    <Grid item xs style={{ border: '1px solid red' }}>
                        <TestTool label='Patient Circuit Test' isSuccess={true} timestamp={new Date()} />
                    </Grid>
                </Grid>
                {/* Right Bottom Panel */}
                <Grid container item xs={8} direction='column' style={{ border: '1px solid black' }}>
                    <Grid container item xs direction='row'>
                        <Grid item xs style={{ border: '1px solid purple' }}>
                            <Typography>Value</Typography>
                        </Grid>
                        <Grid item xs style={{ border: '1px solid purple' }}>
                            <Typography>Value</Typography>
                        </Grid>
                    </Grid>
                    <Grid container item xs direction='row'>
                        <Grid item xs style={{ border: '1px solid purple' }}>
                            <Typography>Value</Typography>
                        </Grid>
                        <Grid item xs style={{ border: '1px solid purple' }}>
                            <Typography>Value</Typography>
                        </Grid>
                    </Grid>
                </Grid>
            </Grid>
            {/* Title */}
            <Grid item> 
                <ModeBanner />
            </Grid>
        </Grid>
    )
}

export default QuickStartPage