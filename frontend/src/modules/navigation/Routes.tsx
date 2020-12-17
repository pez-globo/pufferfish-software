import React from 'react';
import { useSelector } from 'react-redux';
import { Switch } from 'react-router-dom';
import { getIsVentilating } from '../../store/controller/selectors';
import AlarmsPage from '../alarms/AlarmsPage';
import LandingPageRoute from '../app/layouts/LandingPageRoute';
import ScreensaverRoute from '../app/layouts/ScreensaverRoute';
import SidebarRoute from '../app/layouts/SidebarRoute';
import SidebarSlideRoute from '../app/layouts/SidebarSlideRoute';
import DashboardPage from '../dashboard/DashboardPage';
import LandingPage from '../landing-page/LandingPage';
import LogsPage from '../logs/LogsPage';
import ModesPage from '../modes/ModesPage';
import QuickStartPage from '../quickstart/QuickStartPage';
import ScreensaverPage from '../screensaver/ScreensaverPage';
import SettingsPage from '../settings/SettingsPage';
import ValuesPage from '../values/ValuesPage';
import {
  ALARMS_ROUTE,
  DASHBOARD_ROUTE,
  LOGS_ROUTE,
  MODES_ROUTE,
  QUICKSTART_ROUTE,
  SCREENSAVER_ROUTE,
  SETTINGS_ROUTE,
  VALUES_ROUTE,
} from './constants';

const Routes = (): JSX.Element => {
  const ventilating = useSelector(getIsVentilating);
  const RouteLayout = ventilating ? SidebarSlideRoute : SidebarRoute;

  return (
    <Switch>
      <RouteLayout exact path={DASHBOARD_ROUTE.path} component={DashboardPage} />
      <RouteLayout path={QUICKSTART_ROUTE.path} component={QuickStartPage} />
      <RouteLayout path={MODES_ROUTE.path} component={ModesPage} />
      <RouteLayout path={ALARMS_ROUTE.path} component={AlarmsPage} />
      <RouteLayout path={VALUES_ROUTE.path} component={ValuesPage} />
      <RouteLayout path={SETTINGS_ROUTE.path} component={SettingsPage} />
      <RouteLayout path={LOGS_ROUTE.path} component={LogsPage} />
      <ScreensaverRoute path={SCREENSAVER_ROUTE.path} component={ScreensaverPage} />
      <LandingPageRoute component={LandingPage} />
    </Switch>
  );
};

export default Routes;
