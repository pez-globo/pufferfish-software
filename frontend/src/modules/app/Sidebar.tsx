import { Button, Grid, makeStyles, Theme } from '@material-ui/core';
import React from 'react';
import { useSelector } from 'react-redux';
import { Link } from 'react-router-dom';
import { getParametersRequest } from '../../store/controller/selectors';
import { DASHBOARD_ROUTE, QUICKSTART_ROUTE, SCREENSAVER_ROUTE } from '../navigation/constants';
import Navigation from '../navigation/Navigation';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: 90,
    height: '100%',
    backgroundColor: 'transparent',
    padding: '15px',
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
    minWidth: 0,
    borderRadius: 8,
    marginRight: '0px',
    padding: '6px 10px',
    lineHeight: 'normal',
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
      <Grid item style={{ margin: '0 auto' }}>
        <Button
          component={Link}
          to={parameterRequest.ventilating ? DASHBOARD_ROUTE.path : QUICKSTART_ROUTE.path}
          variant="contained"
          color="primary"
          className={classes.screensaverButton}
          disableElevation
        >
          <SCREENSAVER_ROUTE.icon style={{ fontSize: '1.5rem' }} />
        </Button>
      </Grid>
      <Grid item className={classes.navMenu}>
        <Navigation />
      </Grid>
    </Grid>
  );
};

export default Sidebar;
