/**
 * @summary Component to display Navigation Routes with Icon
 *
 */
import { Tab, Tabs, Typography } from '@material-ui/core';
import { makeStyles, Theme } from '@material-ui/core/styles';
import React, { PropsWithChildren, useEffect, useMemo } from 'react';
import { Link, useLocation } from 'react-router-dom';
import { ALARMS_ROUTE, SETTINGS_ROUTE } from './constants';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: '100%',
    height: 'auto',
  },
  tabs: {
    alignText: 'center',
    zIndex: 1,
    marginTop: 20,
    height: '100%',
    '&;selected': {
      backgroundColor: 'white',
    },
    '& .MuiTabs-fixed': {
      height: '100%',
      '& .MuiTabs-centered': {
        height: '100%',
      },
    },
  },
  tab: {
    minWidth: 0,
    // minHeight: 80,
    color: theme.palette.text.primary,
    fontSize: theme.typography.subtitle2.fontSize,
    zIndex: 1,
  },
  indicator: {
    color: 'theme.palette.text.primary',
    backgroundColor: 'transparent',
    borderLeft: 'transparent',
    opacity: '1',
    width: '100%',
    zIndex: 0,
  },

  sidebarIcon: {
    border: '2px solid #0056b3b0',
    // opacity: '.5',
    padding: '5px',
    width: '40px',
    height: '40px',
    borderRadius: '8px',

    '&:hover': {
      border: '2px solid #0056B3',
      opacity: '1',
    },
    '&;selected': {
      border: '2px solid #0056B3',
      opacity: '1',
    },
  },
}));

/**
 * Navigation
 *
 * @component The main interface for router/page-based navigation.
 *
 * @param {boolean} fullPage Takes full width of the page if true
 * @param {function} toggleStatus Defines the status of Sidebar drawer Open/Close
 *
 * @returns {JSX.Element}
 *
 */
export const Navigation = ({
  fullPage,
  toggleStatus,
}: {
  fullPage?: boolean;
  toggleStatus?(toogle: boolean): void;
}): JSX.Element => {
  const classes = useStyles();
  const location = useLocation();

  const routes = useMemo(
    () => [
      // QUICKSTART_ROUTE, // TODO: Hide QuickStart tab when ventilator is on. Need to tap into redux store.
      // MODES_ROUTE,
      ALARMS_ROUTE,
      // VALUES_ROUTE,
      SETTINGS_ROUTE,
    ],
    [],
  );

  const routePath = routes.find((route) => location.pathname.startsWith(route.path));
  /**
   * State to manage Route path
   * RoutePath is an object with 'key(string)' which is a unique identifier for each route
   * The routes on the sidebar are highlighted based on the active routePath key
   */
  const [route, setRoute] = React.useState(routePath ? routePath.key : 0);

  /**
   * Triggers once to update current route in State
   * On Initialization, current route is selected and highlighted on the sidebar
   */
  useEffect(() => {
    const routePath = routes.find((route) => location.pathname.startsWith(route.path));
    setRoute(routePath ? routePath.key : 0);
  }, [location, routes]);

  /**
   * Function for handling route changes
   * updates the current route on change, and highlights the navigated route icon
   *
   * @param {React.ChangeEvent<Record<string, unknown>>} event DOM Change Event
   * @param {number} newRoute Route path to be navigated
   *
   */
  const handleRouteChange = (
    event: React.ChangeEvent<Record<string, unknown>>,
    newRoute: number,
  ) => {
    if (toggleStatus) {
      toggleStatus(false);
    }
    setRoute(newRoute);
  };

  /**
   * RouteLabel
   *
   * @component Handles route labels
   *
   * @returns {JSX.Element}
   *
   */
  const RouteLabel = (
    props: PropsWithChildren<{ fullPage?: boolean; label: string }>,
  ): JSX.Element => {
    const { fullPage, label, children } = props;
    if (fullPage) {
      return (
        <React.Fragment>
          {children}
          <br />
          <Typography variant="subtitle2" align="center">
            {label}
          </Typography>
        </React.Fragment>
      );
    }
    return (
      <React.Fragment>
        <div className={classes.sidebarIcon}>{children}</div>
        <div>
          <Typography variant="subtitle2" align="center">
            {label}
          </Typography>
        </div>
      </React.Fragment>
    );
  };

  return (
    <div className={classes.root}>
      <Tabs
        orientation="vertical"
        value={route}
        onChange={handleRouteChange}
        className={classes.tabs}
        classes={{ indicator: classes.indicator }}
      >
        {routes.map((route) => {
          return (
            <Tab
              value={route.key}
              label={
                <RouteLabel fullPage={fullPage} label={route.label}>
                  <route.icon style={{ width: '100%' }} />
                </RouteLabel>
              }
              component={Link}
              to={route.path}
              className={classes.tab}
            />
          );
        })}
      </Tabs>
    </div>
  );
};

export default Navigation;
