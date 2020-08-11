import React from 'react'
import { Grid, Button, makeStyles, Theme } from '@material-ui/core'
import KeyboardArrowUp from '@material-ui/icons/KeyboardArrowUp'
import KeyboardArrowDown from '@material-ui/icons/KeyboardArrowDown'

const useStyles = makeStyles((theme: Theme) => ({
    root: {
        justifyContent: 'center',
        alignItems: 'center',
        height: '100%',
        width: '100%',
        // border: '1px dashed red',
    },
    iconButton: {
        border: '1px solid black',
        borderRadius: 8,
        height: '100%',
        minHeight: 55,
        minWidth: 78,
    },
    space: {
        marginBottom: theme.spacing(2)
    }
}))

interface Props {
    value: number,
    onClick: (value: number) => void,
    // An inclusive range defined by [min, max]
    min?: number,
    max?: number,
}

/**
 * ValueClicker
 * 
 * A re-usable component for simple increment and decrement value adjustments. 
 */
export const ValueClicker = ({ value, min = 0, max = 100, onClick }: Props) => {
    const classes = useStyles()

    const circularUpdate = (step: number) => () => {
        let change = value + step
        if (change > max) {
            change = min
        } else if (change < min) {
            change = max
        }
        return onClick(change)
    }

    return (
        <Grid container direction='column' className={classes.root}>
            <Grid item className={classes.space}>
                <Button onClick={circularUpdate(1)} variant='contained' color='primary' className={classes.iconButton}>
                    <KeyboardArrowUp fontSize='large' />
                </Button>
            </Grid>
            <Grid item>
                <Button onClick={circularUpdate(-1)} variant='contained' color='primary' className={classes.iconButton}>
                    <KeyboardArrowDown fontSize='large' />
                </Button>
            </Grid>
        </Grid>
    )
}

export default ValueClicker 