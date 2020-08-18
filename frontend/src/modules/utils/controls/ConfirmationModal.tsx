import React, { PropsWithChildren } from 'react'
import Grid from '@material-ui/core/Grid'
import {
    Button,
    Dialog,
    DialogTitle,
    DialogContent,
    DialogActions,
    DialogProps,
} from '@material-ui/core'

interface Props {
    label: string,
    onConfirm(): any,
}

export const ConfirmationModal = (props: PropsWithChildren<Props>) => {
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
                <Button style={{ color:'white',
                height: '100%',
                //border: '1px solid black',
                width: '100%' }} onClick={handleOpen}>
                    {props.label}
                </Button>
            </Grid>
            <Dialog fullWidth={fullWidth} maxWidth={maxWidth} open={open} onClose={handleClose}>
                <DialogTitle id='form-dialog-title'>
                    {props.label}
                </DialogTitle>
                <DialogContent>
                    <Grid
                        container
                        item
                        xs
                        direction='row'
                        justify='center'
                        alignItems='center'
                        style={{ minHeight: '300px', height: '100%', border: '1px dashed black' }}
                    >
                        {props.children}
                    </Grid>
                </DialogContent>
                <DialogActions>
                    <Grid container justify='center'
                        style={{
                            // border: '1px solid black'
                        }}
                    >
                        <Grid container item xs justify='center'>
                            <Button onClick={handleClose} 
                                style={{ width: '80%', border: '1px solid orange' }}>
                                Cancel
                            </Button>
                        </Grid>
                        <Grid container item xs justify='center'
                            style={{
                                // border: '1px solid red'
                            }}>
                            <Button onClick={handleConfirm}
                                style={{ width: '80%', border: '1px solid teal' }}>
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

