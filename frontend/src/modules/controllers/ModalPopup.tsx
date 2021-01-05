import React, { PropsWithChildren, ReactNode } from 'react';
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
  Typography,
  IconButton,
} from '@material-ui/core';
import CloseIcon from '@material-ui/icons/Close';

const useStyles = makeStyles((theme: Theme) => ({
  closeButton: {
    color: theme.palette.text.primary,
    border: `1px solid ${theme.palette.text.primary}`,
    borderRadius: 6,
  },
  dialogActions: {
    padding: 0,
  },
  popupWidth: {
    '& .MuiDialog-container': {
      '& .MuiDialog-paperWidthSm': {
        maxWidth: '750px',

        '& .MuiDialogTitle-root': {
          paddingBottom: '5px',
        },

        '& .MuiDialogContent-root': {
          padding: '0px 24px',
          overflow: 'hidden',
        },
      },
    },
  },
}));

interface Props {
  open: boolean;
  label: string | ReactNode;
  withAction?: boolean;
  fullWidth?: boolean;
  showCloseIcon?: boolean;
  onClose?(): void;
  onConfirm?(): void;
  maxWidth?: DialogProps['maxWidth'];
}

interface ActionProps {
  onClose?(): void;
  onConfirm?(): void;
}

const ModalAction = ({ onClose, onConfirm }: ActionProps): JSX.Element => {
  return (
    <Grid container justify="center" style={{ marginBottom: '10px' }}>
      <Grid container item xs justify="center">
        <Button onClick={onClose} variant="contained" color="primary" style={{ width: '90%' }}>
          Cancel
        </Button>
      </Grid>
      <Grid container item xs justify="center">
        <Button onClick={onConfirm} variant="contained" color="secondary" style={{ width: '90%' }}>
          Confirm
        </Button>
      </Grid>
    </Grid>
  );
};

export const ModalPopup = (props: PropsWithChildren<Props>): JSX.Element => {
  const classes = useStyles();
  const {
    open,
    onClose,
    label,
    showCloseIcon,
    children,
    withAction,
    onConfirm,
    fullWidth = false,
    maxWidth = 'sm',
  } = props;
  return (
    <Dialog
      fullWidth={true}
      maxWidth={fullWidth ? false : maxWidth}
      open={open}
      onClose={onClose}
      className={classes.popupWidth}
      scroll="paper"
    >
      <DialogTitle id="form-dialog-title">
        <Grid container alignItems="center">
          <Grid item xs>
            <Typography variant="h4">{label}</Typography>
          </Grid>
          <Grid item>
            {showCloseIcon && (
              <IconButton aria-label="close" className={classes.closeButton} onClick={onClose}>
                <CloseIcon />
              </IconButton>
            )}
          </Grid>
        </Grid>
      </DialogTitle>
      <DialogContent>{children}</DialogContent>
      <DialogActions className={classes.dialogActions}>
        {withAction && <ModalAction onClose={onClose} onConfirm={onConfirm} />}
      </DialogActions>
    </Dialog>
  );
};

export default ModalPopup;
