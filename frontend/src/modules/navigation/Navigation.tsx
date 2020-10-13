import React, { useEffect } from 'react';
import { Link, useLocation } from 'react-router-dom';
import { makeStyles, Theme } from '@material-ui/core/styles';
import { Tabs, Tab, Typography } from '@material-ui/core';
import {
  QUICKSTART_ROUTE,
  VALUES_ROUTE,
  MODES_ROUTE,
  ALARMS_ROUTE,
  SETTINGS_ROUTE,
} from './constants';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    width: '100%',
    height: '100%',
  },
  tabs: {
    alignText: 'center',
    zIndex: 1,
    height: '100%',
    '&;selected': {
      backgroundColor: 'red',
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
    minHeight: 80,
    color: theme.palette.text.primary,
    fontSize: theme.typography.subtitle2.fontSize,
    zIndex: 1,
  },
  indicator: {
    color: 'theme.palette.text.primary',
    backgroundColor: theme.palette.primary.light,
    borderLeft: `4px solid ${theme.palette.primary.main}`,
    opacity: '1',
    width: '100%',
    zIndex: 0,
  },
}));
/**
 * Navigation
 *
 * The main interface for router/page-based navigation.
 */
export const Navigation = (): JSX.Element => {
  const classes = useStyles();
  const location = useLocation();

  const routes = [
    // QUICKSTART_ROUTE, // TODO: Hide QuickStart tab when ventilator is on. Need to tap into redux store.
    MODES_ROUTE,
    ALARMS_ROUTE,
    VALUES_ROUTE,
    SETTINGS_ROUTE,
  ];

  const routePath = routes.find((route) => location.pathname.startsWith(route.path));
  const [route, setRoute] = React.useState(routePath ? routePath.key : 0);

  useEffect(() => {
    const routePath = routes.find((route) => location.pathname.startsWith(route.path));
    setRoute(routePath ? routePath.key : 0);
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [location]);

  const handleRouteChange = (
    event: React.ChangeEvent<Record<string, unknown>>,
    newRoute: number,
  ) => {
    setRoute(newRoute);
  };

  return (
    <div className={classes.root}>
      <Tabs
        orientation="vertical"
        variant="fullWidth"
        value={route}
        onChange={handleRouteChange}
        centered
        className={classes.tabs}
        classes={{ indicator: classes.indicator }}
      >
        {routes.map((route) => {
          return (
            <Tab
              value={route.key}
              label={
                <div>
                  <route.icon style={{ width: '100%' }} />
                  <br />
                  <Typography variant="subtitle2" align="center">
                    {route.label}
                  </Typography>
                </div>
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
