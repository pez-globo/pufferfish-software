import React, { Component, PropsWithChildren, useEffect, useState } from 'react';
import { Route, RouteProps, useLocation } from 'react-router-dom';
import { makeStyles, Theme } from '@material-ui/core/styles';
import { Grid } from '@material-ui/core';
import { Subscription } from 'rxjs';
import Sidebar from '../Sidebar';
import ToolBar from '../ToolBar';
import UserActivity from '../UserActivity';
import OverlayScreen from './OverlayScreen';
import { getActiveEventState } from '../Service';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100vh',
    backgroundColor: theme.palette.background.default,
    flexWrap: 'nowrap',
    display: 'grid',
    gridTemplateAreas: `
                    'content vent'`,
    gridTemplateColumns: '90px 1fr',
  },
  sidebarGrid: {
    gridArea: 'content',
    height: '100vh',
  },

  main: {
    gridGap: '15px',
    display: 'grid',
    padding: '15px',
    height: '100vh',
    width: '100%',
    flexWrap: 'nowrap',
    gridArea: 'vent',
    gridTemplateRows: '40px 1fr',
    overflow: 'hidden',
  },
  mainContainer: {
    height: '100%',
  },
  overlay: {
    width: '100%',
    height: '100%',
    background: 'rgb(0 0 0 / 39%)',
    position: 'absolute',
    zIndex: 9999,
  },
  borderOverlay: {
    width: '100%',
    height: '100%',
    position: 'absolute',
    border: '4px solid red',
  },
}));

const SidebarLayout = ({ children, ...rest }: PropsWithChildren<unknown>): JSX.Element => {
  const classes = useStyles();
  const [showBorder, setShowBorder] = React.useState(false);

  useEffect(() => {
    const logEventSubscription: Subscription = getActiveEventState().subscribe((state: boolean) => {
      setShowBorder(state);
    });
    return () => {
      if (logEventSubscription) {
        logEventSubscription.unsubscribe();
      }
    };
  }, []);

  return (
    <React.Fragment>
      <OverlayScreen />
      <Grid
        container
        justify="center"
        alignItems="stretch"
        className={`${showBorder && classes.borderOverlay} ${classes.root}`}
      >
        <Grid item className={classes.sidebarGrid}>
          <Sidebar />
        </Grid>
        <Grid container item direction="column" className={classes.main}>
          <Grid container item alignItems="center">
            <ToolBar />
          </Grid>
          <Grid container item className={classes.mainContainer}>
            {children}
          </Grid>
        </Grid>
      </Grid>
      <UserActivity />
    </React.Fragment>
  );
};

const SidebarRoute = ({ component: Component, ...rest }: RouteProps): JSX.Element | null => {
  if (!Component) return null;
  return (
    <Route
      {...rest}
      render={(matchProps) => (
        <SidebarLayout>
          <Component {...matchProps} />
        </SidebarLayout>
      )}
    />
  );
};

export default SidebarRoute;
