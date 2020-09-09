import React, { useState, useEffect } from 'react'
import { Link } from 'react-router-dom'
import { BellIcon } from '../icons'
import { Button, Snackbar, makeStyles, Theme, Grid, Typography } from '@material-ui/core'
import { Alert } from '@material-ui/lab';


const useStyles = makeStyles((theme: Theme) => ({
    root: {
        height: '100%',
        paddingTop: theme.spacing(1),
        paddingBottom: theme.spacing(2),
        paddingLeft: theme.spacing(2),
        paddingRight: theme.spacing(2)
    },
    controlPanel: {
        justifyContent: 'space-between',
        alignItems: 'center',
        minHeight: 40
    },
    marginRight: {
        marginRight: theme.spacing(1)
    },
    openButton: {
        width: '100%',
        border: '1px solid ' + theme.palette.common.black
    }
}))

interface Props {
    path: string,
    label: string
}

export const AlertToast = ({ openState, label }: { openState: boolean, label: string }) => {
    const classes = useStyles()
    const [open, setOpen] = React.useState(openState);

    const handleClick = () => {
        setOpen(true);
    };

    const handleClose = (event?: React.SyntheticEvent, reason?: string) => {
        setOpen(false);
    };

    return (
        <Snackbar open={open} onClose={handleClose} anchorOrigin={{ vertical: "top", horizontal: "center" }}>
            <Alert icon={false} onClose={handleClose} variant="filled" severity="error">
                <Grid container direction='column' className={classes.root}>
                    <Grid item xs style={{ width: '100%' }}>
                        <Typography variant='h5'>
                            {label}
                        </Typography>
                    </Grid>
                    <Grid container item direction='row' className={classes.controlPanel} wrap='nowrap'>
                        <Grid item xs={6} className={classes.marginRight}>
                            <Button onClick={() => { }} variant='contained' className={classes.openButton}>
                                Reset Alarm
                            </Button>
                        </Grid>
                        <Grid item xs={6}>
                            <Button onClick={() => { }} variant='contained' className={classes.openButton}>
                                Silence for 2 min
                            </Button>
                        </Grid>
                    </Grid>
                </Grid>
            </Alert>
        </Snackbar>
    )
}



export const EventAlerts = ({ path, label }: Props) => {

    const [alert, setAlert] = useState({ label: 'Peep Above upper boundary!' });
    // Generate Random Alerts
    useEffect(() => {
        const randomAlertInterval = setInterval(() => {
            setAlert({ label: 'random alert ' + Math.random()})
        }, 2000);
        return () => clearInterval(randomAlertInterval)
    }, [])

    return (
        <div>
            <Button style={{ marginRight: 12 }} component={Link} to={path} variant='contained' color='primary'>
                <BellIcon />
            </Button>
            {label}
            <AlertToast openState={true} label={alert.label} />
        </div>
    )
}

export default EventAlerts
