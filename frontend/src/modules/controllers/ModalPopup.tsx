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
    Theme,
    Typography
} from '@material-ui/core'

const useStyles = makeStyles((theme: Theme) => ({
    closeButton: {
        color: theme.palette.text.primary,
        border: '1px solid ' + theme.palette.text.primary,
        borderRadius: 6
    },
    dialogActions: {
        paddingBottom: theme.spacing(2)
    }
}))

interface Props {
    open: boolean,
    label: string,
    withAction?: boolean,
    onClose?(): any,
    onConfirm?(): any,
}

interface ActionProps {
    onClose?(): any,
    onConfirm?(): any,
}

const ModalAction = ({onClose, onConfirm}: ActionProps) => {
    return (
        <Grid container justify='center'>
            <Grid container item xs justify='center'>
                <Button onClick={onClose} variant='contained' color='primary' style={{ width: '90%' }}>
                    Cancel
                </Button>
            </Grid>
            <Grid container item xs justify='center'>
                <Button onClick={onConfirm} variant='contained' color='secondary' style={{ width: '90%' }}>
                    Confirm
                </Button>
            </Grid>
        </Grid>
    )
}

export const ModalPopup = (props: PropsWithChildren<Props>) => {
    const classes = useStyles()
    const [fullWidth] = React.useState(true)
    const [maxWidth] = React.useState<DialogProps['maxWidth']>('sm')
    return (
        <Dialog fullWidth={fullWidth} maxWidth={maxWidth} open={props.open} onClose={props.onClose}>
            <DialogTitle id='form-dialog-title'>
                <Grid container alignItems='center'>
                    <Grid item xs>
                        <Typography variant='h4'>
                            {props.label}
                        </Typography>
                    </Grid>
                    {/* <Grid item>
                        <IconButton aria-label="close" className={classes.closeButton} onClick={props.onClose}>
                            <CloseIcon />
                        </IconButton>
                    </Grid> */}
                </Grid>
            </DialogTitle>
            <DialogContent>
                {props.children}
            </DialogContent>
            <DialogActions className={classes.dialogActions}>
                {props.withAction && <ModalAction onClose={props.onClose} onConfirm={props.onConfirm} /> }
            </DialogActions>
        </Dialog>
    )
}

export default ModalPopup