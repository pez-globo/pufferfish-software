import React from 'react';
import { makeStyles, Theme } from '@material-ui/core/styles';
import { Grid } from '@material-ui/core';
import Routes from '../navigation/Routes';
import ToolBar from './ToolBar';
import Sidebar from './Sidebar';
import { useLocation } from 'react-router-dom';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100vh',
    backgroundColor: theme.palette.background.default,
    flexWrap: 'nowrap',
    // border: '1px solid blue'
  },
  main: {
    padding: '0 16px 0 16px',
    height: '100%',
    width: '100%',
    flexWrap: 'nowrap',
    // border: '1px solid red'
  },
  toolbarContainer: {
    minHeight: theme.mixins.toolbar.minHeight,
    // border: '1px solid red',
  },
  mainContainer: {
    height: '100%',
    // border: '1px solid red',
  },
}));

const Layout = (): JSX.Element => {
  const classes = useStyles();
  const location = useLocation();
  console.log('Localtion path name', location.pathname);

  return location.pathname === '/' ? (
    <Routes />
  )
  :
  (
    <Grid container justify="center" alignItems="stretch" className={classes.root}>
      <Grid item>
        <Sidebar />
      </Grid>
      <Grid container item direction="column" className={classes.main}>
        <Grid container item alignItems="center" className={classes.toolbarContainer}>
          <ToolBar />
        </Grid>
        <Grid container item className={classes.mainContainer}>
          <Routes />
        </Grid>
      </Grid>
    </Grid>
  );
};

export default Layout;
