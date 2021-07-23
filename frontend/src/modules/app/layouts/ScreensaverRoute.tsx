/**
 * @summary Layout for screensaver page
 * see Routes.tsx where the main content is supplied through the "component" prop
 *
 */
import { AppBar, Grid, makeStyles, Theme } from '@material-ui/core';
import React, { PropsWithChildren, useEffect } from 'react';
import { useSelector } from 'react-redux';
import { Route, RouteProps, useHistory } from 'react-router-dom';
import { getParametersIsVentilating } from '../../../store/controller/selectors';
import ClockIcon from '../../icons/ClockIcon';
import { DASHBOARD_ROUTE, QUICKSTART_ROUTE } from '../../navigation/constants';
import Alarms from '../../alarms/Alarms';
import UserActivity from '../UserActivity';
import { HeaderClock, PowerIndicator } from '../ToolBar';
import OverlayScreen from '../OverlayScreen';
import { getAlarmNotifyStatus } from '../../../store/app/selectors';

const useStyles = makeStyles((theme: Theme) => ({
  root: {
    height: '100vh',
    backgroundColor: theme.palette.background.default,
    flexWrap: 'nowrap',
    display: 'grid',
    gridTemplateAreas: `
                    'vent vent'`,
    gridTemplateColumns: '90px 1fr',
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
  marginRight: {
    marginRight: theme.spacing(2),
  },
  paddingRight: {
    paddingRight: theme.spacing(1),
  },
  screensaverButton: {
    minWidth: 0,
    borderRadius: 8,
    marginRight: '0px',
    padding: '6px 10px',
    lineHeight: 'normal',
  },
  borderOverlay: {
    border: '4px solid red',
  },
}));

/**
 * ScreensaverLayout
 *
 * @component Component for screensaver layout
 *
 * @returns {JSX.Element}
 */
const ScreensaverLayout = ({ children }: PropsWithChildren<unknown>): JSX.Element => {
  const classes = useStyles();

  return (
    <React.Fragment>
      <OverlayScreen />
      <Grid container justify="center" alignItems="stretch" className={classes.root}>
        <ContentComponent>{children}</ContentComponent>
      </Grid>
      <UserActivity />
    </React.Fragment>
  );
};

/**
 * ContentComponent
 *
 * @component Component for displaying the main content
 *
 * @returns {JSX.Element}
 */
const ContentComponent = React.memo(({ children }: PropsWithChildren<unknown>) => {
  const classes = useStyles();
  const history = useHistory();
  const ventilating = useSelector(getParametersIsVentilating);
  const notifyAlarm = useSelector(getAlarmNotifyStatus);
  const [showBorder, setShowBorder] = React.useState(false);

  /**
   * Triggers when Alarm event is active (Referenced in `OverlayScreen` )
   * RED_BORDER reduxs store is dispatched when alarm is active
   * It adds a red border around the page
   */
  useEffect(() => {
    setShowBorder(notifyAlarm);
  }, [notifyAlarm]);

  /**
   * Triggerred when clicking on anywhere in page
   */
  const onClick = () => {
    history.push(ventilating ? DASHBOARD_ROUTE.path : QUICKSTART_ROUTE.path);
  };

  return (
    <Grid
      onClick={onClick}
      container
      item
      className={`${showBorder && classes.borderOverlay} ${classes.main}`}
    >
      <Grid container item alignItems="center">
        <AppBar color="transparent" elevation={0} position="static" style={{ display: 'contents' }}>
          <Grid item style={{ margin: '0 auto' }} />
          <Grid item xs className={classes.marginRight} style={{ margin: '0 auto' }}>
            <Alarms />
          </Grid>
          <Grid container item xs justify="flex-end" alignItems="center">
            <PowerIndicator />
            <HeaderClock />
            <ClockIcon style={{ fontSize: '2.5rem' }} />
          </Grid>
        </AppBar>
      </Grid>
      <Grid container item className={classes.mainContainer}>
        {children}
      </Grid>
    </Grid>
  );
});

/**
 * ScreensaverRoute
 *
 * @component Component for displaying the screen saver layout
 *
 * @returns {JSX.Element | null}
 */
const ScreensaverRoute = ({ component: Component, ...rest }: RouteProps): JSX.Element | null => {
  if (!Component) return null;
  return (
    <Route
      {...rest}
      render={(matchProps) => (
        <ScreensaverLayout>
          <Component {...matchProps} />
        </ScreensaverLayout>
      )}
    />
  );
};

export default ScreensaverRoute;
