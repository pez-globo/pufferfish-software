/**
 * @summary A short one-line description for the file
 *
 * @file More detailed description for the file, if necessary;
 * perhaps spanning multiple lines.
 */
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
import ReplyIcon from '@material-ui/icons/Reply';

const useStyles = makeStyles((theme: Theme) => ({
  closeButton: {
    color: theme.palette.text.primary,
    border: `1px solid ${theme.palette.text.primary}`,
    borderRadius: 6,
    padding: '5px',
  },
  dialogActions: {
    padding: 0,
  },
  popupWidth: {
    '& .MuiDialog-container': {
      '& .MuiPaper-root': {
        display: 'flex',
        alignItems: 'center',
        minHeight: 'calc(100% - 64px)',
      },
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
  containerClass: {
    flexDirection: 'column',
    flexGrow: 1,

    '& .MuiDialogContent-root': {
      position: 'relative',
      width: '100%',
      display: 'flex',
      flexDirection: 'column',
      justifyContent: 'center',
    },
  },
  wrapper: {
    width: '100%',
    height: '100%',
    display: 'flex',
    // alignItems: 'center',
    // marginTop: 'auto',
    // marginBottom: 'auto'
    flexDirection: 'column',
    flexGrow: 1,
  },
  wrapperFlex: {
    width: '100%',
    height: '100%',
    display: 'flex',
    flexDirection: 'column',
    flexGrow: 1,

    '& .MuiDialogTitle-root': {
      // padding: '25px',
      width: '100%',
    },
  },
  actionButtons: {
    margin: '15px',
    display: 'flex',
    alignItems: 'center',
    width: '100%',
  },
}));

/**
 * @typedef Props
 *
 * Props interface for ModalPopup component
 *
 * @prop {boolean} open desc for open
 * @prop {string | ReactNode} label desc for label
 * @prop {boolean} withAction desc for withAction
 * @prop {boolean} fullWidth desc for fullWidth
 * @prop {boolean} showCloseIcon desc for showCloseIcon
 * @prop {function} onClose desc for onClose
 * @prop {function} onConfirm desc for onConfirm
 * @prop {DialogProps['maxWidth']} maxWidth desc for maxWidth
 */
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

/**
 * @typedef ActionProps
 *
 * Interface for Action Props
 *
 * @prop {function} onClose desc for onClose
 * @prop {function} onConfirm desc for onConfirm
 */
interface ActionProps {
  onClose?(): void;
  onConfirm?(): void;
}

/**
 * ModalAction
 *
 * Description for ModalAction
 *
 * Uses the [[ActionProps]] interface
 *
 * @returns JSX.Element
 */
const ModalAction = ({ onClose, onConfirm }: ActionProps): JSX.Element => {
  const classes = useStyles();
  return (
    <Grid container justify="center" className={classes.actionButtons}>
      <Grid item justify="center">
        <Button
          onClick={onClose}
          variant="contained"
          color="primary"
          style={{ marginRight: '15px' }}
        >
          {/* style={{ width: '90%' }} */}
          Cancel
        </Button>
      </Grid>
      <Grid item justify="center">
        <Button onClick={onConfirm} variant="contained" color="secondary">
          Confirm
        </Button>
      </Grid>
    </Grid>
  );
};

/**
 * ModalPopup
 *
 * @component A re-usable "Modal" component for displaying popup.
 *
 * Uses the [[Props]] interface
 *
 * @returns JSX.Element
 */
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
    fullWidth = true,
    maxWidth = 'xl',
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
      <Grid alignItems="center" className={classes.wrapper}>
        <Grid alignItems="center" className={classes.wrapperFlex}>
          <DialogTitle id="form-dialog-title">
            <Grid container alignItems="center">
              <Grid item xs>
                <Typography variant="h4" style={{ fontWeight: 'normal' }}>
                  {label}
                </Typography>
              </Grid>
              <Grid item>
                {showCloseIcon && (
                  <IconButton aria-label="close" className={classes.closeButton} onClick={onClose}>
                    <ReplyIcon />
                  </IconButton>
                )}
              </Grid>
            </Grid>
          </DialogTitle>
          <Grid container alignItems="center" className={classes.containerClass}>
            <DialogContent>
              {children}
              <DialogActions style={{ width: '100%' }} className={classes.dialogActions}>
                {withAction && <ModalAction onClose={onClose} onConfirm={onConfirm} />}
              </DialogActions>
            </DialogContent>
          </Grid>
        </Grid>
      </Grid>
    </Dialog>
  );
};

export default ModalPopup;
