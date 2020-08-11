import React, { PropsWithChildren } from 'react'
import {
    Button,
    Dialog,
    DialogTitle,
    DialogContent,
    DialogActions,
    DialogProps,
    Grid,
    makeStyles,
    Theme
} from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    actionButton: {
        width: '90%'
    },
    openButton: {
        width: '100%',
        border: '1px solid ' + theme.palette.background.default
    },
    contentContainer: {
        height: '100%',
        minHeight: '300px',
        border: '2px dashed black'
    }
}))

interface Props {
    label: string,
    onConfirm(): any,
}

export const ConfirmationModal = (props: PropsWithChildren<Props>) => {
    const classes = useStyles()
    const [open, setOpen] = React.useState(false)
    const [fullWidth] = React.useState(true)
    const [maxWidth] = React.useState<DialogProps['maxWidth']>('sm')

    const handleOpen = () => {
        setOpen(true)
    }

    const handleClose = () => {
        setOpen(false)
    }

    const handleConfirm = () => {
        props.onConfirm()
        setOpen(false)
    }

    return (
        <Grid container direction='column' alignItems='center' justify='center'>
            <Grid container item xs>
                <Button onClick={handleOpen} color="primary" variant='contained' className={classes.openButton}>
                    {props.label}
                </Button>
            </Grid>
            <Dialog fullWidth={fullWidth} maxWidth={maxWidth} open={open} onClose={handleClose}>
                <DialogTitle id='form-dialog-title'>
                    {props.label}
                </DialogTitle>
                <DialogContent>
                    <Grid container item xs justify='center' alignItems='center' className={classes.contentContainer}>
                        {props.children}
                    </Grid>
                </DialogContent>
                <DialogActions>
                    <Grid container justify='center'>
                        <Grid container item xs justify='center'>
                            <Button onClick={handleClose} variant='contained' color='primary' style={{ width: '90%' }}>
                                Cancel
                            </Button>
                        </Grid>
                        <Grid container item xs justify='center'>
                            <Button onClick={handleConfirm} variant='contained' color='secondary' style={{ width: '90%' }}>
                                Confirm
                            </Button>
                        </Grid>
                    </Grid>
                </DialogActions>
            </Dialog>
        </Grid>
    )
}

export default ConfirmationModal