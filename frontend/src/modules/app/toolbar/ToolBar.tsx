/**
 * @summary Toolbar
 *
 * @file Header toolbar with dynamic sub components
 *
 */
import { AppBar, Grid } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React from 'react';
import { useLocation } from 'react-router-dom';
import ViewDropdown from '../../dashboard/views/ViewDropdown';
import ClockIcon from '../../icons/ClockIcon';
import ModesDropdown from './ModesDropdown';
import { DASHBOARD_ROUTE, QUICKSTART_ROUTE } from '../navigation/constants';
import Alarms from './Alarms';
import StartButton, { StartButtonModalPopup } from './StartPauseButton';
import HeaderClock from './HeaderClock';
import PowerIndicator from './PowerIndicator';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    // border: '1px solid red',
  },
  marginRight: {
    marginRight: theme.spacing(0.5),
  },

  toolContainer: {
    height: '100%',
    // border: '1px solid red'
  },
}));

const Tools = (): JSX.Element => {
  const classes = useStyles();
  const location = useLocation();
  /**
   * Display buttons dynamically on the toolbar
   */
  const tools: JSX.Element[] = [];
  if (location.pathname === DASHBOARD_ROUTE.path) {
    tools.push(<ViewDropdown />);
  } else if (location.pathname === QUICKSTART_ROUTE.path) {
    // tools.push(
    //   <Button variant="contained" color="primary" disabled>
    //     Last Patient Settings
    //   </Button>,
    // );
  }
  if (location.pathname !== '/') {
    tools.push(<ModesDropdown />, <Alarms />);
  }
  return (
    <>
      {tools.map((tool) => (
        <Grid item className={classes.marginRight}>
          {tool}
        </Grid>
      ))}
    </>
  );
};

/**
 * ToolBar
 *
 * @component A container for displaying buttons that handle system changes based on
 * various states and conditions such as ventilator state and current page route.
 *
 * @param {React.ReactNode} children Adds dynamic items as children for SidebarSlideRoute (eg: Pufferfish Icon & Drawer icon)
 * @param {boolean} staticStart staticStart is used as a config for reusing Start/Pause Ventilation button for LandingPage Layout
 *
 * @returns {JSX.Element}
 */
export const ToolBar = ({
  children,
  staticStart = false,
}: {
  children?: React.ReactNode;
  staticStart?: boolean;
}): JSX.Element => {
  const classes = useStyles();

  return (
    <AppBar color="transparent" elevation={0} position="static">
      <Grid
        container
        direction="row"
        justify="space-between"
        alignItems="center"
        className={classes.root}
        wrap="nowrap"
      >
        {children}
        <Grid
          container
          item
          xs={8}
          direction="row"
          justify="flex-start"
          alignItems="center"
          wrap="nowrap"
          zeroMinWidth
          className={classes.toolContainer}
        >
          <Tools />
        </Grid>
        <Grid
          container
          item
          xs
          direction="row"
          justify="flex-end"
          alignItems="center"
          zeroMinWidth
          className={classes.toolContainer}
        >
          <Grid container item xs justify="flex-end" alignItems="center">
            <PowerIndicator />
            <HeaderClock />
            <ClockIcon style={{ fontSize: '2.5rem' }} />
          </Grid>
          <StartButton staticStart={staticStart} />
        </Grid>
        <StartButtonModalPopup />
      </Grid>
    </AppBar>
  );
};

export default ToolBar;
