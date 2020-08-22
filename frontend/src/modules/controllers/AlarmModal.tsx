import React from 'react'
import ValueSlider from './ValueSlider'
import ConfirmationModal from './ConfirmationModal'
import ValueClicker from './ValueClicker'
import { makeStyles, Theme, Grid, Button, Typography } from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    contentContainer: {
        width: '100%',
        height: '100%',
        minHeight: '300px',
        border: '2px dashed ' + theme.palette.background.default,
        borderRadius: theme.panel.borderRadius
    },
    borderBottom: {
        borderBottom: '2px dashed ' + theme.palette.background.default
    },
    openButton: {
        width: '100%',
        //border: '1px solid ' + theme.palette.background.default
    },
    alarmContainer: {
        // border: '1px solid red',
    },
    alarmValue: {
        marginRight: theme.spacing(3),
        border: '1px solid ' + theme.palette.text.primary,
        minWidth: 100,
        paddingLeft: theme.spacing(1),
        paddingRight: theme.spacing(1),
        borderRadius: 8,
    },
}))

export interface AlarmAdjustProps {
    committedMin: number,
    committedMax: number
}

interface Props {
    label: string,
    units: string,
    committedMin?: number,
    committedMax?: number,
    requestCommitRange(min: number, max: number): any
}

export const AlarmModal = (
    { label, committedMin = 0, committedMax = 100, requestCommitRange }: Props
) => {
    const classes = useStyles()
    const [open, setOpen] = React.useState(false)
    const [min] = React.useState(committedMin)
    const [max] = React.useState(committedMax)
    const [rangeValue, setRangeValue] = React.useState<number[]>([min, max])

    const handleOpen = () => {
        setOpen(true)
    }

    const handleClose = () => {
        setOpen(false)
    }

    const handleConfirm = () => {
        requestCommitRange(min, max)
    }

    const setSortedRangeValue = (rangeValue: any) => {
        setRangeValue(rangeValue.sort());
    };

    return (
        <Grid container direction='column' alignItems='center' justify='center'>
            <Grid container item xs>
                <Button onClick={handleOpen} color="primary" variant='contained' className={classes.openButton}>
                    Alarm
                </Button>
            </Grid>
            <ConfirmationModal label={`${label} - Alarm`} open={open} onClose={handleClose} onConfirm={handleConfirm}>
                <Grid container direction='column' alignItems='stretch' className={classes.contentContainer}>
                    <Grid container item xs justify='center' alignItems='center' className={`${classes.alarmContainer} ${classes.borderBottom}`}>
                        <Grid item className={classes.alarmValue}>
                            <Typography align='center' variant='h3'>
                                {rangeValue[0] !== undefined ? rangeValue[0].toFixed(0) : '--'}
                            </Typography>
                        </Grid>
                        <Grid item>
                            <ValueClicker
                                value={rangeValue[0]}
                                min={committedMin} max={committedMax}
                                onClick={value => setRangeValue(Object.assign([], rangeValue, { 0: value }))}
                                direction='row'
                            />
                        </Grid>
                    </Grid>
                    <Grid container item xs justify='center' alignItems='center' className={`${classes.alarmContainer} ${classes.borderBottom}`}>
                        <Grid item className={classes.alarmValue}>
                            <Typography align='center' variant='h3'>
                                {rangeValue[1] !== undefined ? rangeValue[1].toFixed(0) : '--'}
                            </Typography>
                        </Grid>
                        <Grid item>
                            <ValueClicker
                                value={rangeValue[1]}
                                min={committedMin} max={committedMax}
                                onClick={value => setRangeValue(Object.assign([], rangeValue, { 1: value }))}
                                direction='row'
                            />
                        </Grid>
                    </Grid>
                    <Grid container item xs alignItems='center'>
                        <ValueSlider rangeValues={rangeValue} onChange={setSortedRangeValue} min={min} max={max} />
                    </Grid>
                </Grid>
            </ConfirmationModal>
        </Grid>
    )
}

export default AlarmModal