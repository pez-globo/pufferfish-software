import { Grid, makeStyles, Theme, Typography } from '@material-ui/core';
import React, { useEffect, useState } from 'react';
import { useSelector } from 'react-redux';
import { getScreenStatus } from '../../../store/controller/selectors';
import ModalPopup from '../../controllers/ModalPopup';

const useStyles = makeStyles((theme: Theme) => ({
  overlay: {
    width: '100%',
    height: '100%',
    background: 'rgb(0 0 0 / 39%)',
    position: 'absolute',
    zIndex: 9999,
  },
  label: {
    fontSize: '10px',
  },
}));

export const OverlayScreen = (): JSX.Element => {
  const classes = useStyles();
  const screenStatus = useSelector(getScreenStatus);
  const [open, setOpen] = React.useState(false);
  const [overlay, setOverlay] = useState(screenStatus || false);

  useEffect(() => {
    setOverlay(screenStatus);
  }, [screenStatus]);

  const alertPopup = () => {
    if (open) {
      return;
    }
    setOpen(true);
    setTimeout(() => {
      setOpen(false);
    }, 3000);
  };

  return (
    <React.Fragment>
      {overlay && (
        <div
          role="button"
          tabIndex={0}
          aria-label="Screenlock Alert"
          className={classes.overlay}
          onClick={alertPopup}
          onKeyDown={() => null}
        />
      )}
      <ModalPopup withAction={false} label="Alert" open={open}>
        <Grid container direction="row">
          <Grid container item xs direction="column">
            <Grid container item xs wrap="nowrap">
              <Grid container item alignItems="baseline">
                <Typography variant="h5" align="center">
                  Screen is locked
                </Typography>
              </Grid>
            </Grid>
          </Grid>
        </Grid>
      </ModalPopup>
    </React.Fragment>
  );
};

export default OverlayScreen;
