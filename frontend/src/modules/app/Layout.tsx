import React from 'react';
import { makeStyles, Theme } from '@material-ui/core/styles';
import { Grid } from '@material-ui/core';
import Routes from '../navigation/Routes';
import ToolBar from './ToolBar';
import Sidebar from './Sidebar';

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
}));

const Layout = (): JSX.Element => {
  const classes = useStyles();

  return (
    <React.Fragment>
      {/* <div className={classes.overlay}></div> */}
      <Grid container justify="center" alignItems="stretch" className={classes.root}>
        <Grid item className={classes.sidebarGrid}>
          <Sidebar />
        </Grid>
        <Grid container item direction="column" className={classes.main}>
          <Grid container item alignItems="center">
            <ToolBar />
          </Grid>
          <Grid container item className={classes.mainContainer}>
            <Routes />
          </Grid>
        </Grid>
      </Grid>
    </React.Fragment>
  );
};

export default Layout;
