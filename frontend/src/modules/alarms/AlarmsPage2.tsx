import React from 'react'
import { makeStyles, Theme } from '@material-ui/core/styles'
import { Typography, Button, Grid } from '@material-ui/core'
import Pagination from '@material-ui/lab/Pagination'
import InputSlider from '../utils/controls/InputSlider'
import ModeBanner from '../utils/displays/ModeBanner'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        border: '1px solid yellow',
    },
}))

/**
 * AlarmsPage
 */
export const AlarmsPage = () => {
    const classes = useStyles()
    const [page, setPage] = React.useState(1);

    const handleChange = (event: React.ChangeEvent<unknown>, value: number) => {
        setPage(value)
    }

    /**
     * TODO: Fill in the `InputSlider` placeholders with sliders that actually
     *       change the alarm states in the redux store.
     */
    const page1 =
        <Grid container item xs
            direction='column'
            justify='space-between'
            style={{ border: '1px solid green' }}
        >
            <InputSlider label='Pressure above PEEP' min={0} max={100} />
            <InputSlider label='PAW' min={0} max={100} />
            <InputSlider label='PiP' min={0} max={100} />
            <InputSlider label='PEEP' min={0} max={100} />
            <InputSlider label='Insp. Time' min={0} max={100} />
        </Grid>

    const page2 =
        <Grid container item xs
            direction='column'
            justify='space-between'
            style={{ border: '1px solid green' }}
        >
            <InputSlider label='RR' min={0} max={100} />
            <InputSlider label='TV' min={0} max={100} />
            <InputSlider label='Flow' min={0} max={100} />
            <InputSlider label='MVe' min={0} max={100} />
            <InputSlider label='Apnea' min={0} max={100} />
        </Grid>

    return (
        <Grid
            container
            direction='column'
            justify='space-between'
            alignItems='stretch'
            className={classes.root}
            wrap='nowrap'
        >
            {/* Content */}
            <Grid
                container
                item
                xs
                wrap='nowrap'
                style={{ border: '1px solid teal' }}
            >
                {/* Left Panel */}
                <Grid
                    container
                    item
                    xs={3}
                    direction='column'
                    justify='space-between'
                    style={{ border: '1px solid green' }}
                >
                    <Grid item style={{ border: '1px solid blue' }}>
                        <Typography variant='h3' style={{ border: '1px dashed red' }}>Alarms</Typography>
                    </Grid>
                    <Grid container item alignItems='center' direction='column' style={{ width: '100%', border: '1px solid blue' }} >
                        <Grid item xs style={{ marginBottom: 10, border: '1px solid blue' }}>
                            <Pagination count={2} page={page} onChange={handleChange} />
                        </Grid>
                        <Grid item style={{ width: '100%' }}>
                            <Button variant='outlined' style={{ width: '100%', border: '1px solid red', borderRadius: 0 }}>
                                Apply Changes
                        </Button>
                        </Grid>
                    </Grid>
                </Grid>
                {/* Right Panel of Alarm Slides */}
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