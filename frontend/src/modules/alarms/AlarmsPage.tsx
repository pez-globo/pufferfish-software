import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Typography, Button, Grid } from '@material-ui/core'
import Pagination from '@material-ui/lab/Pagination'
import ValueSlider from '../controllers/ValueSlider'
import ModeBanner from '../displays/ModeBanner'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        justifyContent: 'space-between',
        alignItems: 'stretch',
        flexWrap: 'nowrap'
    },
    panel: {
        borderRadius: theme.panel.borderRadius,
        flexWrap: 'nowrap',
        marginBottom: theme.spacing(2),
        backgroundColor: theme.palette.background.paper
    },
    leftContainer: {
        justifyContent: 'space-between',
        borderRight: '2px dashed ' + theme.palette.background.default,
        padding: theme.spacing(2),
    },
    rightContainer: {
        justifyContent: 'space-between',
        padding: theme.spacing(2)
    },
    paginationContainer: {
        alignItems: 'center',
        width: '100%',
        // border: '1px solid blue'
    },
    applyButton: {
        width: '100%',
        border: '1px solid black',
    },
}))

// NOTE: Temporary Alarm until the UI team address interface concerns.
interface AlarmProps {
    label: string,
    min: number,
    max: number
}

const Alarm = ({ label, min, max }: AlarmProps) => {
    return (
        <Grid container>
            <Grid item xs={12} style={{paddingBottom: 20}}>
                <Typography variant='h5'>{label}</Typography>
            </Grid>
            <Grid item xs={12}>
                <ValueSlider min={min} max={max} />
            </Grid>
        </Grid>
    )
}

/**
 * AlarmsPage
 *
 * A container for housing all alarm configurations.
 */
export const AlarmsPage = () => {
    const classes = useStyles()
    const [page, setPage] = React.useState(1);

    const handleChange = (event: React.ChangeEvent<unknown>, value: number) => {
        setPage(value)
    }

    /**
     * TODO: Fill in the `ValueSlider` placeholders with sliders that actually
     *       change the alarm states in the redux store.
     */
    const page1 =
        <Grid container item xs direction='column' className={classes.rightContainer}>
            <Alarm label='Pressure above PEEP' min={0} max={100} />
            <Alarm label='PAW' min={0} max={100} />
            <Alarm label='PiP' min={0} max={100} />
            <Alarm label='PEEP' min={0} max={100} />
            <Alarm label='Insp. Time' min={0} max={100} />
        </Grid>

    const page2 =
        <Grid container item xs direction='column' className={classes.rightContainer}>
            <Alarm label='RR' min={0} max={100} />
            <Alarm label='TV' min={0} max={100} />
            <Alarm label='Flow' min={0} max={100} />
            <Alarm label='MVe' min={0} max={100} />
            <Alarm label='Apnea' min={0} max={100} />
        </Grid>

    return (
        <Grid container direction='column' className={classes.root}>
            <Grid container item xs direction='row' className={classes.panel}>
                <Grid container item xs={3} direction='column' className={classes.leftContainer}>
                    <Grid item>
                        <Typography variant='h3'>Alarms</Typography>
                    </Grid>
                    <Grid container direction='column' className={classes.paginationContainer}>
                        <Grid item xs style={{ marginBottom: 10 }}>
                            <Pagination count={2} page={page} onChange={handleChange} variant='outlined' shape="rounded" size="large" />
                        </Grid>
                        <Grid item style={{ width: '100%' }}>
                            <Button color='secondary' variant='contained' className={classes.applyButton}>
                                Apply Changes
                            </Button>
                        </Grid>
                    </Grid>
                </Grid>
                {/* Right Container for Storing Alarm Slides */}
                {page === 1 ? page1 : page2}
            </Grid>
            {/* Title */}
            <Grid item>
                <ModeBanner />
            </Grid>
        </Grid>
    )
}

export default AlarmsPage