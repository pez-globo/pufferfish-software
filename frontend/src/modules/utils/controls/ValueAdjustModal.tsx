import React from 'react'
import Grid from '@material-ui/core/Grid'
import {
    Button,
    Dialog,
    DialogTitle,
    DialogContent,
    DialogActions,
    DialogProps,
    ButtonGroup,
    IconButton
} from '@material-ui/core'
import AddIcon from '@material-ui/icons/Add'
import RemoveIcon from '@material-ui/icons/Remove'
import { ValueDisplay } from '../displays/ValueDisplay'

export interface SettingAdjustProps {
    committedSetting: number
}

interface Props {
    label: string,
    units: string,
    committedSetting: number,
    requestCommitSetting(setting: number): any,
}

export const ValueAdjustModal = (
    { label, units, committedSetting, requestCommitSetting }: Props
) => {
    const [open, setOpen] = React.useState(false)
    const [fullWidth] = React.useState(true)
    const [maxWidth] = React.useState<DialogProps['maxWidth']>('sm')
    const [setting, setSetting] = React.useState(committedSetting)
    const title = 'Set New'

    const handleSettingAdjustment = (step: number) => () => {
        setSetting(setting + step)
    }

    const handleClickOpen = () => {
        setOpen(true)
    }

    const handleClickClose = () => {
        setOpen(false)
    }

    const handleClickConfirm = () => {
        requestCommitSetting(setting)
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
                            xs={9}
                            direction='column'
                            justify='center'
                            alignItems='center'
                            style={{ height: '320px', border: '1px solid orange' }}
                        >
                            <ValueDisplay value={setting} label={label} units={units} />
                        </Grid>
                        <Grid container item xs direction='column' alignItems='center' style={{ border: '1px solid red' }}>
                            <ButtonGroup orientation='vertical' style={{ border: '1px solid black', borderRadius: 0 }}>
                                <IconButton
                                    style={{ borderRadius: 0, border: '1px dashed green', height: 80 }}
                                    onClick={handleSettingAdjustment(10)}
                                >
                                    <AddIcon /><AddIcon />
                                </IconButton>
                                <IconButton
                                    style={{ borderRadius: 0, border: '1px dashed green', height: 80 }}
                                    onClick={handleSettingAdjustment(1)}
                                >
                                    <AddIcon />
                                </IconButton>
                                <IconButton
                                    style={{ borderRadius: 0, border: '1px dashed green', height: 80 }}
                                    onClick={handleSettingAdjustment(-1)}
                                >
                                    <RemoveIcon />
                                </IconButton>
                                <IconButton
                                    style={{ borderRadius: 0, border: '1px dashed green', height: 80 }}
                                    onClick={handleSettingAdjustment(-10)}
                                >
                                    <RemoveIcon /><RemoveIcon />
                                </IconButton>
                            </ButtonGroup >
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

export default ValueAdjustModal