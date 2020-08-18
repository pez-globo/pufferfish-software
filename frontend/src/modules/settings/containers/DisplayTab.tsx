import React from 'react'
import {
    Grid,
    Typography,
    FormControl,
    FormControlLabel,
    RadioGroup,
    Radio,
    Box,
    Button,
    makeStyles,
    Theme
} from '@material-ui/core'
import ValueClicker from '../../utils/controls/ValueController'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        marginBottom: theme.spacing(2)
    },
    leftPanel: {
        backgroundColor: theme.palette.background.paper,
        borderRadius: 16,
        border: '1px solid black',
    },
    rightPanel: {
        height: '100%',
        marginLeft: theme.spacing(2),
        backgroundColor: theme.palette.background.paper,
        borderRadius: 16,
        border: '1px solid black',
    },
    borderBottom: {
        borderBottom: '2px dashed black'
    },
    leftContainer: {
        borderTop: '2px dashed black',
        paddingLeft: theme.spacing(2)
    }
}))

/**
 * props
 */
// interface props {
// }

// Periods for 12-Hour Based Time Format
enum Period { AM, PM }
// UI Color UITheme
enum UITheme { DARK, LIGHT }

// Returns the number of days in a month for a given year.
const getDaysInMonth = (month: number, year: number) => {
    const days: number = (new Date(year, month, 0)).getDate()
    return days
}
// Converts a 24-hour formatted hour to 12-hour period based format. 
const to12HourClock = (hour: number) => { return (hour % 12) || 12 }
// Converts a 12-hour formatted hour to 24-hour based format.
const to24HourClock = (hour: number, period: Period) => { return (period === Period.PM) ? hour + 12 : hour }

/**
 * DisplayTab 
 * 
 * TODO: Hook this up to the redux state to persist changes across the system's state.
 *       We need to make sure that updates to dat
 */
export const DisplayTab = () => {
    const classes = useStyles()
    const [brightness, setBrightness] = React.useState(100)
    const [theme, setTheme] = React.useState(UITheme.DARK)
    // Date & Time State
    // TODO: `date` needs to hook into the redux store so that every date-related state
    //       declared below can be initalized with the same state the ventilator is working off.
    const [date, setDate] = React.useState<Date>(new Date())
    const [period, setPeriod] = React.useState((date.getHours() >= 12) ? Period.PM : Period.AM)
    const [minute, setMinute] = React.useState(date.getMinutes())
    const [hour, setHour] = React.useState(to12HourClock(date.getHours())) // Note: `date.hours()` is 24-hour formatted.
    const [day, setDay] = React.useState(date.getDate())
    const [month, setMonth] = React.useState(date.getMonth() + 1) /* Note: `getMonth()` returns a value in [0, 11] */
    const [year, setYear] = React.useState(date.getFullYear())

    const handleSubmit = () => {
        // TODO: The below `set<State>(...)` calls should be replaced by dispatches into the redux store.
        const dateChange = new Date(year, month - 1, day, to24HourClock(hour, period), minute)
        setDate(dateChange)
        setTheme(theme)
        setBrightness(brightness)
    }

    const handleMonthChange = (change: number) => {
        const maxDaysInMonth = getDaysInMonth(change, year)
        // Update `day` component if its value > max days in the new month.
        if (day > maxDaysInMonth) {
            setDay(maxDaysInMonth)
        }
        setMonth(change)
    }

    return (
        <Grid container className={classes.root}>
            <Grid container item xs={4} direction='column' className={classes.leftPanel}>
                {/* Brightness */}
                <Grid container item xs direction='row'>
                    <ValueClicker
                        value={brightness}
                        label='Brightness'
                        units='%'
                        onClick={setBrightness}
                        min={0}
                        max={100}
                    />
                </Grid>
                {/* Color TODO: This should be a toggle switch. */}
                <Grid container item xs direction='column' justify='center' className={classes.leftContainer}>
                    <Typography variant='h5'>Color</Typography>
                    <FormControl component='fieldset'>
                        <RadioGroup
                            value={theme}
                            onChange={(event) => setTheme(+event.target.value as UITheme)}
                            name='ui-theme-radios'
                        >
                            <FormControlLabel value={UITheme.DARK} control={<Radio />} label='Dark UI' />
                            <FormControlLabel value={UITheme.LIGHT} control={<Radio />} label='Light UI' />
                        </RadioGroup>
                    </FormControl>
                </Grid>
                {/* Date & Time */}
                <Grid container item xs direction='column' justify='space-evenly' className={classes.leftContainer}>
                    <Box>
                        <Typography variant='h6'>Date</Typography>
                        <Typography>{date.toLocaleDateString()}</Typography>
                    </Box>
                    <Box>
                        <Typography variant='h6'>Time</Typography>
                        <Typography>{date.toLocaleTimeString()}</Typography>
                    </Box>
                </Grid>
            </Grid>
            {/* Right Panel: Date & Time */}
            <Grid container item xs direction='column' className={classes.rightPanel}>
                <Grid container item xs alignItems='stretch' className={classes.borderBottom}>
                    <Grid item xs> 
                        <ValueClicker
                            value={hour}
                            label='Hour'
                            onClick={setHour}
                            min={1}
                            max={12}
                        />
                    </Grid>
                    <Grid item xs>
                        <ValueClicker
                            value={minute}
                            label='Minute'
                            onClick={setMinute}
                            min={0}
                            max={59}
                        />
                    </Grid>
                    <Grid container item justify='center' alignItems='center' xs={3}>
                        <FormControl component='fieldset'>
                            <RadioGroup
                                value={period}
                                onChange={(event) => setPeriod(+event.target.value as Period)}
                                name='period-radios'
                            >
                                <FormControlLabel value={Period.AM} control={<Radio />} label='AM' />
                                <FormControlLabel value={Period.PM} control={<Radio />} label='PM' />
                            </RadioGroup>
                        </FormControl>
                    </Grid>
                </Grid>
                <Grid container item xs direction='row' className={classes.borderBottom}>
                    <Grid item xs>
                        <ValueClicker
                            value={month}
                            label='Month'
                            onClick={handleMonthChange}
                            min={1}
                            max={12}
                        />
                    </Grid>
                    <Grid item xs>
                        <ValueClicker
                            value={day}
                            label='Day'
                            onClick={setDay}
                            min={1}
                            max={getDaysInMonth(month, year)}
                        />
                    </Grid>
                    <Grid item xs={3}>
                    </Grid>
                </Grid>
                <Grid container item xs>
                    <Grid item xs={6}>
                        <ValueClicker
                            value={year}
                            label='Year'
                            onClick={setYear}
                            min={year}
                            max={year}
                        />
                    </Grid>
                    <Grid container item xs justify='center' alignItems='center' >
                        <Button onClick={() => handleSubmit()} style={{ border: '1px solid orange' }}>
                            Apply Changes
                        </Button>
                    </Grid>
                </Grid>
            </Grid>
        </Grid >
    )
}

export default DisplayTab