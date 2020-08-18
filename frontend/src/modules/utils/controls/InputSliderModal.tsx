import React from 'react'
import {
    Button,
    Dialog,
    DialogTitle,
    DialogContent,
    DialogActions,
    DialogProps,
    Grid
} from '@material-ui/core'
import InputSlider from './InputSlider'

export interface SettingAdjustProps {
    committedSetting: number
}

interface Props {
    label: string,
    units: string,
    min: number,
    max: number,
}

export const InputSliderModal = (
    { label, units, min, max }: Props
) => {
    const [open, setOpen] = React.useState(false)
    const [fullWidth] = React.useState(true)
    const [maxWidth] = React.useState<DialogProps['maxWidth']>('sm')
    const title = 'Alarm'

    const handleClickOpen = () => {
        setOpen(true)
    }

    const handleClickClose = () => {
        setOpen(false)
    }

    const handleClickConfirm = () => {
        setOpen(false)
    }

    return (
        <Grid
            container
            direction='column'
            alignItems='center'
            justify='center'
        >
            <Button variant='outlined' style={{ color:'white'}} color='primary' onClick={handleClickOpen}>
                {title}
            </Button>
            <Dialog
                fullWidth={fullWidth}
                maxWidth={maxWidth}
                open={open}
                onClose={handleClickClose}
                aria-labelledby='form-dialog-title'
            >
                <DialogTitle id='form-dialog-title'>
                    {title}
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

                        <Grid
                            container
                            item
                            direction='column'
                            justify='center'
                            alignItems='center'
                            style={{ width: '100%', height: '320px', border: '1px solid orange' }}
                        >                            
                            <InputSlider label={label} min={0} max={100} />
                        </Grid>
                    </Grid>
                </DialogContent>
                <DialogActions>
                    <Grid container justify='center' style={{ border: '1px solid green' }} >
                        <Grid container item justify='center' xs style={{ border: '1px solid red' }}>
                            <Button onClick={handleClickClose} color='primary'
                                style={{ width: '80%', borderRadius: 0, border: '1px solid teal' }}>
                                Cancel
                            </Button>
                        </Grid>
                        <Grid container item justify='center' xs style={{ border: '1px solid red' }}>
                            <Button onClick={handleClickConfirm} color='primary'
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

export default InputSliderModal

