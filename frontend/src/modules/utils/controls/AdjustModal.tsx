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
    title: string,
    controller: React.ReactNode,
    display: React.ReactNode,
    requestCommit(): any,
}

export const AdjustModal = (props: PropsWithChildren<Props>) => {
    const [fullWidth] = React.useState(true)
    const [maxWidth] = React.useState<DialogProps['maxWidth']>('sm')
    const [open, setOpen] = React.useState(false)

    const handleOpen = () => {
        setOpen(true)
    }

    const handleClose = () => {
        setOpen(false)
    }

    const handleConfirm = () => {
        props.requestCommit()
        setOpen(false)
    }

    return (
        <Grid
            container
            direction='column'
            alignItems='center'
            justify='center'
        >
            <Button variant='outlined' style={{ borderRadius: 0 }} color='primary' onClick={handleOpen}>
                {props.title}
            </Button>
            <Dialog
                fullWidth={fullWidth}
                maxWidth={maxWidth}
                open={open}
                onClose={handleClose}
                aria-labelledby='form-dialog-title'
            >
                <DialogTitle id='form-dialog-title'>
                    {props.title}
                </DialogTitle>
                <DialogContent>
                    <Grid
                        item
                        container
                        direction='row'
                        justify='center'
                        alignItems='center'
                        style={{ height: '100%', border: '1px solid blue' }}
                        wrap='nowrap'
                    >
                        {props.display}
                        {props.controller}
                    </Grid>
                </DialogContent>
                <DialogActions>
                    <Grid container justify='center' style={{ border: '1px solid green' }} >
                        <Grid container item justify='center' xs style={{ border: '1px solid red' }}>
                            <Button onClick={handleClose} color='primary'
                                style={{ width: '80%', borderRadius: 0, border: '1px solid teal' }}>
                                Cancel
                            </Button>
                        </Grid>
                        <Grid container item justify='center' xs style={{ border: '1px solid red' }}>
                            <Button onClick={handleConfirm} color='primary'
                                style={{ width: '80%', borderRadius: 0, border: '1px solid teal' }}>
                                Confirm
                            </Button>
                        </Grid>
                    </Grid>
                </DialogActions>
            </Dialog>
        </Grid>
    )
}

export default AdjustModal

