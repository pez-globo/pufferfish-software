/**
 * @summary Component responsible for Application Routing
 *
 * For any new page to be accessible via url path,
 * It is required to add an Route object in `constants` file
 * And add it inside the `<Switch>` tag in below format
 *
 * <RouteLayout component={PageComponent} path={UrlPath} />
 *
 * Use appropriate RouteLayout based on available layouts in `app/Layout` directory'
 *
 * To Visibly see the Route in Sidebar, Update `routes` array in `Navigation` file
 */
import React from 'react';
import { useSelector } from 'react-redux';
import { Switch } from 'react-router-dom';
import { getParametersIsVentilating } from '../../../store/controller/selectors';
import AlarmLimitsPage from '../../alarms/limits/AlarmLimitsPage';
import LandingPageRoute from './layouts/LandingPageRoute';
import ScreensaverRoute from './layouts/ScreensaverRoute';
import SidebarRoute from './layouts/SidebarRoute';
import SidebarSlideRoute from './layouts/SidebarSlideRoute';
import DashboardPage from '../../dashboard/DashboardPage';
import LogsPage from '../../event-logs/LogsPage';
import {
  ALARMS_ROUTE,
  DASHBOARD_ROUTE,
  LOGS_ROUTE,
  QUICKSTART_ROUTE,
  SCREENSAVER_ROUTE,
  SETTINGS_ROUTE,
} from './constants';
import SettingsPage from '../../settings/SettingsPage';
import QuickStartPage from '../quickstart/QuickStartPage';
import ScreensaverPage from '../screensaver/ScreensaverPage';
import LandingPage from '../landing-page/LandingPage';

/**
 * Routes
 *
 * @component Handling router/page-based navigation.
 *
 * @returns {JSX.Element}
 *
 */
const Routes = (): JSX.Element => {
  const ventilating = useSelector(getParametersIsVentilating);
  const RouteLayout = ventilating ? SidebarSlideRoute : SidebarRoute;

  return (
    <Switch>
      <SidebarSlideRoute exact path={DASHBOARD_ROUTE.path} component={DashboardPage} />
      <RouteLayout path={QUICKSTART_ROUTE.path} component={QuickStartPage} />
      <RouteLayout path={ALARMS_ROUTE.path} component={AlarmLimitsPage} />
      <RouteLayout path={SETTINGS_ROUTE.path} component={SettingsPage} />
      <RouteLayout path={LOGS_ROUTE.path} component={LogsPage} />
      <ScreensaverRoute path={SCREENSAVER_ROUTE.path} component={ScreensaverPage} />
      <LandingPageRoute component={LandingPage} />
    </Switch>
  );
};

export default Routes;
