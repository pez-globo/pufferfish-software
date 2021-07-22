/**
 * @summary Main Wrapper component for Dashboard
 *
 */
import { Grid, makeStyles } from '@material-ui/core';
import React from 'react';
import HFNCMainView from './views/HFNCMainView';

const useStyles = makeStyles(() => ({
  root: {
    flexGrow: 1,
    justifyContent: 'center',
    alignItems: 'stretch',
    // marginBottom: theme.spacing(2),
  },
}));

/**
 * DashboardPage
 *
 * @component Main Wrapper component for Dashboard
 *
 * TODO: Hook into the redux store to get the current dashboard `view` from the `ViewDropdown`
 *
 * @returns {JSX.Element}
 *
 */
export const DashboardPage = (): JSX.Element => {
  const classes = useStyles();

  return (
    <Grid container className={classes.root}>
      {/* TODO: Switch views based on `view` state. */}
      <HFNCMainView />
    </Grid>
  );
};

export default DashboardPage;
