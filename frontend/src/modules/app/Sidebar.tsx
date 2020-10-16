import React from 'react';
import { Link } from 'react-router-dom';
import { Button, Grid, Typography, makeStyles, Theme } from '@material-ui/core';
import { useSelector } from 'react-redux';
import Navigation from '../navigation/Navigation';
import { DASHBOARD_ROUTE, QUICKSTART_ROUTE, SCREENSAVER_ROUTE } from '../navigation/constants';
import ShutdownIcon from '../icons/ShutdownIcon';
import { getParameters, getParametersRequest } from '../../store/controller/selectors';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: 90,
    height: '100%',
    backgroundColor: theme.palette.background.paper,
  },
  shutdownButton: {
    '&:hover': { background: theme.palette.error.dark },
    backgroundColor: theme.palette.error.main,
    color: theme.palette.text.primary,
    height: 80,
    width: '100%',
    borderRadius: 0,
  },
  screensaverButton: {
    height: 80,
    width: '100%',
    minWidth: 0,
    borderRadius: 0,
  },
  navMenu: {
    height: '100%',
  },
}));

/**
 * Sidebar
 *
 * The main navigation panel of the application.
 */
const Sidebar = (): JSX.Element => {
  const classes = useStyles();
  const parameterRequest = useSelector(getParametersRequest);

  return (
    <Grid
      container
      direction="column"
      justify="space-between"
      alignItems="stretch"
      className={classes.root}
      wrap="nowrap"
    >
      <Grid item>
        <Button
          component={Link}
          to={parameterRequest.ventilating ? DASHBOARD_ROUTE.path : QUICKSTART_ROUTE.path}
          variant="contained"
          color="primary"
          className={classes.screensaverButton}
          disableElevation
        >
          <div>
            <SCREENSAVER_ROUTE.icon style={{ fontSize: '1.5rem', width: '100%' }} />
            <br />
            <Typography variant="subtitle2" align="center">
              {SCREENSAVER_ROUTE.label}
            </Typography>
          </div>
        </Button>
      </Grid>
      <Grid item className={classes.navMenu}>
        <Navigation />
      </Grid>
      <Grid item>
        <Button variant="contained" className={classes.shutdownButton} disableElevation>
          <div>
            <ShutdownIcon style={{ fontSize: '1.5rem' }} />
            <br />
            <Typography variant="subtitle2" style={{ color: 'white' }} align="center">
              Shutdown
            </Typography>
          </div>
        </Button>
      </Grid>
    </Grid>
  );
};

export default Sidebar;
